#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

class Analytics
{
    public:
        Analytics(string instance);
        void readFile(string path);
        void print(string path);

    private:
        int bestDual;
        string instance;
        unordered_map<string, int> primalBounds;
        unordered_map<string, double> times;
};

Analytics::Analytics(string instance)
{
    bestDual = __INT_MAX__;
    this->instance = instance;
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
}

void Analytics::print(string path)
{
    ifstream inputFile(path + "/gap-time.txt");

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

    ofstream outputFile(path + "/gap-time.txt");

    if(!outputFile.is_open())
    {
        cout << "Error opening file " << path << "/gap-time.txt for writing" << endl;
        return;
    }

    outputFile << sumGaps + (double)(bestDual - primalBounds[path])/primalBounds[path] << endl;
    outputFile << sumTimes + times[path] << endl;
    outputFile << count + 1 << endl;

    outputFile.close();
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Usage: ./calculate-gaps <instance>" << endl;
        return 0;
    }

    vector<string> instanceSets = {"real", "literature"};
    vector<string> paths = {
        "asc-iterative/combinatorial", "asc-iterative/trivial", "asc-iterative/heuristic",
        "binary/combinatorial", "binary/combinatorial-trivial", "binary/trivial",
        "binary/trivial-combinatorial", "binary/heuristic-combinatorial", "binary/heuristic-trivial",
        "desc-iterative/combinatorial", "desc-iterative/trivial", "desc-iterative/combinatorial/heuristic-primal",
        "min-violations/penalize",
        "regular", "regular/heuristic-primal",
        "sos1", "sos1/heuristic-primal"
    };

    Analytics analytics(argv[1]);

    for(string instanceSet : instanceSets)
    {
        for(string path : paths)
        {
            // populate dicts and find best dual
            analytics.readFile(instanceSet + "/" + path);
        }

        for(string path : paths)
        {
            analytics.print(instanceSet + "/" + path);
        }
    }

    return 0;
}
