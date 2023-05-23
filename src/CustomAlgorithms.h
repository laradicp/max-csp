#ifndef CUSTOM_ALGORITHMS_H
#define CUSTOM_ALGORITHMS_H

#include "Model.h"

class CustomAlgorithms
{
	private:
		Model* model;

	public:
		CustomAlgorithms(Model* model);
		int binarySearch(int lb, int ub);
		int descIterativeSearch(int ub);
		int ascIterativeSearch(int lb);
};

#endif
