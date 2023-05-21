#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <string>
#include <ilcplex/ilocplex.h>

#include "Data.h"

using namespace std;

class Model
{
    private:

        Data data;
        IloEnv env;
        IloModel model;
        IloArray<IloBoolVarArray> x;

        string sequencePath;
        string unscheduledPath;
        
        vector<int> carsPerOption;
        vector<vector<int>> optionsIntersections;
        vector<bool> optionOverlap;

        int nbPositions;

        vector<int> sequence;
        vector<int> unscheduled;
        IloNum primal;
        IloNum dual;
        IloAlgorithm::Status status;
        IloNum elapsedTime;

        void definePaths(string filePath);

        void calculateOptionsIntersections();
        void calculateOptionOverlap();

        void sos1();

    public:

        Model(
            string filePath,
            bool sos1Branching = false,
            bool cumulative = false,
            int customSearch = 0
        );

        bool solve();
        void output();
};

#endif
