#include "Model.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Correct usage: ./max-csp.exe <path> -<flags>" << endl;
        exit(1);
    }

    bool sos1Branching = false;
    bool cumulative = false;

    for(int i = 2; i < argc; i++)
    {
        if(strcmp(argv[2], "-sos1") == 0)
        {
            sos1Branching = true;
        }
        else if(strcmp(argv[2], "-cumulative") == 0)
        {
            cumulative = true;
        }
    }

    Model model(argv[1], sos1Branching, cumulative);

    if(model.solve())
    {
        model.output();
    }

    return 0;
}
