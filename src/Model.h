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
        
        vector<int> carsPerOption;
        vector<vector<int>> optionsIntersections;
        vector<bool> optionOverlap;

        vector<int> sequence;
        vector<int> unscheduled;
        IloNum primal;
        IloNum dual;
        IloAlgorithm::Status status;

        void calculateOptionsIntersections();
        void calculateOptionOverlap();

    public:

        Model(string filePath);

        bool solve();
};

#endif
