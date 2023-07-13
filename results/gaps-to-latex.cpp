#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct GapTime
{
    double gap;
    double time;
    int count;
    GapTime(double gap, double time, int count) : gap(gap), time(time), count(count) {}
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

    inputFile >> gap;
    inputFile >> time;
    inputFile >> count;

    inputFile.close();

    return GapTime(gap, time, count);
}

int main(int argc, char** argv)
{
    vector<string> instanceSets = {"real", "literature"};
    vector<vector<string>> instanceSubsets(instanceSets.size(), vector<string>(1, ""));
    vector<string> paths = {
        "asc-iterative/combinatorial", "asc-iterative/trivial", "asc-iterative/heuristic",
        "binary/combinatorial", "binary/combinatorial-trivial", "binary/trivial",
        "binary/trivial-combinatorial", "binary/heuristic-combinatorial", "binary/heuristic-trivial",
        "desc-iterative/combinatorial", "desc-iterative/trivial", "desc-iterative/combinatorial/heuristic-primal",
        // "min-violations/penalize",
        "regular", "regular/heuristic-primal",
        "sos1", "sos1/heuristic-primal"
    };
    instanceSubsets[1].push_back("-hard");

    cout << "\\hline algorithm & gap & time & instance set \\\\ \\hline" << endl;
    for(int i = 0; i < instanceSets.size(); i++)
    {
        for(string instanceSubset : instanceSubsets[i])
        {
            GapTime gapTime = readFile("heuristic/gap-time-" + instanceSets[i] + instanceSubset + ".txt");
            cout << "heuristic & " << gapTime.gap*100/gapTime.count << "\\\% & " <<
            gapTime.time/gapTime.count << " & " << "\\multirow{" << paths.size() + 1 <<
            "}{*}{" << instanceSets[i] + instanceSubset << "} \\\\" << endl;
        
            for(string path : paths)
            {
                gapTime = readFile(instanceSets[i] + "/" + path + "/gap-time" + instanceSubset + ".txt");
                cout << path << " & " << gapTime.gap*100/gapTime.count << "\\\% & " <<
                    gapTime.time/gapTime.count << " \\\\" << endl;
            }
            cout << "\\hline" << endl;
        }
    }

    return 0;
}
