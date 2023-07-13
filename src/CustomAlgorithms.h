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

		int initialLB;

		void defineOutput(int lb, int ub, chrono::time_point<chrono::steady_clock> start);

	public:
		CustomAlgorithms(Model* model);
		void binarySearch(int lb, int ub, double prevElapsedTime = 0.0);
		void descIterativeSearch(int ub, int lb = 1, double prevElapsedTime = 0.0);
		void ascIterativeSearch(int lb, double prevElapsedTime = 0.0);

		void output(bool toFile = false);
};

#endif
