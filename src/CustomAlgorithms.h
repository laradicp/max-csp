#ifndef CUSTOM_ALGORITHMS_H
#define CUSTOM_ALGORITHMS_H

#include "Model.h"
#include <vector>
#include <chrono>
#include <ilcplex/ilocplex.h>

class CustomAlgorithms
{
	private:
		Model* model;

		vector<int> sequence;
		vector<int> unscheduled;
		int primal;
		int dual;
		IloAlgorithm::Status status;
		chrono::duration<double> elapsedTime;

		void defineOutput(int lb, int ub, chrono::time_point<chrono::steady_clock> start);

	public:
		CustomAlgorithms(Model* model);
		int binarySearch(int lb, int ub);
		int descIterativeSearch(int ub);
		int ascIterativeSearch(int lb);

		void output(bool toFile = false);
};

#endif
