#include "Data.h"

#include <fstream>

Data::Data(string filePath, bool cumulative)
{
    readInstance(filePath);

    retrieveId(filePath);

    if(cumulative)
    {
        readUnscheduled(filePath);
    }

    definePaths(filePath);
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

    nbCarsPerClass.resize(nbClasses);
    options.resize(nbClasses);
    for(int i = 0; i < nbClasses; i++)
    {
        int idx;
        instance >> idx; // the index is ignored

        instance >> nbCarsPerClass[i];

        for(int j = 0; j < nbOptions; j++)
        {
            int hasOption;
            instance >> hasOption;
            options[i].push_back(hasOption == 1);
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

    ifstream unscheduled;
    unscheduled.open("unscheduled/" + filePath.substr(10, 3) + "_" + to_string(id - 1) + ".out");

    int idx, nbUnscheduledCars;
    unscheduled >> idx;
    unscheduled >> nbUnscheduledCars;
    while(!unscheduled.eof())
    {
        nbCarsPerClass[idx] += nbUnscheduledCars;

        unscheduled >> idx;
        unscheduled >> nbUnscheduledCars;
    }

    unscheduled.close();
}

void Data::definePaths(string filePath)
{
    // the file must be inside the folder instances
    for(unsigned int i = 10; i < filePath.size(); i++)
    {
        if(filePath[i] == '.')
        {
            sequencePath = "sequences/" + filePath.substr(10, i - 10) + ".out";
            unscheduledPath = "unscheduled/" + filePath.substr(10, i - 10) + ".out";
            return;
        }
    }

    sequencePath = "sequences/" + filePath.substr(10, filePath.size() - 10) + ".out";
    unscheduledPath = "unscheduled/" + filePath.substr(10, filePath.size() - 10) + ".out";
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

string Data::getSequencePath()
{
    return sequencePath;
}

string Data::getUnscheduledPath()
{
    return unscheduledPath;
}

int Data::getUpperBound()
{
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

        surplus[j1] = nbCarsPerOption[j1] - ceil(getMaxCarsPerWindow(j1)*nbCars/
                      (double)(getWindowSize(j1)));
        surplus[j1] = surplus[j1] > 0 ? surplus[j1] : 0;

        for(int j2 = 0; j2 < j1; j2++)
        {
            int min = surplus[j2] < intersect[j1][j2] ? surplus[j2] : intersect[j1][j2];
            surplus[j1] -= min;
            surplus[j1] = surplus[j1] > 0 ? surplus[j1] : 0;
        }
    }

    int ub = nbCars;
    for(int j = 0; j < nbOptions; j++)
    {
        ub -= surplus[j];
    }

    return ub;
}

int Data::used(int r, int s, vector<double> &score, vector<int> &nbCarsPerScore,
    vector<vector<int>> &classesPerScore, vector<vector<int>> &intersection)
{
    calculateLB(s - 1, score, nbCarsPerScore, classesPerScore, intersection);

    int nbFittingCars = ceil(lb[s - 1]/score[s]);
    if(r == s)
    {
        return nbFittingCars < nbCarsPerScore[s] ? nbFittingCars : nbCarsPerScore[s] ;
    }

    int min = nbFittingCars < intersection[s][r] ? nbFittingCars : intersection[s][r];
    return used(r, s - 1, score, nbCarsPerScore, classesPerScore, intersection) + min;
}

void Data::calculateLB(int s, vector<double> &score, vector<int> &nbCarsPerScore,
    vector<vector<int>> &classesPerScore, vector<vector<int>> &intersection)
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
    
    calculateLB(s - 1, score, nbCarsPerScore, classesPerScore, intersection);

    int min = ceil(lb[s - 1]/score[s]) < nbCarsPerScore[s] ? ceil(lb[s - 1]/score[s]) : nbCarsPerScore[s],
        violations = 0;
    int usedPos = 0;
    if(s == 1)
    {
        usedPos = used(s, s, score, nbCarsPerScore, classesPerScore, intersection);
    }
    
    // r = 0 does not correspond to any option, therefore it is not used for violations
    for(int r = 1; r < s; r++)
    {
        usedPos = used(r, s, score, nbCarsPerScore, classesPerScore, intersection);
        violations += intersection[s][r] < usedPos ? intersection[s][r] : usedPos;
    }

    int max = min - violations > 0 ? min - violations : 0; // number of cars added to solution
    
    // update primal solution
    int nbScheduled = 0;
    for(int t = 0; t < primalSol.size(); t++)
    {
        if(nbScheduled == max)
        {
            break;
        }

        for(int iIdx = 0; iIdx < classesPerScore[s].size(); iIdx++)
        {	
            if(getNbCarsPerClass(classesPerScore[s][iIdx]))
            {
                bool feasibleClass = true; // true if inserting class i = classesPerScore[s][iIdx] in position t is feasible

                for(int j = 0; j < s; j++) // M can have cars with option up to j = s - 1
                {
                    if(getOption(classesPerScore[s][iIdx], j))
                    {
                        int begin = t - getWindowSize(j) + 1 > 0 ? t - getWindowSize(j) + 1 : 0;
                        int end = t + getWindowSize(j) - 1 < primalSol.size() ? t + getWindowSize(j) - 1 : primalSol.size();
                        int sum = 1;

                        for(int k = begin; k < t; k++)
                        {
                            if(data.getOption(primalSol[k], j))
                            {
                                sum++;
                            }
                        }

                        if(sum > data.getMaxCarsPerWindow(j))
                        {
                            feasibleClass = false;
                            break;
                        }

                        for(int k = t; k < end; k++)
                        {
                            if((k - data.getWindowSize(j) + 1 >= 0)&&
                                (data.getOption(primalSol[k - data.getWindowSize(j) + 1], j)))
                            {
                                sum--;
                            }

                            if(data.getOption(primalSol[k], j))
                            {
                                sum++;

                                if(sum > data.getMaxCarsPerWindow(j))
                                {
                                    feasibleClass = false;
                                    break;
                                }
                            }
                        }
                    }
                }

                if(feasibleClass)
                {
                    primalSol.insert(primalSol.begin() + t, classesPerScore[s][iIdx]);
                    unscheduled[classesPerScore[s][iIdx]]--;

                    if(++nbScheduled == max)
                    {
                        break;
                    }
                }
            }
        }
    }

    lb[s] = lb[s - 1] + max;
}

int Data::getLowerBound()
{
    if(primalSol.empty())
    {
        lb.resize(nbOptions + 1, -1);
        unscheduled.resize(nbClasses);
        for(int i = 0; i < nbClasses; i++)
        {
            unscheduled[i] = getNbCarsPerClass(i);
        }

        vector<double> score(nbOptions + 1, 0);
        vector<int> nbCarsPerScore(nbOptions + 1, 0);
        vector<vector<int>> classesPerScore(nbOptions + 1, vector<int>(0, 0));
        vector<vector<int>> intersection(nbOptions + 1, vector<int>(nbOptions + 1, 0));

        // Set scores
        for(int s = 1; s < nbOptions + 1; s++)
        {
            score[s] = (getWindowSize(s - 1) - getMaxCarsPerWindow(s - 1))/(double)(getMaxCarsPerWindow(s - 1));
        }

        // Set |M| and intersections
        for(int i = 0; i < nbClasses; i++)
        {
            bool noOption = true;
            for(int s = 1; s < nbOptions + 1; s++)
            {
                if(getCadencesPerFamily(i, s - 1))
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
        
        calculateLB(nbOptions, score, nbCarsPerScore, classesPerScore, intersection);

        if(lb[nbOptions] == 0)
        {
            for(int i = 0; i < nbClasses; i++)
            {
                if(getNbCarsPerClass(i) > 0)
                {
                    primalSol.push_back(i);
                    unscheduled[i]--;
                    break;
                }
            }
        }
    }

    return primalSol.size();
}

int Data::getPrimalSol(int t)
{
    getLowerBound();
    return primalSol[t];
}
