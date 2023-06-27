#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> searchTypes = {"asc-iterative", "desc-iterative", "binary"};
    int bestDual = __INT_MAX__;
    int bestPrimal = 0;
    for(int i = 0; i < searchTypes.size(); i++)
    {
        ifstream inputFile("../literature/" + searchTypes[i] + "/combinatorial/" +
            argv[1]);
        
        string line;
        while(getline(inputFile, line))
        {
            if(line.find("Primal:") != string::npos)
            {
                int primal = stoi(line.substr(line.find(":") + 2));
                if(primal > bestPrimal)
                {
                    bestPrimal = primal;
                }
            }
            else if(line.find("Dual:") != string::npos)
            {
                int dual = stoi(line.substr(line.find(":") + 2));
                if(dual < bestDual)
                {
                    bestDual = dual;
                }
            }
            else if(line.find("Optimal") != string::npos)
            {
                bestDual = bestPrimal;
                break;
            }
        }
    }

    ofstream outputFile("best-dual.txt", ios::app);
    outputFile << argv[1] << " " << bestDual << endl;
    outputFile.close();

    outputFile.open("best-primal.txt", ios::app);
    outputFile << argv[1] << " " << bestPrimal << endl;
    outputFile.close();
    
    return 0;
}
