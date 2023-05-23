#include "CustomAlgorithms.h"

CustomAlgorithms::CustomAlgorithms(Model* model)
{
	this->model = model;
}

int CustomAlgorithms::binarySearch(int lb, int ub)
{
	while(ub > lb)
	{
		int mid = ceil((lb + ub)/2.0);

		model->initModel(false, mid);

		if(!model->solve())
		{
			ub = mid - 1;
		}
		else
		{
			lb = mid;
		}
	}

	return lb;
}

int CustomAlgorithms::descIterativeSearch(int ub)
{
	while(ub)
	{
		model->initModel(false, ub);

		if(model->solve())
		{
			break;
		}

		ub--;
	}

	return ub;
}

int CustomAlgorithms::ascIterativeSearch(int lb)
{
	while(lb <= model->data.getNbCars())
	{
		model->initModel(false, lb + 1);

		if(!model->solve())
		{
			// store feasible solution
			model->initModel(false, lb);
			model->solve();
			break;
		}

		lb++;
	}

	return lb;
}
