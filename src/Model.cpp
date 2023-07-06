#include "Model.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

Model::Model(string filePath, bool cumulative)
{
    data = Data(filePath, cumulative);
    minViolations = false;
    penalize = false;
    firstViolationPos = data.getNbCars();
}

void Model::initModel(bool sos1Branching, int customSearch)
{
    env.end();
    env = IloEnv();

    minViolations = false;

    model = IloModel(env);
    // optionOverlap.resize(data.getNbOptions(), false);

    // avoid redundant constraints
    // calculateOptionOverlap();

    // the customSearch sets the number of positions to be evaluated as feasible or not for scheduling
    if(customSearch > 0)
    {
        nbPositions = customSearch;
    }
    else
    {
        nbPositions = data.getNbCars();
    }

    // let Xit assume value 1 if any car of class i is assigned to position t, and 0 otherwise
    x = IloArray<IloBoolVarArray>(env, data.getNbClasses());
    for(int i = 0; i < data.getNbClasses(); i++)
    {
        IloBoolVarArray v(env, nbPositions);
        x[i] = v;
    }

    // add variable x to the model
    for(int i = 0; i < data.getNbClasses(); i++)
    {
        for(int t = 0; t < nbPositions; t++)
        {
            char name[100];
            sprintf(name, "X(%d,%d)", i, t);
            x[i][t].setName(name);
            model.add(x[i][t]);
        }
    }

    if(sos1Branching)
    {
        sos1();
    }
    else
    {
        if(customSearch > 0)
        {
            // constraints 2: each position is occupied by exactly one car
            for(int t = 0; t < nbPositions; t++)  
            {
                IloExpr sumX(env);
                for(int i = 0; i < data.getNbClasses(); i++)
                {
                    sumX += x[i][t];
                }

                IloRange r = (sumX == 1);
                char name[100];
                sprintf(name, "PositionOccupied(%d)", t);
                r.setName(name);
                model.add(r);
            }
        }
        else
        {
            // add objective function (OF)
            IloExpr sumX(env);
            for(int i = 0; i < data.getNbClasses(); i++) 
            {
                for(int t = 0; t < nbPositions; t++)
                {
                    sumX += x[i][t];
                }
            }
            model.add(IloMaximize(env, sumX));

            // constraints 2: each position is occupied by at most one car
            for(int t = 0; t < nbPositions; t++)  
            {
                IloExpr sumX(env);
                for(int i = 0; i < data.getNbClasses(); i++)
                {
                    sumX += x[i][t];
                }

                IloRange r = (sumX <= 1);
                char name[100];
                sprintf(name, "PositionOccupied(%d)", t);
                r.setName(name);
                model.add(r);
            }

            // constraints 4: there should be unoccupied spaces only at the end
            for(int t = 0; t < nbPositions - 1; t++) 
            {
                IloExpr sumX1(env);
                IloExpr sumX2(env);
                for(int i = 0; i < data.getNbClasses(); i++)
                {
                    sumX1 += x[i][t];
                    sumX2 += x[i][t + 1];
                }

                IloRange r = (sumX2 - sumX1 <= 0);
                char name[100];
                sprintf(name, "UnoccupiedEnd(%d)", t);
                r.setName(name);
                model.add(r);
            }
        }
    }
    
    // constraints 1: respect class demand
    for(int i = 0; i < data.getNbClasses(); i++) 
    {
        IloExpr sumX(env);
        for(int t = 0; t < nbPositions; t++)
        {
            sumX += x[i][t];
        }

        IloRange r = (sumX <= data.getNbCarsPerClass(i));
        char name[100];
        sprintf(name, "CarsProduced(%d)", i);
        r.setName(name);
        model.add(r);
    }
    
    // constraints 3: respect capacities
    for(int t = 0; t < nbPositions; t++) 
    {
        for(int j = 0; j < data.getNbOptions(); j++)
        {
            // if(optionOverlap[j])
            // {
            //     continue;
            // }

            IloExpr sumX(env);

            for(int i = 0; i < data.getNbClasses(); i++)
            {
                if(data.getOption(i, j))
                {
                    for(int k = 0; k < data.getWindowSize(j); k++)
                    {
                        if(t + k < nbPositions)
                        {
                            sumX += x[i][t + k];
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            IloRange r = (sumX <= data.getMaxCarsPerWindow(j));
            char name[100];
            sprintf(name, "Capacity(%d,%d)", t, j);
            r.setName(name);
            model.add(r);
        }
    }
}

void Model::calculateOptionsIntersections()
{
    carsPerOption.resize(data.getNbOptions(), 0);
    optionsIntersections.resize(data.getNbOptions(), vector<int>(data.getNbOptions(), 0));
    for(int i = 0; i < data.getNbClasses(); i++)
    {
        for(int j1 = 0; j1 < data.getNbOptions(); j1++)
        {
            if(data.getOption(i, j1))
            {
                carsPerOption[j1] += data.getNbCarsPerClass(i);
                for(int j2 = 0; j2 < data.getNbOptions(); j2++)
                {
                    if(data.getOption(i, j2))
                    {
                        optionsIntersections[j1][j2] += data.getNbCarsPerClass(i);
                    }
                }
            }
        }
    }
}

void Model::calculateOptionOverlap()
{
    calculateOptionsIntersections();

    for(int j1 = 0; j1 < data.getNbOptions(); j1++)
    {
        if(data.getMaxCarsPerWindow(j1) == data.getWindowSize(j1) - 1)
        {
            for(int j2 = 0; j2 < data.getNbOptions(); j2++)
            {
                if((j1 != j2)&&(data.getMaxCarsPerWindow(j2) == 1))
                {
                    if((carsPerOption[j1] - optionsIntersections[j1][j2] +
                        ceil((data.getMaxCarsPerWindow(j1))/(data.getWindowSize(j2))) <
                        data.getMaxCarsPerWindow(j1))&&
                        (optionsIntersections[j1][j2]*(data.getWindowSize(j2) -
                        data.getMaxCarsPerWindow(j2)) >= data.getMaxCarsPerWindow(j1)))
                    {
                        optionOverlap[j1] = true;
                        break;
                    }
                }
            }
        }
    }
}

void Model::sos1()
{
    // let Zt assume value 1 if t is the first empty position in the sequence, and 0 otherwise
    IloBoolVarArray z(env, data.getNbCars() + 1);

    // add variable z to the model
    for(int t = 1; t < data.getNbCars() + 1; t++)
    {
        char name[100];
        sprintf(name, "Z(%d)", t);
        z[t].setName(name);
        model.add(z[t]);
    }

    // add objective function (OF)
    IloExpr sumZ(env);
    for(int t = 1; t < data.getNbCars() + 1; t++) 
    {
        sumZ += t*z[t];
    }
    model.add(IloMaximize(env, sumZ));

    // constraints 2: each position is occupied by at most one car,
    //  and z defines the first empty position
    for(int t = 0; t < data.getNbCars(); t++)  
    {
        IloExpr sumX(env);
        for(int i = 0; i < data.getNbClasses(); i++)
        {
            sumX += x[i][t];
        }

        IloExpr sumZ(env);
        for(int t2 = 1; t2 <= t; t2++)
        {
            sumZ += z[t2];
        }

        IloRange r = (sumX + sumZ == 1);
        char name[100];
        sprintf(name, "PositionOccupied(%d)", t);
        r.setName(name);
        model.add(r);
    }

    // sos1
    sumZ.clear();
    for(int t = 1; t < data.getNbCars() + 1; t++)  
    {
        sumZ += z[t];
    }

    IloRange r = (sumZ == 1);
    char name[100];
    sprintf(name, "SOS1");
    r.setName(name);
    model.add(r);
}

bool Model::solve(double prevElapsedTime, vector<int>* initialSol)
{
    IloCplex maxCSP(model);
    maxCSP.setParam(IloCplex::Param::TimeLimit, 600.0 - prevElapsedTime);
    maxCSP.setParam(IloCplex::Param::Threads, 1);
    maxCSP.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);

    if(initialSol != nullptr)
    {
        // set initial solution
        IloNumVarArray startVar(env);
        IloNumArray startVal(env);
        for(unsigned int t = 0; t < initialSol->size(); t++)
        {
            for(int i = 0; i < data.getNbClasses(); i++)
            {
                startVar.add(x[i][t]);
                startVal.add(initialSol->at(t) == i);
            }
        }

        for(int t = initialSol->size(); t < nbPositions; t++)
        {
            for(int i = 0; i < data.getNbClasses(); i++)
            {
                startVar.add(x[i][t]);
                startVal.add(0);
            }
        }
        
        maxCSP.addMIPStart(startVar, startVal);
        startVal.end();
        startVar.end();
    }

    maxCSP.exportModel("model.lp");

    try
    {
        if(maxCSP.solve())
        {
            sequence.clear();
            unscheduled.resize(data.getNbClasses());
            for(int i = 0; i < data.getNbClasses(); i++)
            {
                unscheduled[i] = data.getNbCarsPerClass(i);
            }

            for(int t = 0; t < nbPositions; t++)
            {
                for(int i = 0; i < data.getNbClasses(); i++)
                {
                    if(maxCSP.getValue(x[i][t]) > 0.5)
                    {
                        unscheduled[i]--;
                        sequence.push_back(i);
                        break;
                    }
                }
            }

            if(minViolations)
            {
                firstViolationPos = nbPositions;

                for(int j = 0; j < data.getNbOptions(); j++)
                {
                    for(int t = 0; t < nbPositions; t++)
                    {
                        if(maxCSP.getValue(y[j][t]) > 0.5)
                        {
                            if(firstViolationPos > t)
                            {
                                firstViolationPos = t;
                            }

                            break;
                        }
                    }
                }

                primal = (int)maxCSP.getObjValue();
                dual = (int)maxCSP.getBestObjValue();
            }
            else
            {
                primal = maxCSP.getObjValue();
                dual = maxCSP.getBestObjValue();
            }

            status = maxCSP.getStatus();
            elapsedTime = maxCSP.getTime();

            return true;
        }
    }
    catch(IloException& e)
    {
        cout << e;
    }

    return false;
}

void Model::output(bool toFile)
{
    if(toFile)
    {
        ofstream output;

        output.open(data.getResultPath());

        if(minViolations)
        {
            if(penalize)
            {
                output << "Violation-free sequence bound:\t" << firstViolationPos << endl;
                output << "Unscheduled:\t" << data.getNbCars() - firstViolationPos << endl;
            }
            else
            {
                output << "Primal number of violations:\t" << primal << endl;
                output << "Dual number of violations:\t" << dual << endl;
            }
        }
        else
        {
            output << "Primal:\t" << primal << endl;
            output << "Dual:\t" << dual << endl;
        }
        
        output << "Status:\t" << status << endl;
        output << "Time:\t" << elapsedTime << endl;
        output << "Sequence:" << endl;
        for(unsigned int t = 0; t < sequence.size(); t++)
        {
            output << "\t" << sequence[t] << endl;
        }

        if(minViolations&&penalize)
        {
            output << "Violation-free sequence:" << endl;
            for(int t = 0; t < firstViolationPos; t++)
            {
                output << "\t" << sequence[t] << endl;
            }
        }

        output.close();
    }
    else
    {
        if(minViolations)
        {
            if(penalize)
            {
                cout << "Violation-free sequence bound:\t" << firstViolationPos << endl;
                cout << "Unscheduled:\t" << data.getNbCars() - firstViolationPos << endl;
            }
            else
            {
                cout << "Primal number of violations:\t" << primal << endl;
                cout << "Dual number of violations:\t" << dual << endl;
            }
        }
        else
        {
            cout << "Primal:\t" << primal << endl;
            cout << "Dual:\t" << dual << endl;
        }

        cout << "Status:\t" << status << endl;
        cout << "Time:\t" << elapsedTime << endl;
        cout << "Sequence:" << endl;
        for(unsigned int t = 0; t < sequence.size(); t++)
        {
            cout << "\t" << sequence[t] << endl;
        }

        if(minViolations&&penalize)
        {
            cout << "Violation-free sequence:" << endl;
            for(int t = 0; t < firstViolationPos; t++)
            {
                cout << "\t" << sequence[t] << endl;
            }
        }
    }

    if(data.isCumulative())
    {
        ofstream unscheduledOutput;
        unscheduledOutput.open(data.getUnscheduledPath());

        for(int i = 0; i < data.getNbClasses(); i++)
        {
            if(unscheduled[i] > 0)
            {
                unscheduledOutput << i << " " << unscheduled[i] << endl;
            }
        }

        unscheduledOutput.close();
    }
}

int Model::getSequenceSize()
{
    return sequence.size();
}

int Model::getSequence(int t)
{
    return sequence[t];
}

int Model::getUnscheduled(int i)
{
    return unscheduled[i];
}

IloAlgorithm::Status Model::getStatus()
{
    return status;
}

void Model::minViolationsModel(bool penalize)
{
    env.end();
    env = IloEnv();

    minViolations = true;
    this->penalize = penalize;
    nbPositions = data.getNbCars();
    firstViolationPos = data.getNbCars();

    model = IloModel(env);
    vector<double> alpha(data.getNbCars(), 1.0); // weights for penalization
    // double because alpha increases exponentially

    if(this->penalize)
    {
        int sumOptionsInfeasPos = 0;
        for(int j = 0; j < data.getNbOptions(); j++)
        {
            sumOptionsInfeasPos += data.getWindowSize(j) - data.getMaxCarsPerWindow(j);
        }

        double sumAlphas = 1.0;
        for(int t = data.getNbCars() - 2; t >=0; t--)
        {
            alpha[t] = 1.0 + sumAlphas*sumOptionsInfeasPos;
            sumAlphas += alpha[t];
        }
    }
    
    // let Xit assume value 1 if any car of class i is assigned to position t, and 0 otherwise
    x = IloArray<IloBoolVarArray>(env, data.getNbClasses());
    for(int i = 0; i < data.getNbClasses(); i++)
    {
        IloBoolVarArray v(env, data.getNbCars());
        x[i] = v;
    }

    // add variable x to the model
    for(int i = 0; i < data.getNbClasses(); i++)
    {
        for(int t = 0; t < data.getNbCars(); t++)
        {
            char name[100];
            sprintf(name, "X(%d,%d)", i, t);
            x[i][t].setName(name);
            model.add(x[i][t]);
        }
    }

    // let Zjt be the positive difference between the maximum number of times that option j may
    // appear and the number of times that it does appear in the segment ending at position t
    IloArray<IloNumVarArray> z(env, data.getNbOptions());
    for(int j = 0; j < data.getNbOptions(); j++)
    {
        IloNumVarArray v(env, data.getNbCars(), 0, data.getMaxCarsPerWindow(j), ILOINT);
        z[j] = v;
    }

    // add variable z to the model
    for(int j = 0; j < data.getNbOptions(); j++)
    {
        for(int t = 0; t < data.getNbCars(); t++)
        {
            char name[100];
            sprintf(name, "Z(%d,%d)", j, t);
            z[j][t].setName(name);
            model.add(z[j][t]);
        }
    }

    // let Yjt be the positive difference between the number of times that option j appears and the
    // maximum number of times that it may appear in the segment ending at position t
    y = IloArray<IloNumVarArray>(env, data.getNbOptions());
    for(int j = 0; j < data.getNbOptions(); j++)
    {
        IloNumVarArray v(env, data.getNbCars(), 0, data.getWindowSize(j) - data.getMaxCarsPerWindow(j), ILOINT);
        y[j] = v;
    }

    // add variable y to the model
    for(int j = 0; j < data.getNbOptions(); j++)
    {
        for(int t = 0; t < data.getNbCars(); t++)
        {
            char name[100];
            sprintf(name, "Y(%d,%d)", j, t);
            y[j][t].setName(name);
            model.add(y[j][t]);
        }
    }

    // add objective function (OF)
    IloExpr sumY(env);
    for(int j = 0; j < data.getNbOptions(); j++) 
    {
        for(int t = 0; t < data.getNbCars(); t++)
        {
            sumY += alpha[t]*y[j][t];
        }
    }
    model.add(IloMinimize(env, sumY));

    // each position is occupied by exactly one car
    for(int t = 0; t < data.getNbCars(); t++)  
    {
        IloExpr sumX(env);
        for(int i = 0; i < data.getNbClasses(); i++)
        {
            sumX += x[i][t];
        }

        IloRange r = (sumX == 1);
        char name[100];
        sprintf(name, "PositionOccupied(%d)", t);
        r.setName(name);
        model.add(r);
    }

    // respect class demand
    for(int i = 0; i < data.getNbClasses(); i++) 
    {
        IloExpr sumX(env);
        for(int t = 0; t < data.getNbCars(); t++)
        {
            sumX += x[i][t];
        }

        IloRange r = (sumX == data.getNbCarsPerClass(i));
        char name[100];
        sprintf(name, "CarsProduced(%d)", i);
        r.setName(name);
        model.add(r);
    }

    // link x to y and z
    for(int t = 0; t < data.getNbCars(); t++) 
    {
        for(int j = 0; j < data.getNbOptions(); j++)
        {
            IloExpr sumX(env);

            for(int i = 0; i < data.getNbClasses(); i++)
            {
                if(data.getOption(i, j))
                {
                    for(int k = 0; k < data.getWindowSize(j); k++)
                    {
                        if(t - k >= 0)
                        {
                            sumX += x[i][t - k];
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            int maxCarsInSegment = min(data.getMaxCarsPerWindow(j), t + 1);
                // t + 1 because it is the number of positions until t
            IloRange r = (z[j][t] - y[j][t] + sumX == maxCarsInSegment);
            char name[100];
            sprintf(name, "DefineYZ(%d,%d)", t, j);
            r.setName(name);
            model.add(r);
        }
    }
}
