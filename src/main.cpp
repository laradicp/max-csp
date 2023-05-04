#include "Model.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Correct usage: ./max-csp.exe <path>" << endl;
        exit(1);
    }

    if((argc > 2)&&(strcmp(argv[2], "-sos1") == 0))
    {
        Model model(argv[1], true);

        if(model.solve())
        {
            model.output();
        }

        return 0;
    }
    else
    {
        Model model(argv[1]);

        if(model.solve())
        {
            model.output();
        }
    }
    
    return 0;
}
