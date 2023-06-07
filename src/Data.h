#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

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

        int ub;
        chrono::duration<double> elapsedTimeUB;

        vector<int> lb;
        vector<int> primalSol;
        vector<int> unscheduled;
        chrono::duration<double> elapsedTimeLB;

        void retrieveId(string filePath);
        void readInstance(string filePath);
        void readUnscheduled(string filePath);

        void definePaths(string filePath);

        int used(int r, int s, vector<double> &score, vector<int> &nbCarsPerScore,
            vector<vector<int>> &classesPerScore, vector<vector<int>> &intersection);

        void calculateLB(int s, vector<double> &score, vector<int> &nbCarsPerScore,
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

        int getUpperBound();
        double getElapsedTimeUB();

        int getLowerBound();
        int getPrimalSol(int t);
        double getElapsedTimeLB();
        
};

#endif
