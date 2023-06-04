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

        string sequencePath;
        string unscheduledPath;

        vector<int> maxCarsPerWindow;
        vector<int> windowSize;

        vector<int> nbCarsPerClass;
        vector<vector<bool>> options;

        vector<int> primalSol;
        vector<int> unscheduled;

        void retrieveId(string filePath);
        void readInstance(string filePath);
        void readUnscheduled(string filePath);

        void definePaths(string filePath);

        int Data::used(int r, int s, vector<double> &score, vector<int> &nbCarsPerScore,
            vector<vector<int>> &classesPerScore, vector<vector<int>> &intersection);

        void Data::calculateLB(int s, vector<double> &score, vector<int> &nbCarsPerScore,
            vector<vector<int>> &classesPerScore, vector<vector<int>> &intersection);

    public:

        Data(string filePath, bool cumulative);
        Data() = default;

        int getId();
        int getNbCars();
        int getNbClasses();
        int getNbOptions();

        int getMaxCarsPerWindow(int j);
        int getWindowSize(int j);

        int getNbCarsPerClass(int i);
        bool getOption(int i, int j);

        string getSequencePath();
        string getUnscheduledPath();

        int Data::getUpperBound();
        int Data::getLowerBound();
        int Data::getLowerBoundSol(int t);
        
};

#endif
