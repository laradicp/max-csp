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

    do
    {
        nbCarsPerClass[idx]++;
        lastShift = shift;
        file >> idx;
        file >> shift;
    } while (lastShift == shift);
}

void InstanceGenerator::defineConstants()
{
    nbClasses = 12;
    nbOptions = 6;
    maxCarsPerWindow = {1, 3, 1, 1, 1, 2};
    windowSize = {3, 4, 2, 6, 4, 3};

    nbCarsPerClass = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
