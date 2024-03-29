#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include "Data.h"

using namespace std;

class Heuristic
{
    private:

        Data data;
		vector<int> sequence;
		vector<int> bestSequence;
		vector<int> unscheduled;
		vector<int> bestUnscheduled;
		vector<char> infeasibleSwapPos; // char because boolean takes longer to access
		chrono::duration<double> elapsedTime;
		int maxWindow;
		
		void calculateMaxWindow();
		void initialization();

		void construction();

		bool feasibleInsertion(int t, int i);
		void insertion();

		bool feasibleRemoval(int t);
		void removalCL(int beginSearch, int endSearch,
			list<int> &feasiblePositions, int &feasiblePositionsSize);
		void removal(int t);

		bool feasibleSwap(int t1, int t2);
		void swap(int t1, int t2);

		void perturbation();

    public:

		Heuristic(Data data);

		void output(bool toFile = false);

		int getSequenceSize();
        int getSequence(int t);
        int getUnscheduled(int i);
		double getElapsedTime();
        
};

#endif
