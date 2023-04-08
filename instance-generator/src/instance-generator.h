#ifndef INSTANCE_GENERATOR_H
#define INSTANCE_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class InstanceGenerator
{
    private:

        int nbCars;
        int nbClasses;
        int nbOptions;

        vector<int> maxCarsPerWindow;
        vector<int> windowSize;

        vector<int> nbCarsPerClass;
        vector<vector<bool>> options;
    
    public:

        InstanceGenerator(string filePath);

        void defineConstants();
};

#endif
