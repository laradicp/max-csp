#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> instanceSets = {"real", "literature"};
    vector<string> paths = {
        "asc-iterative/combinatorial", "asc-iterative/trivial", "asc-iterative/heuristic",
        "binary/combinatorial", "binary/combinatorial-trivial", "binary/trivial",
        "binary/trivial-combinatorial", "binary/heuristic-combinatorial", "binary/heuristic-trivial",
        "desc-iterative/combinatorial", "desc-iterative/trivial", "desc-iterative/combinatorial/heuristic-primal",
        // "min-violations/penalize",
        "regular", "regular/heuristic-primal",
        "sos1", "sos1/heuristic-primal"
    };

    for(string instanceSet : instanceSets)
    {
        cout << instanceSet << endl;
        for(string path : paths)
        {
            ifstream inputFile(instanceSet + "/" + path + "/gap-time.txt");

            if(!inputFile.is_open())
            {
                cout << "Error opening file " << instanceSet << "/" << path << "/gap-time.txt for reading" << endl;
                exit(1);
            }

            double gap;
            double time;
            int count;

            inputFile >> gap;
            inputFile >> time;
            inputFile >> count;

            inputFile.close();

            cout << path << " & " << gap*100/count << "\\\% & " << time/count << " \\\\" << endl;
        }
    }

    return 0;
}
