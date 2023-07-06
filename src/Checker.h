#ifndef CHECKER_H
#define CHECKER_H

#include <iostream>
#include <string>
#include <vector>
#include "Data.h"

using namespace std;

class Checker
{
    private:

        Data data;

        vector<int> sequence;

    public:

        Checker(Data data);

        void readSequence(string sequencePath);
        bool isFeasible();
        
};

#endif
