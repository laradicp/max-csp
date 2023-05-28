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
