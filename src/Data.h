#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Data
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

        void retrieveId(string filePath);
        void readInstance(string filePath);
        void readUnscheduled(string filePath);

    public:

        Data(string filePath);
        Data() = default;

        int getId();
        int getNbCars();
        int getNbClasses();
        int getNbOptions();

        int getMaxCarsPerWindow(int j);
        int getWindowSize(int j);

        int getNbCarsPerClass(int i);
        int getOption(int i, int j);
};

#endif
