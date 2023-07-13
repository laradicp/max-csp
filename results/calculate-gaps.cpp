#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <unordered_map>

using namespace std;

class Analytics
{
    public:
        Analytics(string instance);
        void readFile(string path);
        void readHeuristicFile();
        void print(string path, string instanceSubset = "");

    private:
        int bestDual;
        string instance;
        string heuristicInstance;
        unordered_map<string, int> primalBounds;
        unordered_map<string, double> times;
};

Analytics::Analytics(string instance)
{
    bestDual = __INT_MAX__;
    this->instance = instance;
    if(instance.substr(instance.size() - 4, 4) == ".out")
    {
        heuristicInstance = instance.substr(0, instance.size() - 4) + ".in";
    }
    else
    {
        heuristicInstance = instance;
    }
}

void Analytics::readFile(string path)
{
    ifstream inputFile(path + "/" + instance);

    if(!inputFile.is_open())
    {
        cout << "Error opening file " << path << "/" << instance << endl;
        return;
    }
    
    string line;
    while(getline(inputFile, line))
    {
        if(line.find("Primal:") != string::npos ||
            line.find("Violation-free sequence bound:") != string::npos)
        {
            primalBounds[path] = stoi(line.substr(line.find(":") + 2));
        }
        else if(line.find("Dual:") != string::npos)
        {
            int dual = stoi(line.substr(line.find(":") + 2));
            if(dual < bestDual)
            {
                bestDual = dual;
            }
        }
        else if(line.find("Time:") != string::npos)
        {
            times[path] = stod(line.substr(line.find(":") + 2));
        }
    }

    if(path.find("heuristic") != string::npos)
    {
        times[path] += times["heuristic"];
    }

    inputFile.close();
}

void Analytics::readHeuristicFile()
{
    ifstream heuristicFile("heuristic/" + heuristicInstance);

    if(!heuristicFile.is_open())
    {
        cout << "Error opening file heuristic/" << instance << endl;
        return;
    }

    int heuristicPrimal;
    double heuristicTime;
    heuristicFile >> heuristicPrimal >> heuristicTime;
    primalBounds["heuristic"] = heuristicPrimal;
    times["heuristic"] = heuristicTime;

    heuristicFile.close();
}

void Analytics::print(string path, string instanceSubset)
{
    ifstream inputFile(path + "/gap-time" + instanceSubset + ".txt");

    double sumGaps = 0;
    double sumTimes = 0;
    int count = 0;

    if(inputFile.is_open())
    {
        inputFile >> sumGaps;
        inputFile >> sumTimes;
        inputFile >> count;

        inputFile.close();    
    }

    ofstream outputFile(path + "/gap-time" + instanceSubset + ".txt");

    if(!outputFile.is_open())
    {
        cout << "Error opening file " << path << "/gap-time.txt for writing" << endl;
        return;
    }

    if(double(bestDual - primalBounds[path])/primalBounds[path] < 0)
    {
        cout << "Warning: " << path << " has a negative gap in instance " << instance << endl;
        cout << primalBounds[path] << " " << bestDual << endl;
    }

    outputFile << sumGaps + (double)(bestDual - primalBounds[path])/primalBounds[path] << endl;
    outputFile << sumTimes + times[path] << endl;
    outputFile << count + 1 << endl;

    outputFile.close();
}

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        cout << "Usage: ./calculate-gaps <instance> -<instance set> (optional)-<instance subset>" << endl;
        exit(1);
    }

    string instanceSet = argv[2] + 1;
    string instanceSubset = argc == 4 ? argv[3] : "";
    vector<string> paths = {
        "asc-iterative/combinatorial", "asc-iterative/trivial", "asc-iterative/heuristic",
        "binary/combinatorial", "binary/combinatorial-trivial", "binary/trivial",
        "binary/trivial-combinatorial", "binary/heuristic-combinatorial", "binary/heuristic-trivial",
        "desc-iterative/combinatorial", "desc-iterative/trivial", "desc-iterative/combinatorial/heuristic-primal",
        // "min-violations/penalize",
        "regular", "regular/heuristic-primal",
        "sos1", "sos1/heuristic-primal"
    };

    Analytics analytics(argv[1]);

    // populate dicts and find best dual
    analytics.readHeuristicFile();
    for(string path : paths)
    {
        analytics.readFile(instanceSet + "/" + path);
    }
    
    analytics.print("heuristic", "-" + instanceSet + instanceSubset);
    for(string path : paths)
    {
        analytics.print(instanceSet + "/" + path, instanceSubset);
    }

    return 0;
}
