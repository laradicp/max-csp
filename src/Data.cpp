#include "Data.h"

#include <fstream>
#include <math.h>
#include <algorithm>

Data::Data(string filePath, bool cumulative)
{
    this->cumulative = cumulative;

    readInstance(filePath);
    id = -1;

    if(this->cumulative)
    {
        retrieveId(filePath);
        readUnscheduled(filePath);
    }

    definePaths(filePath);

    ub = -1;
}

void Data::retrieveId(string filePath)
{
    for(unsigned int i = 0; i < filePath.size(); i++)
    {
        if(filePath[i] == '_')
        {
            string idStr;
            for(unsigned int j = i + 1; j < filePath.size(); j++)
            {
                if(filePath[j] == '_')
                {
                    id = stoi(idStr);
                    break;
                }

                idStr += filePath[j];
            }

            break;
        }
    }
}

void Data::readInstance(string filePath)
{
    ifstream instance;
    instance.open(filePath);

    instance >> nbCars;
    instance >> nbOptions;
    instance >> nbClasses;

    maxCarsPerWindow.resize(nbOptions);
    for(int i = 0; i < nbOptions; i++)
    {
        instance >> maxCarsPerWindow[i];
    }

    windowSize.resize(nbOptions);
    for(int i = 0; i < nbOptions; i++)
    {
        instance >> windowSize[i];
    }

    // sort the options by the score
    vector<pair<double, int>> scoreIds;
    for(int i = 0; i < nbOptions; i++)
    {
        scoreIds.push_back(
            make_pair((double)(windowSize[i] - maxCarsPerWindow[i])/maxCarsPerWindow[i], i)
        );
    }
    sort(scoreIds.begin(), scoreIds.end());
    
    // update maxCarsPerWindow and windowSize
    vector<int> maxCarsPerWindowCopy = maxCarsPerWindow;
    vector<int> windowSizeCopy = windowSize;
    for(int i = 0; i < nbOptions; i++)
    {
        maxCarsPerWindow[i] = maxCarsPerWindowCopy[scoreIds[i].second];
        windowSize[i] = windowSizeCopy[scoreIds[i].second];
    }

    nbCarsPerClass.resize(nbClasses);
    options.resize(nbClasses);
    for(int i = 0; i < nbClasses; i++)
    {
        int idx;
        instance >> idx; // the index is ignored

        instance >> nbCarsPerClass[i];
        
        options[i].resize(nbOptions);
        for(int j = 0; j < nbOptions; j++)
        {
            int hasOption;
            instance >> hasOption;
            options[i][scoreIds[j].second] = hasOption == 1;
        }
    }

    instance.close();
}

void Data::readUnscheduled(string filePath)
{
    if(id == 0)
    {
        return;
    }

    for(int i = filePath.size() - 1; i >= 0; i--)
    {
        if(filePath[i] == '/')
        {
            for(int j = 1; j < filePath.size(); j++)
            {
                if(filePath[i + j] == '_')
                {
                    filePath = filePath.substr(i + 1, j - 1);
                    break;
                }
            }

            break;
        }
    }

    ifstream unscheduled;
    unscheduled.open("unscheduled/" + filePath + "_" + to_string(id - 1) + ".out");

    int idx, nbUnscheduledCars;
    unscheduled >> idx;
    unscheduled >> nbUnscheduledCars;
    while(!unscheduled.eof())
    {
        nbCarsPerClass[idx] += nbUnscheduledCars;
        nbCars += nbUnscheduledCars;

        unscheduled >> idx;
        unscheduled >> nbUnscheduledCars;
    }

    unscheduled.close();
}

void Data::definePaths(string filePath)
{
    resultPath = "results/";
    unscheduledPath.clear();

    for(int i = filePath.size() - 1; i >= 0; i--)
    {
        if(filePath[i] == '/')
        {
            for(int j = 1; j < filePath.size(); j++)
            {
                resultPath += filePath[i + j];

                if(unscheduledPath.empty()&&(filePath[i + j] == '_'))
                {
                    unscheduledPath = "unscheduled/" + filePath.substr(i + 1, j) +
                        to_string(id) + ".out";
                }
                else if(filePath[i + j] == '.')
                {
                    resultPath += "out";
                    return;
                }
            }

            break;
        }
    }

    resultPath += ".out";
}

int Data::getId()
{
    return id;
}

int Data::getNbCars()
{
    return nbCars;
}

int Data::getNbClasses()
{
    return nbClasses;
}

int Data::getNbOptions()
{
    return nbOptions;
}

int Data::getMaxCarsPerWindow(int j)
{
    return maxCarsPerWindow[j];
}

int Data::getWindowSize(int j)
{
    return windowSize[j];
}

int Data::getNbCarsPerClass(int i)
{
    return nbCarsPerClass[i];
}

bool Data::getOption(int i, int j)
{
    return options[i][j];
}

string Data::getResultPath()
{
    return resultPath;
}

string Data::getUnscheduledPath()
{
    return unscheduledPath;
}

int Data::getUpperBound()
{
    if(ub == -1)
    {
        auto start = chrono::system_clock::now();

        // setup
        vector<int> nbCarsPerOption(nbOptions, 0);
        vector<int> surplus(nbOptions, 0);
        vector<vector<int>> intersect(nbOptions, vector<int>(nbOptions, 0));
        for(int j1 = 0; j1 < nbOptions; j1++)
        {
            for(int i = 0; i < nbClasses; i++)
            {
                if(getOption(i, j1))
                {
                    nbCarsPerOption[j1] += getNbCarsPerClass(i);
                    for(int j2 = 0; j2 < nbOptions; j2++)
                    {
                        if(getOption(i, j2))
                        {
                            intersect[j1][j2] += getNbCarsPerClass(i);
                        }
                    }
                }
            }

            surplus[j1] = nbCarsPerOption[j1] - (getMaxCarsPerWindow(j1)*
                floor(nbCars/(double)(getWindowSize(j1))) +
                min(getMaxCarsPerWindow(j1), nbCars%getWindowSize(j1)));
            surplus[j1] = max(surplus[j1], 0);

            for(int j2 = 0; j2 < j1; j2++)
            {
                surplus[j1] -= min(surplus[j2], intersect[j1][j2]);
                surplus[j1] = max(surplus[j1], 0);
            }
        }

        ub = nbCars;
        for(int j = 0; j < nbOptions; j++)
        {
            ub -= surplus[j];
        }

        elapsedTimeUB = chrono::system_clock::now() - start;
    }

    return ub;
}

int Data::used(int r, int s, vector<int> &nbCarsPerScore, vector<vector<int>> &classesPerScore,
    vector<vector<int>> &intersection)
{
    calculateLB(s - 1, nbCarsPerScore, classesPerScore, intersection);

    int nbFittingCars = getMaxCarsPerWindow(s - 1)*(int)floor(lb[r]/(double)getWindowSize(s - 1))
        + min(getMaxCarsPerWindow(s - 1), lb[r]%getWindowSize(s - 1));
    if(r == s)
    {
        return min(nbFittingCars, nbCarsPerScore[s]);
    }

    return used(r, s - 1, nbCarsPerScore, classesPerScore, intersection) + min(nbFittingCars, intersection[s][r]);
}

void Data::calculateLB(int s, vector<int> &nbCarsPerScore, vector<vector<int>> &classesPerScore,
    vector<vector<int>> &intersection)
{
    if(lb[s] != -1)
    {
        return;
    }

    if(s == 0)
    {
        // update primal solution
        int nbScheduled = 0;
        for(int iIdx = 0; iIdx < classesPerScore[s].size(); iIdx++)
        {
            for(; nbScheduled < nbCarsPerScore[s]; nbScheduled++)
            {
                if(unscheduled[classesPerScore[s][iIdx]] == 0)
                {
                    break;
                }

                primalSol.push_back(classesPerScore[s][iIdx]);
                unscheduled[classesPerScore[s][iIdx]]--;
            }

            if(nbScheduled == nbCarsPerScore[s])
            {
                break;
            }
        }

        lb[s] = nbCarsPerScore[s];

        return;
    }
    
    calculateLB(s - 1, nbCarsPerScore, classesPerScore, intersection);

    int violations = 0;
    int usedPos = 0;
    if(s == 1)
    {
        usedPos = used(s, s, nbCarsPerScore, classesPerScore, intersection);
    }
    
    // r = 0 does not correspond to any option, therefore it is not used for violations
    for(int r = 1; r < s; r++)
    {
        usedPos = used(r, s, nbCarsPerScore, classesPerScore, intersection);
        violations += min(intersection[s][r], usedPos);
    }

    int nbCarsToSchedule = max(min(getMaxCarsPerWindow(s - 1)*
        ((int)floor(lb[s - 1]/(double)(getWindowSize(s - 1) - getMaxCarsPerWindow(s - 1))) + 1),
        nbCarsPerScore[s]) - violations, 0);
    
    // update primal solution
    int nbScheduled = 0;
    for(int t = 0; t < primalSol.size(); t++)
    {
        if(nbScheduled == nbCarsToSchedule)
        {
            break;
        }

        for(int iIdx = 0; iIdx < classesPerScore[s].size(); iIdx++)
        {	
            if(unscheduled[classesPerScore[s][iIdx]] == 0)
            {
                continue;
            }

            bool feasibleClass = true; // true if inserting class i = classesPerScore[s][iIdx] in position t is feasible

            for(int j = 0; j < s; j++) // M can have cars with option up to j = s - 1
            {
                if(getOption(classesPerScore[s][iIdx], j))
                {
                    int begin = max(t - getWindowSize(j) + 1, 0);
                    int end = min(t + getWindowSize(j) - 1, (int)primalSol.size());
                    int sum = 1;

                    for(int k = begin; k < t; k++)
                    {
                        if(getOption(primalSol[k], j))
                        {
                            sum++;
                        }
                    }

                    if(sum > getMaxCarsPerWindow(j))
                    {
                        feasibleClass = false;
                        break;
                    }

                    for(int k = t; k < end; k++)
                    {
                        if((k - getWindowSize(j) + 1 >= 0)&&
                            (getOption(primalSol[k - getWindowSize(j) + 1], j)))
                        {
                            sum--;
                        }

                        if(getOption(primalSol[k], j))
                        {
                            sum++;

                            if(sum > getMaxCarsPerWindow(j))
                            {
                                feasibleClass = false;
                                break;
                            }
                        }
                    }
                }

                if(!feasibleClass)
                {
                    break;
                }
            }

            if(feasibleClass)
            {
                primalSol.insert(primalSol.begin() + t, classesPerScore[s][iIdx]);
                unscheduled[classesPerScore[s][iIdx]]--;

                if(++nbScheduled == nbCarsToSchedule)
                {
                    break;
                }
            }
        }
    }

    lb[s] = lb[s - 1] + nbScheduled;
}

int Data::getLowerBound()
{
    if(primalSol.empty())
    {
        auto start = chrono::system_clock::now();

        lb.resize(nbOptions + 1, -1);
        unscheduled.resize(nbClasses);
        for(int i = 0; i < nbClasses; i++)
        {
            unscheduled[i] = getNbCarsPerClass(i);
        }

        vector<int> nbCarsPerScore(nbOptions + 1, 0);
        vector<vector<int>> classesPerScore(nbOptions + 1, vector<int>(0, 0));
        vector<vector<int>> intersection(nbOptions + 1, vector<int>(nbOptions + 1, 0));

        // Set |M| and intersections
        for(int i = 0; i < nbClasses; i++)
        {
            bool noOption = true;
            for(int s = 1; s < nbOptions + 1; s++)
            {
                if(getOption(i, s - 1))
                {
                    noOption = false;
                    bool mostRestrictive = true;
                    for(int r = s + 1; r < nbOptions + 1; r++)
                    {
                        if(getOption(i, r - 1))
                        {
                            mostRestrictive = false;
                            break;
                        }
                    }
                    if(mostRestrictive)
                    {
                        // Update |M|
                        nbCarsPerScore[s] += getNbCarsPerClass(i);
                        classesPerScore[s].push_back(i);
                        // Update intersections
                        for(int r = 1; r < s; r++)
                        {
                            if(getOption(i, r - 1))
                            {
                                intersection[s][r] += getNbCarsPerClass(i);
                            }
                        }
                    }
                }
            }

            if(noOption)
            {
                // Update |M|
                nbCarsPerScore[0] += getNbCarsPerClass(i);
                classesPerScore[0].push_back(i);
            }
        }
        
        calculateLB(nbOptions, nbCarsPerScore, classesPerScore, intersection);

        if(lb[nbOptions] == 0) // trivial solution
        {
            for(int i = 0; i < nbClasses; i++)
            {
                if(unscheduled[i] > 0)
                {
                    primalSol.push_back(i);
                    unscheduled[i]--;
                    break;
                }
            }
        }

        elapsedTimeLB = chrono::system_clock::now() - start;
    }

    return primalSol.size();
}

int Data::getPrimalSol(int t)
{
    getLowerBound();
    return primalSol[t];
}

int Data::getUnscheduled(int i)
{
    getLowerBound();
    return unscheduled[i];
}

double Data::getElapsedTimeUB()
{
    return elapsedTimeUB.count();
}

double Data::getElapsedTimeLB()
{
    return elapsedTimeLB.count();
}

bool Data::isCumulative()
{
    return cumulative;
}
