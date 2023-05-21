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
    bool binarySearch = false;
    bool descIterativeSearch = false;
    bool ascIterativeSearch = false;

    for(int i = 2; i < argc; i++)
    {
        if(strcmp(argv[i], "-sos1") == 0)
        {
            sos1Branching = true;
        }
        else if(strcmp(argv[i], "-cumulative") == 0)
        {
            cumulative = true;
        }
        else if(strcmp(argv[i], "-binsearch") == 0)
        {
            if(sos1Branching)
            {
                cout << "Cannot use both -sos1 and -binsearch flags" << endl;
                exit(1);
            }

            binarySearch = true;
        }
        else if(strcmp(argv[i], "-descitsearch") == 0)
        {
            if(sos1Branching)
            {
                cout << "Cannot use both -sos1 and -descitsearch flags" << endl;
                exit(1);
            }

            if(binarySearch)
            {
                cout << "Cannot use both -binsearch and -descitsearch flags" << endl;
                exit(1);
            }
            
            descIterativeSearch = true;
        }
        else if(strcmp(argv[i], "-ascitsearch") == 0)
        {
            if(sos1Branching)
            {
                cout << "Cannot use both -sos1 and -ascitsearch flags" << endl;
                exit(1);
            }

            if(binarySearch)
            {
                cout << "Cannot use both -binsearch and -ascitsearch flags" << endl;
                exit(1);
            }

            if(descIterativeSearch)
            {
                cout << "Cannot use both -descitsearch and -ascitsearch flags" << endl;
                exit(1);
            }
            
            ascIterativeSearch = true;
        }
        else
        {
            cout << "Invalid flag: " << argv[i] << endl;
            exit(1);
        }
    }

    Model model(argv[1], sos1Branching, cumulative, binarySearch || descIterativeSearch || ascIterativeSearch);

    if(model.solve())
    {
        model.output();
    }

    return 0;
}
