#include "Model.h"
#include "Heuristic.h"
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 7)
    {
        cout << "Correct usage: ./max-csp.exe <instancePath> <outputPath> <iR> <ilsMult> <pType> <pDiv>" << endl;
        exit(1);
    }

    int iR = stoi(argv[3]);
    float ilsMult = stof(argv[4]);
    int pType = stoi(argv[5]);
    int pDiv = stoi(argv[6]);

    Model model(argv[1]);
    Heuristic heuristic(model.data, iR, ilsMult*model.data.getNbCars(), pType, pDiv);
    heuristic.output(argv[2]);

    return 0;
}
