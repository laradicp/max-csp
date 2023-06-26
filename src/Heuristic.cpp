#include "Heuristic.h"
#include <fstream>

void Heuristic::calculateMaxWindow()
{
    maxWindow = 0;

    for(int j = 0; j < data.getNbOptions(); j++)
    {
        if(data.getWindowSize(j) > maxWindow)
        {
            maxWindow = data.getWindowSize(j);
        }
    }
}

void Heuristic::initialization()
{
    sequence.clear();
    unscheduled.resize(data.getNbClasses());
	for(int i = 0; i < data.getNbClasses(); i++)
	{
		unscheduled[i] = data.getNbCarsPerClass(i);
	}
}

bool Heuristic::isInfeasible()
{
    for(int t = 0; t < sequence.size(); t++)
    {
        for(int j = 0; j < data.getNbOptions(); j++)
        {
            if(data.getOption(sequence[t], j))
            {
                int sum = 1;
                int end = min(t + data.getWindowSize(j), (int)sequence.size());
                for(int k = t + 1; k < end; k++)
                {
                    if(data.getOption(sequence[k], j))
                    {
                        sum++;
                    }
                }

                if(sum > data.getMaxCarsPerWindow(j))
				{
                    return true;
				}
            }
        }
    }

    return false;
}

void Heuristic::construction()
{
    vector<int> feasibleClasses;

    for(int i = 0; i < data.getNbClasses(); i++)
    {
        if(unscheduled[i] > 0)
            feasibleClasses.push_back(i);
    }

    while(feasibleClasses.size())
    {
        int alfa = rand()%feasibleClasses.size();

        sequence.push_back(feasibleClasses[alfa]);
        unscheduled[feasibleClasses[alfa]]--;

        feasibleClasses.clear();

        for(int i = 0; i < data.getNbClasses(); i++)
        {
            if(unscheduled[i] > 0)
            {
                bool feasible = true;

                for(int j = 0; j < data.getNbOptions(); j++)
                {
                    if(data.getOption(i, j))
                    {
                        int begin = max((int)sequence.size() - data.getWindowSize(j) + 1, 0);
						int sum = 1;

						for(int k = begin; k < sequence.size(); k++)
						{
							if(data.getOption(sequence[k], j))
							{
								sum++;
							}
						}

						if(sum > data.getMaxCarsPerWindow(j))
						{
							feasible = false;
							break;
						}
                    }

                    if(!feasible)
                    {
                        break;
                    }
                }

                if(feasible)
                {
                    feasibleClasses.push_back(i);
                }
            }
        }
    }
}

bool Heuristic::feasibleInsertion(int t, int i)
{
    if(unscheduled[i] == 0)
    {
        return false;
    }

    for(int j = 0; j < data.getNbOptions(); j++)
    {
        if(data.getOption(i, j))
        {
            int begin = max(t - data.getWindowSize(j) + 1, 0);
            int end = min(t + data.getWindowSize(j) - 1, (int)sequence.size());
            int sum = 1;
            
            for(int k = begin; k < t; k++)
            {
                if(data.getOption(sequence[k], j))
                {
                    sum++;
                }
            }

            if(sum > data.getMaxCarsPerWindow(j))
            {
                return false;
            }

            for(int k = t; k < end; k++)
            {
                if((k - data.getWindowSize(j) + 1 >= 0)&&
                    (data.getOption(sequence[k - data.getWindowSize(j) + 1], j)))
                {
                    sum--;
                }

                if(data.getOption(sequence[k], j))
                {
                    sum++;

                    if(sum > data.getMaxCarsPerWindow(j))
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Heuristic::insertion()
{
    int begin = rand()%sequence.size();

    for(int t = begin; t < sequence.size(); t++)
    {
        for(int i = 0; i < data.getNbClasses(); i++)
        {	
            if(feasibleInsertion(t, i))
            {
                sequence.insert(sequence.begin() + t, i);
                unscheduled[i]--;
            }
        }
    }

    for(int t = 0; t < begin; t++)
    {
        for(int i = 0; i < data.getNbClasses(); i++)
        {	
            if(feasibleInsertion(t, i))
            {
                sequence.insert(sequence.begin() + t, i);
                unscheduled[i]--;
                begin++;
            }
        }
    }
}

bool Heuristic::feasibleRemoval(int t)
{
    for(int j = 0; j < data.getNbOptions(); j++)
    {
        if((data.getMaxCarsPerWindow(j) == 1)&&data.getOption(sequence[t], j))
        // if s[t] has option j, there is no job with option j in the range
        {
            continue;
        }

        int begin = max(t - data.getWindowSize(j) + 1, 0); 
        int end = min(t + data.getWindowSize(j), (int)sequence.size());
        int sum = 0;

        for(int k = begin; k < t; k++)
        {
            if(data.getOption(sequence[k], j))
            {
                sum++;
            }
        }

        for(int k = t + 1; k < end; k++)
        {
            if((k - data.getWindowSize(j) - 1 >= begin)&&
                (data.getOption(sequence[k - data.getWindowSize(j) - 1], j)))
            {
                sum--;
            }

            if(data.getOption(sequence[k], j))
            {
                sum++;

                if(sum > data.getMaxCarsPerWindow(j))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

// for removal, beginSearch > 0 and endSearch < sSize, because there is never a search in these positions
void Heuristic::removalCL(int beginSearch, int endSearch, list<int> &feasiblePositions, int &feasiblePositionsSize)
{
    // remove candidates that are in the range between beginSearch and endSearch, which will be updated later
    for(auto iter = feasiblePositions.begin(); iter != feasiblePositions.end();)
    {
        if(*iter <= endSearch)
        {
            if(*iter >= beginSearch)
            {
                feasiblePositions.erase(iter++);
                feasiblePositionsSize--;
            }
            else
            {
                iter++;
            }
        }
        else
        {
            (*iter)--;
            iter++;
        }
    }

    // first and last feasible positions should always be first and last positions of the solution
    if(feasiblePositionsSize == 0)
    {
        feasiblePositions.push_back(0);
        feasiblePositions.push_back(sequence.size() - 1);
        feasiblePositionsSize += 2;
    }

    for(int t = beginSearch; t < endSearch; t++)
    {
        if(feasibleRemoval(t))
        {
            feasiblePositions.push_back(t);
            feasiblePositionsSize++;
        }
    }
}

void Heuristic::removal(int t)
{
    unscheduled[sequence[t]]++;
    sequence.erase(sequence.begin() + t);
}

bool Heuristic::feasibleSwap(int t1, int t2)
{
    if(sequence[t1] == sequence[t2])
    {
        return false;
    }

    if(t1 > t2)
    {
        int aux = t1;
        t1 = t2;
        t2 = aux;
    }

    for(int j = 0; j < data.getNbOptions(); j++)
    {
        // if both jobs or none have option j, there is no need to check feasibility
        if(data.getOption(sequence[t1], j))
        {
            if(!data.getOption(sequence[t2], j))
            {
                int begin = max(t2 - data.getWindowSize(j) + 1, 0);
                int end = min(t2 + data.getWindowSize(j), (int)sequence.size());
                int sum = 1;

                if(data.getMaxCarsPerWindow(j) == 1)
                {
                    if(t1 > begin)
                    {
                        begin = t1 + data.getWindowSize(j);
                        // there is no job with option j closer to t1 than
                        // data.getWindowSize(j) - 1 positions
                    }
                }
                else if(data.getMaxCarsPerWindow(j) == data.getWindowSize(j) - 1)
                {
                    if(t1 > begin)
                    {
                        begin = t1 + 1;
                        // position t1 will be filled by sequence[t2], which doesn't have option j
                    }
                }

                for(int k = begin; k < t2; k++)
                {
                    if(k == t1)
                    {
                        continue;
                    }

                    if(data.getOption(sequence[k], j))
                    {
                        sum++;
                    }
                }

                if(sum > data.getMaxCarsPerWindow(j))
                {
                    return false;
                }

                for(int k = t2 + 1; k < end; k++)
                {
                    if((k - data.getWindowSize(j) >= begin)&&(k - data.getWindowSize(j) != t1)&&
                        (data.getOption(sequence[k - data.getWindowSize(j)], j)))
                    {
                        sum--;
                    }

                    if(data.getOption(sequence[k], j))
                    {
                        sum++;

                        if(sum > data.getMaxCarsPerWindow(j))
                        {
                            return false;
                        }
                    }
                }
            }
        }
        else if(data.getOption(sequence[t2], j))
        {
            int begin = max(t1 - data.getWindowSize(j) + 1, 0);
            int end = min(t1 + data.getWindowSize(j), (int)sequence.size());
            int sum = 1;

            if(data.getMaxCarsPerWindow(j) == 1)
            {
                if(t2 < end)
                {
                    end = t2 - data.getWindowSize(j) + 1;
                    // there is no job with option j closer to t2 than
                    // data.getWindowSize(j) - 1 positions
                }
            }
            else if(data.getMaxCarsPerWindow(j) == data.getWindowSize(j) - 1)
            {
                if(t2 < end)
                {
                    end = t2;
                    // position t2 will be filled by sequence[t1], which doesn't have option j
                }
            }

            for(int k = begin; k < t1; k++)
            {
                if(data.getOption(sequence[k], j))
                {
                    sum++;
                }
            }

            if(sum > data.getMaxCarsPerWindow(j))
            {
                return false;
            }

            for(int k = t1 + 1; k < end; k++)
            {
                if((k - data.getWindowSize(j) >= begin)&&
                    (data.getOption(sequence[k - data.getWindowSize(j)], j)))
                {
                    sum--;
                }

                if(k == t2)
                {
                    continue;
                }

                if(data.getOption(sequence[k], j))
                {
                    sum++;

                    if(sum > data.getMaxCarsPerWindow(j))
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Heuristic::swap(int t1, int t2)
{
    int aux = sequence[t1];
    sequence[t1] = sequence[t2];
    sequence[t2] = aux;
}

int Heuristic::perturbationType()
{
    if(pType == 2)
    {
        return rand()%2;
    }

    return pType;
}

void Heuristic::perturbation()
{
    int p = perturbationType();
    int n = min((int)sequence.size()/2, data.getNbCars()/pDiv);

    if(n == 0)
    {
        return;
    }

    if(p == 0) // removal
    {
        list<int> feasiblePositions;
        int feasiblePositionsSize = 0;
        removalCL(1, sequence.size() - 1, feasiblePositions, feasiblePositionsSize);

        int i = rand()%feasiblePositionsSize;
        auto iter = feasiblePositions.begin();
        while(i-- > 0)
        {
            iter++;
        }
        removal(*iter);
        
        for(int j = 1; j < n; j++)
        {
            // update candidate list
            int beginSearch = max(*iter - maxWindow + 1, 1);
            int endSearch = min(*iter + maxWindow - 1, (int)sequence.size() - 1);
            removalCL(beginSearch, endSearch, feasiblePositions, feasiblePositionsSize);

            i = rand()%feasiblePositionsSize;
            iter = feasiblePositions.begin();
            while(i-- > 0)
            {
                iter++;
            }
            removal(*iter);
        }
    }
    else // swap
    {
        list<int> computedPos;
        for(int j = 0; j < n; j++)
        {
            int t1 = rand()%sequence.size();
            while(infeasibleSwapPos[t1] == '1')
            // it will find a '0' because n = min(sequence.size()/2, data.getDimension()/25)
            {
                t1 = (t1 + 1)%sequence.size();
            }

            infeasibleSwapPos[t1] = '1';
            computedPos.push_back(t1);
            
            int begin = rand()%sequence.size();
            for(int t2 = begin; t2 < sequence.size(); t2++)
            {
                if((infeasibleSwapPos[t2] == '0')&&(feasibleSwap(t1, t2)))
                {
                    swap(t1, t2);

                    // clean infeasibleSwapPos
                    for(auto iter = computedPos.begin(); iter != computedPos.end();)
                    {
                        infeasibleSwapPos[*iter] = '0';
                        computedPos.erase(iter++);
                    }
                    
                    break;
                }
            }

            if(!computedPos.empty()) // if swap not already performed, continue search
            {
                for(int t2 = 0; t2 < begin; t2++)
                {
                    if((infeasibleSwapPos[t2] == '0')&&(feasibleSwap(t1, t2)))
                    {
                        swap(t1, t2);

                        // clean infeasibleSwapPos
                        for(auto iter = computedPos.begin(); iter != computedPos.end();)
                        {
                            infeasibleSwapPos[*iter] = '0';
                            computedPos.erase(iter++);
                        }
                        
                        break;
                    }
                }
            }
        }

        // clean infeasibleSwapPos
        for(auto iter = computedPos.begin(); iter != computedPos.end();)
        {
            infeasibleSwapPos[*iter] = '0';
            computedPos.erase(iter++);
        }
    }
}

Heuristic::Heuristic(Data data, int iR, int iILS, int pType, int pDiv)
{
    this->data = data;
    this->pType = pType;
    this->pDiv = pDiv;

    auto start = chrono::system_clock::now();

    int seed = time(NULL);
    srand(seed);

    calculateMaxWindow();
    infeasibleSwapPos.resize(this->data.getNbCars(), '0');

    for(int i = 0; i < iR; i++)
    {
        initialization();

        construction();

        vector<int> currentBestSequence = sequence;
        vector<int> currentBestUnscheduled = unscheduled;
        
        if(currentBestSequence.size() == this->data.getUpperBound())
        {
            bestSequence = currentBestSequence;
            bestUnscheduled = currentBestUnscheduled;
            break;
        }
        
        for(int j = 0; j < iILS; j++)
        {
            insertion();
            
            if(sequence.size() > currentBestSequence.size())
            {
                currentBestSequence = sequence;
                currentBestUnscheduled = unscheduled;

                if(currentBestSequence.size() == this->data.getUpperBound())
                {
                    break;
                }
            
                j = 0;
            }
            else
            {
                j++;
            }

            sequence = currentBestSequence;
            unscheduled = currentBestUnscheduled;

            perturbation();
        }

        elapsedTime = chrono::system_clock::now() - start;

        if(currentBestSequence.size() > bestSequence.size())
        {
            bestSequence = currentBestSequence;
            bestUnscheduled = currentBestUnscheduled;

            if(bestSequence.size() == this->data.getUpperBound())
            {
                break;
            }
        }

        if(elapsedTime.count() > 600)
        {
            break;
        }
    }
}

void Heuristic::output(string filePath)
{
    ifstream inputFile(filePath);

    int sumPrimal = 0;
    double sumTime = 0;
    int n = 0;
    if(inputFile)
    {
        inputFile >> sumPrimal;
        inputFile >> sumTime;
        inputFile >> n;
    }

    ofstream outputFile(filePath);
    
    outputFile << sumPrimal + bestSequence.size() << endl;
    outputFile << sumTime + elapsedTime.count() << endl;
    outputFile << n + 1 << endl;
}

int Heuristic::getSequenceSize()
{
    return bestSequence.size();
}

int Heuristic::getSequence(int t)
{
    return bestSequence[t];
}

int Heuristic::getUnscheduled(int i)
{
    return bestUnscheduled[i];
}

double Heuristic::getElapsedTime()
{
    return elapsedTime.count();
}
