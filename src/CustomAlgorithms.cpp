#include "CustomAlgorithms.h"
#include <fstream>

CustomAlgorithms::CustomAlgorithms(Model* model)
{
	this->model = model;
}

int CustomAlgorithms::binarySearch(int lb, int ub)
{
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

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

	defineOutput(lb, ub, start);

	return lb;
}

int CustomAlgorithms::descIterativeSearch(int ub)
{
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

	while(ub)
	{
		model->initModel(false, ub);

		if(model->solve())
		{
			break;
		}

		ub--;
	}

	defineOutput(ub, ub, start);

	return ub;
}

int CustomAlgorithms::ascIterativeSearch(int lb)
{
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

	while(lb <= model->data.getNbCars())
	{
		model->initModel(false, lb + 1);

		if(!model->solve())
		{
			dual = lb;
			elapsedTime = chrono::steady_clock::now() - start;
			break;
		}

		lb++;
		defineOutput(lb, model->data.getNbCars(), start);
	}

	return lb;
}

void CustomAlgorithms::defineOutput(int lb, int ub, chrono::time_point<chrono::steady_clock> start)
{
	sequence.clear();

	for(int i = 0; i < model->getSequenceSize(); i++)
	{
		sequence.push_back(model->getSequence(i));
	}

	unscheduled.clear();

	for(int i = 0; i < model->getUnscheduledSize(); i++)
	{
		unscheduled.push_back(model->getUnscheduled(i));
	}

	primal = lb;
	dual = ub;
	status = model->getStatus();
	elapsedTime = chrono::steady_clock::now() - start;
}

void CustomAlgorithms::output(bool toFile)
{
    if(toFile)
    {
        ofstream output;

        output.open(model->getSequencePath());

        for(unsigned int t = 0; t < sequence.size(); t++)
        {
            output << sequence[t] << endl;
        }
        output << "Primal:\t" << primal << endl;
        output << "Dual:\t" << dual << endl;
        output << "Status:\t" << status << endl;
        output << "Time:\t" << elapsedTime.count() << endl;

        output.close();

        output.open(model->getUnscheduledPath());

        for(int i = 0; i < model->data.getNbClasses(); i++)
        {
            if(unscheduled[i] > 0)
            {
                output << i << " " << unscheduled[i] << endl;
            }
        }

        output.close();
    }
    else
    {
        for(unsigned int t = 0; t < sequence.size(); t++)
        {
            cout << sequence[t] << endl;
        }
        cout << "Primal:\t" << primal << endl;
        cout << "Dual:\t" << dual << endl;
        cout << "Status:\t" << status << endl;
        cout << "Time:\t" << elapsedTime.count() << endl;
    }
}
