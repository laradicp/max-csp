#include "instance-generator.h"

InstanceGenerator::InstanceGenerator(string filePath)
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

void InstanceGenerator::writeInstance(string filePath)
{
    ofstream instance;

    instance.open(filePath);
        
    instance << nbCars << " " << nbOptions << " " << nbClasses << endl;

    for(unsigned int i = 0; i < maxCarsPerWindow.size() - 1; i++)
    {
        instance << maxCarsPerWindow[i] << " ";
    }
    instance << maxCarsPerWindow[maxCarsPerWindow.size() - 1] << endl;

    for(unsigned int i = 0; i < windowSize.size() - 1; i++)
    {
        instance << windowSize[i] << " ";
    }
    instance << windowSize[windowSize.size() - 1] << endl;

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
