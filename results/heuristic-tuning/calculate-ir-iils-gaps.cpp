#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> pDiv = {"10", "5"};
    vector<string> ir = {"5", "10", "15", "25", "50"};
    vector<string> iils = {"j-2", "j", "2j"};
    vector<vector<vector<double>>> gaps;
    vector<vector<vector<double>>> times;
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

    for(int i = 0; i < pDiv.size(); i++)
    {
        vector<vector<double>> gapsI;
        vector<vector<double>> timesI;
        for(int j = 0; j < ir.size(); j++)
        {
            vector<double> gapsRow;
            vector<double> timesRow;
            for(int k = 0; k < iils.size(); k++)
            {
                ifstream inputFile("pDiv-" + pDiv[i] + "/ir-" + ir[j] + "/" + iils[k] + "/" + argv[1]);
                
                int sumPrimal;
                double sumTime;
                int n;
                inputFile >> sumPrimal >> sumTime >> n;
                gapsRow.push_back((bestDual - sumPrimal/(double)n)/(sumPrimal/(double)n));
                timesRow.push_back(sumTime/n);

                inputFile.close();
            }

            gapsI.push_back(gapsRow);
            timesI.push_back(timesRow);
        }

        gaps.push_back(gapsI);
        times.push_back(timesI);
    }

    for(int i = 0; i < pDiv.size(); i++)
    {
        for(int j = 0; j < ir.size(); j++)
        {
            for(int k = 0; k < iils.size(); k++)
            {
                ifstream inputFile("pDiv-" + pDiv[i] + "/ir-" + ir[j] + "/" + iils[k] + "/gap-time.txt");

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

                ofstream outputFile("pDiv-" + pDiv[i] + "/ir-" + ir[j] + "/" + iils[k] + "/gap-time.txt");
                
                outputFile << sumGap + gaps[i][j][k] << endl;
                outputFile << sumTime + times[i][j][k] << endl;
                outputFile << n + 1 << endl;

                outputFile.close();
            }
        }
    }
    
    return 0;
}
