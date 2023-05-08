#include "InstanceGenerator.h"

#include <fstream>

InstanceGenerator::InstanceGenerator(string filePath, bool literature)
{
    if(literature)
    {
        literatureInstances(filePath);
    }
    else
    {
        realDemands(filePath);
    }
}

void InstanceGenerator::realDemands(string filePath)
{
    defineConstants();

    ifstream file;
    file.open(filePath);

    int idx;
    char shift, lastShift;
    file >> idx;
    file >> shift;

    while(!file.eof())
    {
        nbCars = 0;
        nbCarsPerClass = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        // for each shift, store data
        do
        {
            nbCarsPerClass[idx]++;
            nbCars++;

            lastShift = shift;
            file >> idx;
            file >> shift;
        } while ((lastShift == shift)&&(!file.eof()));

        writeInstance("../instances/" + filePath.substr(filePath.size() - 7, 3) +
        "_" + to_string(id++) + "_" + to_string(nbCars) + ".in");
    }

    file.close();
}

void InstanceGenerator::literatureInstances(string filePath)
{
    readInstance(filePath);
    condWriteInstance("../instances/sun_" + to_string(id) + "_" + to_string(nbCars) + ".in");
}

void InstanceGenerator::defineConstants()
{
    id = 0;

    nbClasses = 12;
    nbOptions = 6;
    maxCarsPerWindow = {1, 3, 1, 1, 1, 2};
    windowSize = {3, 4, 2, 6, 4, 3};

    options = {
        {1, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 1, 1, 0, 0, 1},
        {0, 1, 1, 0, 1, 1},
        {0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0}
    };
}

void InstanceGenerator::readInstance(string filePath)
{
    ifstream idFile;
    idFile.open("id.txt");
    idFile >> id;
    idFile.close();

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

void InstanceGenerator::writeInstance(string filePath)
{
    ofstream instance;

    instance.open(filePath);
        
    instance << nbCars << " " << nbOptions << " " << nbClasses << endl;

    for(int i = 0; i < nbOptions - 1; i++)
    {
        instance << maxCarsPerWindow[i] << " ";
    }
    instance << maxCarsPerWindow[nbOptions - 1] << endl;

    for(int i = 0; i < nbOptions - 1; i++)
    {
        instance << windowSize[i] << " ";
    }
    instance << windowSize[nbOptions - 1] << endl;

    for(int i = 0; i < nbClasses; i++)
    {
        instance << i << " " << nbCarsPerClass[i] << " ";

        for(int j = 0; j < nbOptions - 1; j++)
        {
            instance << options[i][j] << " ";
        }
        instance << options[i][nbOptions - 1] << endl;
    }

    instance.close();
}

void InstanceGenerator::condWriteInstance(string filePath)
{
    for(int i = 0; i < nbOptions; i++)
    {
        if((maxCarsPerWindow[i] > 1)&&(windowSize[i] != maxCarsPerWindow[i] + 1))
        {
            return;
        }
    }

    writeInstance(filePath);

    ofstream idFile;
    idFile.open("id.txt");
    idFile << id + 1;
    idFile.close();
}
