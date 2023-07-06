#include "Checker.h"
#include <fstream>

Checker::Checker(Data data)
{
    this->data = data;
}

void Checker::Checker::readSequence(string sequencePath)
{
    ifstream sequenceFile(sequencePath);

    if(sequenceFile.is_open())
    {
        int nbCars;
        sequenceFile >> nbCars;

        for(int i = 0; i < nbCars; i++)
        {
            int car;
            sequenceFile >> car;
            sequence.push_back(car);
        }
    }
    else
    {
        cout << "Error opening sequence file " << sequencePath << endl;
        exit(1);
    }
}

bool Checker::isFeasible()
{
    for(int t = 1; t < sequence.size(); t++)
    {
        for(int j = 0; j < data.getNbOptions(); j++)
        {
            if(data.getOption(sequence[t], j))
            {
                int sum = 1;
                int begin = max(0, t - data.getWindowSize(j) + 1);
                for(int k = begin; k < t; k++)
                {
                    if(data.getOption(sequence[k], j))
                    {
                        sum++;
                    }
                }

                if(sum > data.getMaxCarsPerWindow(j))
				{
                    // print the subsequence in which option j is violated
                    cout << "Option " << j << " violated at position " << t << endl;
                    for(int k = begin; k <= t; k++)
                    {
                        cout << sequence[k] << " ";
                    }
                    cout << endl;

                    return false;
				}
            }
        }
    }

    return true;
}
