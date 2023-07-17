#include "CustomAlgorithms.h"
#include <fstream>

CustomAlgorithms::CustomAlgorithms(Model* model)
{
	this->model = model;
	initialLB = 1;
}

void CustomAlgorithms::binarySearch(int lb, int ub, double prevElapsedTime)
{
	initialLB = lb;

	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	elapsedTime = chrono::steady_clock::now() - start;

	while((elapsedTime.count() + prevElapsedTime < 600.0)&&(ub > lb))
	{
		int mid = ceil((lb + ub)/2.0);

		model->initModel(false, mid);

		elapsedTime = chrono::steady_clock::now() - start;
		if(model->solve(elapsedTime.count() + prevElapsedTime))
		{
			lb = mid;
		}
		else if(model->getStatus() == IloAlgorithm::Status::Infeasible)
		{
			ub = mid - 1;
		}

		elapsedTime = chrono::steady_clock::now() - start;
	}

	defineOutput(lb, ub, start);
}

void CustomAlgorithms::descIterativeSearch(int ub, int lb, double prevElapsedTime)
{
	initialLB = lb;

	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	elapsedTime = chrono::steady_clock::now() - start;

	while((elapsedTime.count() + prevElapsedTime < 600.0)&&(ub > lb))
	{
		model->initModel(false, ub);

		elapsedTime = chrono::steady_clock::now() - start;
		if(model->solve(elapsedTime.count() + prevElapsedTime))
		{
			lb = ub;
			break;
		}

		if(model->getStatus() == IloAlgorithm::Status::Infeasible)
		{
			ub--;
		}

		elapsedTime = chrono::steady_clock::now() - start;
	}

	defineOutput(lb, ub, start);
}

void CustomAlgorithms::ascIterativeSearch(int lb, double prevElapsedTime)
{
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	elapsedTime = chrono::steady_clock::now() - start;

	initialLB = lb;
	
	while((elapsedTime.count() + prevElapsedTime < 600.0)&&(lb <= model->data.getUpperBound()))
	{
		defineOutput(lb, model->data.getUpperBound(), start);

		model->initModel(false, lb + 1);

		elapsedTime = chrono::steady_clock::now() - start;
		model->solve(elapsedTime.count() + prevElapsedTime);
		if(model->getStatus() == IloAlgorithm::Infeasible)
		{
			dual = lb;
			status = IloAlgorithm::Status::Optimal;
			elapsedTime = chrono::steady_clock::now() - start;
			break;
		}

		lb++;

		elapsedTime = chrono::steady_clock::now() - start;
	}
}

void CustomAlgorithms::defineOutput(int lb, int ub, chrono::time_point<chrono::steady_clock> start)
{
	elapsedTime = chrono::steady_clock::now() - start;
	status = model->getStatus();

	sequence.clear();
	unscheduled.resize(model->data.getNbClasses());
	
	if((status != IloAlgorithm::Status::Optimal)&&(status != IloAlgorithm::Status::Feasible))
	// no solution found
	{
		if(lb == ub)
		{
			status = IloAlgorithm::Status::Optimal;
		}
		else
		{
			status = IloAlgorithm::Status::Feasible;
		}
		
		if(initialLB == model->data.getLowerBound()) // combinatorial lb
		{
			for(int t = 0; t < model->data.getLowerBound(); t++)
			{
				sequence.push_back(model->data.getPrimalSol(t));
			}

			for(int i = 0; i < model->data.getNbClasses(); i++)
			{
				unscheduled[i] = model->data.getUnscheduled(i);
			}
		}
		else // trivial lb
		{
			for(int i = 0; i < model->data.getNbClasses(); i++)
			{
				unscheduled[i] = model->data.getNbCarsPerClass(i);

				if((unscheduled[i] > 0)&&sequence.empty())
				{
					sequence.push_back(i);
					unscheduled[i]--;
				}
			}
		}
	}
	else
	{
		if(lb < ub)
		{
			status = IloAlgorithm::Status::Feasible;
		}
		
		for(int t = 0; t < model->getSequenceSize(); t++)
		{
			sequence.push_back(model->getSequence(t));
		}

		for(int i = 0; i < model->data.getNbClasses(); i++)
		{
			unscheduled[i] = model->getUnscheduled(i);
		}
	}

	primal = lb;
	dual = ub;
}

void CustomAlgorithms::output(bool toFile)
{
    if(toFile)
    {
        ofstream output;

        output.open(model->data.getResultPath());

        output << "Primal:\t" << primal << endl;
        output << "Dual:\t" << dual << endl;
        output << "Status:\t" << status << endl;
        output << "Time:\t" << elapsedTime.count() << endl;
		output << "Sequence:" << endl;
        for(unsigned int t = 0; t < sequence.size(); t++)
        {
            output << "\t" << sequence[t] << endl;
        }

        output.close();
    }
    else
    {
		cout << "Primal:\t" << primal << endl;
        cout << "Dual:\t" << dual << endl;
        cout << "Status:\t" << status << endl;
        cout << "Time:\t" << elapsedTime.count() << endl;
		cout << "Sequence:" << endl;
        for(unsigned int t = 0; t < sequence.size(); t++)
        {
            cout << "\t" << sequence[t] << endl;
        }
    }

	if(model->data.isCumulative())
	{
		ofstream unscheduledOutput;

		unscheduledOutput.open(model->data.getUnscheduledPath());

        for(int i = 0; i < model->data.getNbClasses(); i++)
        {
            if(unscheduled[i] > 0)
            {
                unscheduledOutput << i << " " << unscheduled[i] << endl;
            }
        }

        unscheduledOutput.close();
	}
}
