#ifndef INSTANCE_GENERATOR_H
#define INSTANCE_GENERATOR_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class InstanceGenerator
{
    private:

        int id;
        int nbCars;
        int nbClasses;
        int nbOptions;

        vector<int> maxCarsPerWindow;
        vector<int> windowSize;

        vector<int> nbCarsPerClass;
        vector<vector<bool>> options;
    
        void defineConstants();
        void writeInstance(string filePath);

    public:

        InstanceGenerator(string filePath);
};

#endif
