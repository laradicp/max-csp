#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <math.h>
#include "../src/Data.cpp"

using namespace std;

class PlotData
{
    public:

        PlotData(string instance, string instanceSet, bool findMaxValues = false);

        void printData();

    private:

        Data data;

        string instance;
        string instanceSet;

        int nbCars;
        int nbOptions;
        int nbClasses;
        // utilization of option j = \mu_j(\sum_{i \in P} d_i*c_{ij})/|D|
        // length of sequence necessary to fit n cars with option j:
        //     \mu_j(n) = q_j((n − 1) div p_j) + ((n − 1) mod p_j) + 1
        double minUtilization;
        double avgUtilization;
        double maxUtilization;
        double stdUtilization; // standard deviation of utilization
        // standard deviation = sqrt(pow(sum_{j \in C}{(\mu_j(n_j)/|D| - avgUtilization), 2))/|C|}
        double avgNbOptionsPerClass; // avg number of options per class
        double minMaxCarsWindowSizeRatio;
        double avgMaxCarsWindowSizeRatio;
        double maxMaxCarsWindowSizeRatio;
        double stdMaxCarsWindowSizeRatio; // standard deviation of maxCarsWindowSizeRatio
        int lcmWindowSize; // least common multiple of all window sizes
        double stdNbCarsPerClass; // standard deviation of number of cars per class

        // for normalizing features, read in file
        int maxNbCars;
        int maxNbOptions;
        int maxNbClasses;
        double maxAvgNbOptionsPerClass;
        int maxLcmWindowSize;

        int getNbCarsWithOption(int j);
        double getUtilization(int j);

        int getNbOptionsPerClass(int i);

        double calculateGap(string path);

        int lcm(int n1, int n2);
};

int PlotData::getNbCarsWithOption(int j)
{
    int nb = 0;
    for(int i = 0; i < data.getNbClasses(); i++)
    {
        if(data.getOption(i, j))
        {
            nb += data.getNbCarsPerClass(i);
        }
    }
    return nb;
}

double PlotData::getUtilization(int j)
{
    // \mu_j(n) = q_j((n − 1) div p_j) + ((n − 1) mod p_j) + 1
    int nbCarsWithOption = getNbCarsWithOption(j);
    double mu = floor(data.getWindowSize(j)*(max(nbCarsWithOption - 1, 0)/data.getMaxCarsPerWindow(j))) +
        (max(nbCarsWithOption - 1, 0)%data.getMaxCarsPerWindow(j)) + 1;
    return mu/data.getNbCars();
}

int PlotData::getNbOptionsPerClass(int i)
{
    int nb = 0;
    for(int j = 0; j < data.getNbOptions(); j++)
    {
        if(data.getOption(i, j))
        {
            nb++;
        }
    }
    return nb;
}

PlotData::PlotData(string instance, string instanceSet, bool findMaxValues)
{
    this->instance = instance;
    this->instanceSet = instanceSet;

    data = Data("../instances/" + this->instanceSet + this->instance, false);

    nbCars = data.getNbCars();
    nbOptions = data.getNbOptions();
    nbClasses = data.getNbClasses();
    minUtilization = 1;
    maxUtilization = 0;

    vector<double> utilizations;
    double sumUtilizations = 0;
    for(int j = 0; j < nbOptions; j++)
    {
        double mu = getUtilization(j);
        utilizations.push_back(mu);
        sumUtilizations += mu;
        if(mu < minUtilization)
        {
            minUtilization = mu;
        }
        if(mu > maxUtilization)
        {
            maxUtilization = mu;
        }
    }

    avgUtilization = sumUtilizations/nbOptions;

    stdUtilization = 0;
    for(int j = 0; j < nbOptions; j++)
    {
        stdUtilization += pow(utilizations[j] - avgUtilization, 2);
    }
    stdUtilization = sqrt(stdUtilization/nbOptions);

    avgNbOptionsPerClass = 0;
    for(int i = 0; i < nbClasses; i++)
    {
        avgNbOptionsPerClass += getNbOptionsPerClass(i);
    }
    avgNbOptionsPerClass /= nbClasses;

    minMaxCarsWindowSizeRatio = 1;
    avgMaxCarsWindowSizeRatio = 0;
    maxMaxCarsWindowSizeRatio = 0;
    for(int j = 0; j < nbOptions; j++)
    {
        double ratio = (double)data.getMaxCarsPerWindow(j)/data.getWindowSize(j);
        avgMaxCarsWindowSizeRatio += ratio;

        if(ratio < minMaxCarsWindowSizeRatio)
        {
            minMaxCarsWindowSizeRatio = ratio;
        }
        if(ratio > maxMaxCarsWindowSizeRatio)
        {
            maxMaxCarsWindowSizeRatio = ratio;
        }
    }
    avgMaxCarsWindowSizeRatio /= nbOptions;

    stdMaxCarsWindowSizeRatio = 0;
    for(int j = 0; j < nbOptions; j++)
    {
        stdMaxCarsWindowSizeRatio += pow((double)data.getMaxCarsPerWindow(j)/data.getWindowSize(j) - avgMaxCarsWindowSizeRatio, 2);
    }
    stdMaxCarsWindowSizeRatio = sqrt(stdMaxCarsWindowSizeRatio/nbOptions);

    lcmWindowSize = data.getWindowSize(0);
    for(int j = 1; j < nbOptions; j++)
    {
        lcmWindowSize = lcm(lcmWindowSize, data.getWindowSize(j));
    }

    double avgNbCarsPerClass = 0;
    for(int i = 0; i < nbClasses; i++)
    {
        avgNbCarsPerClass += data.getNbCarsPerClass(i);
    }
    avgNbCarsPerClass /= nbClasses;
    stdNbCarsPerClass = 0;
    for(int i = 0; i < nbClasses; i++)
    {
        stdNbCarsPerClass += pow(data.getNbCarsPerClass(i) - avgMaxCarsWindowSizeRatio, 2);
    }
    stdNbCarsPerClass = sqrt(stdNbCarsPerClass/nbClasses);

    // read in file
    maxNbCars = 0;
    maxNbOptions = 0;
    maxNbClasses = 0;
    maxAvgNbOptionsPerClass = 0;
    maxLcmWindowSize = 0;

    ifstream inputFile("isa-data-max-values.txt");
    if(inputFile.is_open())
    {
        inputFile >> maxNbCars >> maxNbOptions >> maxNbClasses >> maxAvgNbOptionsPerClass >>
            maxLcmWindowSize;
        inputFile.close();
    }

    if(findMaxValues)
    {
        if(nbCars > maxNbCars)
        {
            maxNbCars = nbCars;
        }

        if(nbOptions > maxNbOptions)
        {
            maxNbOptions = nbOptions;
        }

        if(nbClasses > maxNbClasses)
        {
            maxNbClasses = nbClasses;
        }

        if(avgNbOptionsPerClass > maxAvgNbOptionsPerClass)
        {
            maxAvgNbOptionsPerClass = avgNbOptionsPerClass;
        }

        if(lcmWindowSize > maxLcmWindowSize)
        {
            maxLcmWindowSize = lcmWindowSize;
        }

        ofstream outputFile("isa-data-max-values.txt");
        outputFile << maxNbCars << " " << maxNbOptions << " " << maxNbClasses << " " <<
            maxAvgNbOptionsPerClass << " " << maxLcmWindowSize << endl;

        outputFile.close();

        return;
    }
}

double PlotData::calculateGap(string path)
{
    ifstream inputFile("best-dual.txt");
    string line;
    int dualBound = data.getUpperBound();
    string instance = this->instance;
    if(instance.substr(instance.size() - 3, 3) == ".in")
    {
        if(path.substr(0, path.find_first_of("/")) != "heuristic")
            path = path.substr(0, path.size() - 3) + ".out";
        instance = instance.substr(0, instance.size() - 3) + ".out";
    }
    while(getline(inputFile, line))
    {
        if(line.find(instance) != string::npos)
        {
            dualBound = stoi(line.substr(line.find("\t") + 1));
            break;
        }
    }
    inputFile.close();

    int primalBound = data.getLowerBound();
    // if no specified primal bound, use best primal bound
    if(path.find("instances") != string::npos)
    {
        inputFile.open("best-primal.txt");
        while(getline(inputFile, line))
        {
            if(line.find(instance) != string::npos)
            {
                primalBound = stoi(line.substr(line.find("\t") + 1));
                break;
            }
        }
    }
    else
    {
        inputFile.open(path);
        if(!inputFile.is_open())
        {
            cout << "Error: could not open file " << path << endl;
            return 0;
        }

        if(path.substr(0, path.find_first_of("/")) == "heuristic")
        {
            inputFile >> primalBound;
        }
        else
        {
            while(getline(inputFile, line))
            {
                if(line.find("Primal:") != string::npos)
                {
                    primalBound = stoi(line.substr(line.find(":") + 2));
                    break;
                }
            }
        }
    }
    
    inputFile.close();

    return (double)(dualBound - primalBound)/primalBound;
}

int PlotData::lcm(int n1, int n2)
{
    int m = max(n1, n2);
    while(m%n1 || m%n2)
    {
        m++;
    }

    return m;
}

void PlotData::printData()
{
    cout << instance << "\t" << (double)nbCars/maxNbCars << "\t" <<
        (double)nbOptions/maxNbOptions << "\t" <<
        (double)nbClasses/maxNbClasses << "\t" << minUtilization << "\t" <<
        avgUtilization << "\t" << maxUtilization << "\t" << stdUtilization << "\t" <<
        avgNbOptionsPerClass/maxAvgNbOptionsPerClass << "\t" << minMaxCarsWindowSizeRatio << "\t" <<
        avgMaxCarsWindowSizeRatio << "\t" << maxMaxCarsWindowSizeRatio << "\t" <<
        stdMaxCarsWindowSizeRatio << "\t" << (double)lcmWindowSize/maxLcmWindowSize << "\t" <<
        stdNbCarsPerClass << "\t" <<
        calculateGap(instanceSet + "heuristic/" + instance) << "\t" <<
        calculateGap(instanceSet + "f1/" + instance) << "\t" <<
        calculateGap(instanceSet + "f1/heuristic-primal/" + instance) << "\t" <<
        calculateGap(instanceSet + "incremental/combinatorial/" + instance) << "\t" <<
        calculateGap(instanceSet + "incremental/heuristic/" + instance) << "\t" <<
        calculateGap(instanceSet + "decremental/combinatorial/" + instance) << "\t" <<
        calculateGap(instanceSet + "decremental/combinatorial/heuristic-primal/" + instance) << "\t" <<
        calculateGap(instanceSet + "binary/combinatorial/" + instance) << "\t" <<
        calculateGap(instanceSet + "binary/heuristic-combinatorial/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/heuristic-primal/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/sos1/incremental/combinatorial/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/sos1/incremental/heuristic/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/sos1/decremental/combinatorial/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/sos1/decremental/combinatorial/heuristic-primal/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/sos1/binary/" + instance) << "\t" <<
        calculateGap(instanceSet + "f2/sos1/binary/heuristic-primal/" + instance) << "\t" <<
        endl;
}

string getInstanceName(string path)
{
    return path.substr(path.find_last_of("/") + 1);
}

string getInstanceSet(string path)
{
    if(path.find(".in") != string::npos)
    {
        return "2/";
    }
    
    return "1/";
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        cout << "Usage: ./isa-data <instance> -<flags>" << endl;
        exit(1);
    }

    string instance = argv[1];

    if((argc == 3)&&(strcmp(argv[2], "-maxvals") == 0))
    {
        PlotData plotData(getInstanceName(instance), getInstanceSet(instance), true);
        return 0;
    }

    PlotData plotData(getInstanceName(instance), getInstanceSet(instance));
    plotData.printData();

    return 0;
}
