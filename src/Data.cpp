#include "Data.h"

Data::Data(string filePath)
{
    readInstance(filePath);
    readUnscheduled(filePath);
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
    retrieveId(filePath);
    
    if(id == 0)
    {
        return;
    }

    ifstream unscheduled;
    unscheduled.open("unscheduled/" + filePath.substr(10, 3) + "_" + to_string(id) + ".out");

    int idx, nbUnscheduledCars;
    unscheduled >> idx;
    unscheduled >> nbUnscheduledCars;
    while(!unscheduled.eof())
    {
        nbCarsPerClass[idx] += nbUnscheduledCars;

        unscheduled >> idx;
        unscheduled >> nbUnscheduledCars;
    }
}
