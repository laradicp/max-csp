#include "Model.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

Model::Model(string filePath)
{
    data = Data(filePath);
    model = IloModel(env);
    optionOverlap.resize(data.getNbOptions(), false);

    // avoid redundant constraints
    calculateOptionOverlap();

    for(int i = 0; i < data.getNbClasses(); i++)
    {
        unscheduled.push_back(data.getNbCarsPerClass(i));
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

    // add objective function (OF)
    IloExpr sumX(env);
    for(int i = 0; i < data.getNbClasses(); i++) 
    {
        for(int t = 0; t < data.getNbCars(); t++)
        {
            sumX += x[i][t];
        }
    }
    model.add(IloMaximize(env, sumX));

    // constraints 1: respect class demand
    for(int i = 0; i < data.getNbClasses(); i++) 
    {
        IloExpr sumX(env);
        for(int t = 0; t < data.getNbCars(); t++)
        {
            sumX += x[i][t];
        }

        IloRange r = (sumX <= data.getNbCarsPerClass(i));
        char name[100];
        sprintf(name, "CarsProduced(%d)", i);
        r.setName(name);
        model.add(r);
    }

    // constraints 2: each position is occupied by one car
    for(int t = 0; t < data.getNbCars(); t++)  
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
    
    // constraints 3: respect capacities
    for(int t = 0; t < data.getNbCars(); t++) 
    {
        for(int j = 0; j < data.getNbOptions(); j++)
        {
            if(optionOverlap[j])
            {
                continue;
            }

            IloExpr sumX(env);

            for(int i = 0; i < data.getNbClasses(); i++)
            {
                if(data.getOption(i, j))
                {
                    for(int k = 0; k < data.getWindowSize(j); k++)
                    {
                        if(t + k < data.getNbCars())
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

    // constraints 4: there should be unoccupied spaces only at the end
    for(int t = 0; t < data.getNbCars() - 1; t++) 
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
                if(data.getMaxCarsPerWindow(j2) == 1)
                {
                    if((carsPerOption[j1] - optionsIntersections[j1][j2] +
                        ceil((data.getMaxCarsPerWindow(j1))/(data.getWindowSize(j2))) <
                        data.getMaxCarsPerWindow(j1))&&
                        (optionsIntersections[j1][j2]*(data.getWindowSize(j2) - 1) >=
                        data.getMaxCarsPerWindow(j1)))
                    {
                        optionOverlap[j1] = true;
                        break;
                    }
                }
            }
        }
    }
}

bool Model::solve()
{
    IloCplex maxCSP(model);
    maxCSP.setParam(IloCplex::Param::TimeLimit, 10*60);
    maxCSP.setParam(IloCplex::Param::Threads, 1);
    maxCSP.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);
    maxCSP.exportModel("model.lp");

    try
    {
        if(maxCSP.solve())
        {
            sequence.clear();

            for(int t = 0; t < data.getNbCars(); t++)
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

            primal = maxCSP.getObjValue();
            dual = maxCSP.getBestObjValue();
            status = maxCSP.getStatus();
            
            return true;
        }
    }
    catch(IloException& e)
    {
        cout << e;
    }

    return false;
}
