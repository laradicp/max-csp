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

        PlotData(string instance, bool findMaxValues = false);

        void printData(string path);

    private:

        Data data;

        string instance;

        int nbOptions;
        int nbClasses;
        // utilization of option j = \mu_j(\sum_{i \in P} d_i*c_{ij})/|D|
        // length of sequence necessary to fit n cars with option j:
        //     \mu_j(n) = q_j((n − 1) div p_j) + ((n − 1) mod p_j) + 1
        double minUtilization;
        double avgUtilization;
        double stdUtilization; // standard deviation of utilization
        // standard deviation = sqrt(pow(sum_{j \in C}{(\mu_j(n_j)/|D| - avgUtilization), 2))/|C|}
        double avgNbOptionsPerClass; // avg number of options per class
        double avgMaxCarsWindowSizeRatio;
        double maxMaxCarsWindowSizeRatio;

        // for normalizing features, read in file
        int maxNbOptions;
        int maxNbClasses;
        int maxAvgNbOptionsPerClass;

        vector<double> coord;
        vector<vector<double>> weights;

        int getNbCarsWithOption(int j);
        double getUtilization(int j);

        int getNbOptionsPerClass(int i);

        double calculateGap(string path);
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
    double mu = floor(data.getWindowSize(j)*((nbCarsWithOption - 1)/data.getMaxCarsPerWindow(j))) +
        ((nbCarsWithOption - 1)%data.getMaxCarsPerWindow(j)) + 1;
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

PlotData::PlotData(string instance, bool findMaxValues)
{
    this->instance = instance;

    data = Data("../instances/" + this->instance, false);

    nbOptions = data.getNbOptions();
    nbClasses = data.getNbClasses();
    minUtilization = 1;

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

    avgMaxCarsWindowSizeRatio = 0;
    maxMaxCarsWindowSizeRatio = 0;
    for(int j = 0; j < nbOptions; j++)
    {
        double ratio = (double)data.getMaxCarsPerWindow(j)/data.getWindowSize(j);
        avgMaxCarsWindowSizeRatio += ratio;

        if(ratio > maxMaxCarsWindowSizeRatio)
        {
            maxMaxCarsWindowSizeRatio = ratio;
        }
    }
    avgMaxCarsWindowSizeRatio /= nbOptions;

    // read in file
    ifstream inputFile("results/instance-plot-data-max-values.txt");
    inputFile >> maxNbOptions >> maxNbClasses >> maxAvgNbOptionsPerClass;
    inputFile.close();

    if(findMaxValues)
    {
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

        ofstream outputFile("results/instance-plot-data-max-values.txt");
        outputFile << maxNbOptions << " " << maxNbClasses << " " << maxAvgNbOptionsPerClass << endl;

        outputFile.close();

        return;
    }

    // Sun et al. (2022)
    weights = {
        {0.46417697, -0.13786325, -0.71876354, -0.36351993, 0.22506848, -0.25113443, -0.04415605, 0.03303967},
        {0.13118315, 0.40042554, 0.03250308, -0.44927092, -0.33242684, 0.21863308, 0.59942745, 0.31926206}
    };

    coord = vector<double>(2, 0);
    // multiply normalized features by weights and add to coord
    for(int i = 0; i < 2; i++)
    {
        coord[i] = weights[i][0]*((double)nbOptions/maxNbOptions) +
            weights[i][1]*((double)nbClasses/maxNbClasses) +
            weights[i][2]*minUtilization +
            weights[i][3]*avgUtilization +
            weights[i][4]*stdUtilization +
            weights[i][5]*((double)avgNbOptionsPerClass/maxAvgNbOptionsPerClass) +
            weights[i][6]*avgMaxCarsWindowSizeRatio +
            weights[i][7]*maxMaxCarsWindowSizeRatio;
    }
}

double PlotData::calculateGap(string path)
{
    ifstream inputFile("best-dual.txt");
    string line;
    int dualBound = data.getUpperBound();
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
    if(path.find("instances") != string::npos)
    {
        inputFile.open("best-primal.txt");
        while(getline(inputFile, line))
        {
            if(line.find(path) != string::npos)
            {
                primalBound = stoi(line.substr(line.find("\t") + 1));
                break;
            }
        }
    }
    else
    {
        inputFile.open(path);

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

void PlotData::printData(string path)
{
    cout << instance << " " << coord[0] << " " << coord[1] << " " << 100*calculateGap(path) << endl;
}

string getInstanceName(string path)
{
    string instanceSet;
    if(path.find(".in") != string::npos)
    {
        instanceSet = "real";
    }
    else
    {
        instanceSet = "literature";
    }
    return instanceSet + "/" + path.substr(path.find_last_of("/") + 1);
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        cout << "Usage: ./instance-plot-data <instance> -<flags>" << endl;
        exit(1);
    }

    if((argc == 3)&&(strcmp(argv[2], "-maxvals") == 0))
    {
        PlotData plotData(getInstanceName(argv[1]), true);
        return 0;
    }

    string instance = argv[1];

    PlotData plotData(getInstanceName(instance));

    if(instance.find("instances") != string::npos)
    {
        instance = instance.substr(instance.find_first_of("/") + 1);
    }
    plotData.printData(instance);

    return 0;
}
