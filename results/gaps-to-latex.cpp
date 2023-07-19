#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

using namespace std;

struct GapTime
{
    double gap;
    double time;
    int count;
    int optimalCount;
    GapTime(double gap, double time, int count, int optimalCount) :
        gap(gap), time(time), count(count), optimalCount(optimalCount) {}
};

GapTime readFile(string gapTimePath)
{
    ifstream inputFile(gapTimePath);

    if(!inputFile.is_open())
    {
        cout << "Error opening file " << gapTimePath << "/gap-time.txt for reading" << endl;
        exit(1);
    }

    double gap;
    double time;
    int count;
    int optimalCount;

    inputFile >> gap;
    inputFile >> time;
    inputFile >> count;
    inputFile >> optimalCount;

    inputFile.close();

    return GapTime(gap, time, count, optimalCount);
}

string getMethodName(string method)
{
    if(method.find("asc") != string::npos)
    {
        return "incremental";
    }
    else if(method.find("desc") != string::npos)
    {
        return "decremental";
    }
    else if(method.find("min-violations") != string::npos)
    {
        return "Bautista";
    }

    return method;
}

string initialization(string path)
{
    if(path.find("heur") != string::npos)
    {
        return "initialiazed";
    }
    else
    {
        return "uninitialized";
    }
}

int main(int argc, char** argv)
{
    vector<string> instanceSets = {
        // "real",
        "literature"
    };
    map<string, vector<string>> methods;
    methods["asc-iterative"].push_back("heuristic");
    methods["asc-iterative"].push_back("combinatorial");
    methods["binary"].push_back("heuristic-combinatorial");
    methods["binary"].push_back("combinatorial");
    methods["desc-iterative"].push_back("combinatorial/heuristic-primal");
    methods["desc-iterative"].push_back("combinatorial");
    methods["regular"].push_back("heuristic-primal");
    methods["regular"].push_back("");
    methods["sos1"].push_back("heuristic-primal");
    methods["sos1"].push_back("");
    methods["min-violations"].push_back("penalize");

    cout << "\\hline method & #optimal & avg. gap (\%) & avg. time (s) \\\\ \\hline" << endl;
    for(int i = 0; i < instanceSets.size(); i++)
    {
        for(auto iter = methods.begin(); iter != methods.end(); iter++)
        {
            cout << getMethodName(iter->first) << " \\\\" << endl;
            for(int j = 0; j < iter->second.size(); j++)
            {
                GapTime gapTime = readFile(
                    instanceSets[i] + "/" + iter->first + "/" + iter->second[j] + "/gap-time.txt"
                );
                cout << initialization(iter->second[j]) << " & " << gapTime.optimalCount << " & " <<
                    gapTime.gap*100/gapTime.count << " & " << gapTime.time/gapTime.count << " \\\\" << endl;
            }
        }
        cout << "\\hline" << endl;
    }

    return 0;
}
