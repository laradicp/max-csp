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
        void readInstance(string filePath);
        void writeInstance(string filePath);
        void condWriteInstance(string filePath);
        void realDemands(string filePath);
        void literatureInstances(string filePath);

    public:

        InstanceGenerator(string filePath, bool literature = false);
};

#endif
