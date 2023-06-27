#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> perturbationTypes = {"removal", "swap", "both"};
    vector<string> perturbationDivisors = {"50", "25", "20", "15", "10", "5"};

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

            cout << "\\addplot[mark=o] coordinates{(" << sumTime/n << "," << 100*sumGap/n << ")}" <<
            "node[label={[label distance=-1mm]90:$" << i*perturbationDivisors.size() + j + 1 << "$}]{};" << endl;
        }
    }

    return 0;
}
