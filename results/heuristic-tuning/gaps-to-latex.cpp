#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    vector<string> perturbationTypes = {"removal", "swap", "both"};
    vector<string> perturbationDivisors = {"50", "25", "20", "15", "10", "5"};

    vector<string> pDiv = {"10", "15"};
    vector<string> ir = {"5", "10", "15", "25", "50"};
    vector<string> iils = {"j-2", "j", "2j"};

    cout << "Perturbation" << endl << endl;
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

    cout << endl << "IR and IILS" << endl << endl;

    for(int i = 0; i < pDiv.size(); i++)
    {
        cout << "pDiv = " << pDiv[i] << endl;

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

                cout << "\\addplot[mark=o] coordinates{(" << sumTime/n << "," << 100*sumGap/n << ")}" <<
                "node[label={[label distance=-1mm]90:$" << j*iils.size() + k + 1 << "$}]{};" << endl;
            }
        }

        cout << endl;
    }

    return 0;
}
