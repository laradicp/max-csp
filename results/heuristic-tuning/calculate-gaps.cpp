#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> perturbationTypes = {"removal", "swap", "both"};
    vector<string> perturbationDivisors = {"50", "25", "20", "15", "10", "5"};
    vector<vector<double>> gaps;
    vector<vector<double>> times;
    int bestDual = __INT_MAX__;

    // find best dual
    ifstream bestDualFile("best-dual.txt");
    string line;
    while(getline(bestDualFile, line))
    {
        if(line.find(argv[1]) != string::npos)
        {
            bestDual = stoi(line.substr(line.find(" ") + 1));
            break;
        }
    }
    bestDualFile.close();

    for(int i = 0; i < perturbationTypes.size(); i++)
    {
        vector<double> gapsRow;
        vector<double> timesRow;
        for(int j = 0; j < perturbationDivisors.size(); j++)
        {
            ifstream inputFile(perturbationTypes[i] + "/" + perturbationDivisors[j] + "/" + argv[1]);
            
            int sumPrimal;
            double sumTime;
            int n;
            inputFile >> sumPrimal >> sumTime >> n;
            gapsRow.push_back((bestDual - sumPrimal/(double)n)/(sumPrimal/(double)n));
            timesRow.push_back(sumTime/n);

            inputFile.close();
        }

        gaps.push_back(gapsRow);
        times.push_back(timesRow);
    }

    for(int i = 0; i < perturbationTypes.size(); i++)
    {
        for(int j = 0; j < perturbationDivisors.size(); j++)
        {
            ifstream inputFile(perturbationTypes[i] + "/" + perturbationDivisors[j] + "/gap-time.txt");

            double sumGap = 0;
            double sumTime = 0;
            int n = 0;
            if(inputFile)
            {
                inputFile >> sumGap;
                inputFile >> sumTime;
                inputFile >> n;
            }

            inputFile.close();

            ofstream outputFile(perturbationTypes[i] + "/" + perturbationDivisors[j] + "/gap-time.txt");
            
            outputFile << sumGap + gaps[i][j] << endl;
            outputFile << sumTime + times[i][j] << endl;
            outputFile << n + 1 << endl;

            outputFile.close();
        }
    }

    return 0;
}
