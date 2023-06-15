#include "Model.h"
#include "Heuristic.h"
#include <vector>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 6)
    {
        cout << "Correct usage: ./max-csp.exe <path> <iR> <ilsMult> <pType> <pDiv>" << endl;
        exit(1);
    }

    int iR = stoi(argv[2]);
    float ilsMult = stof(argv[3]);
    int pType = stoi(argv[4]);
    int pDiv = stoi(argv[5]);

    Model model(argv[1]);
    Heuristic heuristic(model.data, iR, ilsMult*model.data.getNbCars(), pType, pDiv);
    heuristic.output();

    sleep(1);

    return 0;
}
