#include "Model.h"
#include "CustomAlgorithms.h"
#include "Heuristic.h"
#include "FlagHandler.h"
#include "Checker.h"
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Correct usage: ./max-csp.exe <path> -<flags>" << endl;
        exit(1);
    }

    FlagHandler flags(argc, argv);
    Model model(argv[1], flags.getCumulative());
    CustomAlgorithms customAlgorithms(&model);

    int lb = 1;
    int ub = model.data.getNbCars();
    double elapsedTime = 0.0;
    vector<int> primalSolution;

    if(ub == 0)
    {
        cout << "No cars to schedule" << endl;
        return 0;
    }

    if(flags.getChecker())
    {
        Checker checker(model.data);
        checker.readSequence("sequence-to-check.txt");
        
        if(checker.isFeasible())
        {
            cout << "Feasible" << endl;
        }
        else
        {
            cout << "Infeasible" << endl;
        }

        return 0;
    }

    if(flags.getMinPaceDelay())
    {
        model.minPaceDelay();
        model.solve();
        model.output();
        return 0;
    }

    if(flags.getMinViolations())
    {
        model.minViolationsModel(flags.getPenalize());
        model.solve();
        model.output();
        return 0;
    }

    if(flags.getHeuristic())
    {
        if(flags.getHeuristicPath() != "")
        {
            ifstream heuristicFile(flags.getHeuristicPath());

            if(!heuristicFile.is_open())
            {
                cout << "Error opening heuristic file" << endl;
                exit(1);
            }

            heuristicFile >> lb;
            heuristicFile >> elapsedTime;
            for(int t = 0; t < lb; t++)
            {
                int car;
                heuristicFile >> car;
                primalSolution.push_back(car);
            }

            heuristicFile.close();
        }
        else
        {
            Heuristic heuristic(model.data);
            heuristic.output();
            
            lb = heuristic.getSequenceSize();
            for(int t = 0; t < lb; t++)
            {
                primalSolution.push_back(heuristic.getSequence(t));
            }
            elapsedTime = heuristic.getElapsedTime();
        }
    }
    else if(!flags.getTrivialLB())
    {
        lb = model.data.getLowerBound();
        for(int t = 0; t < lb; t++)
        {
            primalSolution.push_back(model.data.getPrimalSol(t));
        }
        elapsedTime = model.data.getElapsedTimeLB();
    }

    if(!flags.getTrivialUB())
    {
        ub = model.data.getUpperBound();
        elapsedTime += model.data.getElapsedTimeUB();
    }

    if(flags.getBinarySearch())
    {
        customAlgorithms.binarySearch(lb, ub, elapsedTime);
        customAlgorithms.output();
    }
    else if(flags.getDescIterativeSearch())
    {
        customAlgorithms.descIterativeSearch(ub, lb, elapsedTime);
        customAlgorithms.output();
    }
    else if(flags.getAscIterativeSearch())
    {
        customAlgorithms.ascIterativeSearch(lb, elapsedTime);
        customAlgorithms.output();
    }
    else if(!flags.getNoExact())
    {
        model.initModel(flags.getSos1Branching(), flags.getBranchPriority(), 0, ub);
        model.solve(elapsedTime, &primalSolution);
        model.output();
    }

    return 0;
}
