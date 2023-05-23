#include "Model.h"
#include "CustomAlgorithms.h"
#include "FlagHandler.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Correct usage: ./max-csp.exe <path> -<flags>" << endl;
        exit(1);
    }

    FlagHandler flags(argc, argv);
    Model model(argv[1], flags.getCumulative());
    CustomAlgorithms customAlgorithms(&model);

    if(flags.getBinarySearch())
    {
        customAlgorithms.binarySearch(1, model.data.getNbCars());
        customAlgorithms.output();
    }
    else if(flags.getDescIterativeSearch())
    {
        customAlgorithms.descIterativeSearch(model.data.getNbCars());
        customAlgorithms.output();
    }
    else if(flags.getAscIterativeSearch())
    {
        customAlgorithms.ascIterativeSearch(1);
        customAlgorithms.output();
    }
    else
    {
        model.initModel(flags.getSos1Branching());
        model.solve();
        model.output();
    }

    return 0;
}
