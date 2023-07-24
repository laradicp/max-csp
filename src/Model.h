#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <string>
#include <vector>
#include <ilcplex/ilocplex.h>

#include "Data.h"

using namespace std;

class Model
{
    private:

        IloEnv env;
        IloModel model;
        IloArray<IloBoolVarArray> x;
        IloArray<IloNumVarArray> y;
        IloNumVarArray z;
        
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

        bool minViolations;
        bool penalize;
        int firstViolationPos;
        bool sos1Branching;
        IloNumArray weights;

        void definePaths(string filePath);

        void calculateOptionsIntersections();
        void calculateOptionOverlap();
        
        void setBinarySearchWeights(int lb, int ub);
        void sos1(int lb, int ub, int branchPriority);

    public:
    
        Data data;

        Model(string filePath, bool cumulative = false);

        void initModel(bool sos1Branching = false, int customSearch = 0,
            int branchPriority = 0, int lb = 1, int ub = __INT_MAX__);

        bool solve(double prevElapsedTime = 0.0, vector<int>* initialSol = nullptr);
        void output(bool toFile = false);

        int getSequenceSize();
        int getSequence(int t);
        int getUnscheduled(int i);
        IloAlgorithm::Status getStatus();

        void minViolationsModel(bool penalize = false);

        void minPaceDelay();

};

#endif
