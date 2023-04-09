#include "Model.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Correct usage: ./max-csp.exe <path>" << endl;
        exit(1);
    }

    Model model(argv[1]);

    model.solve();
    model.output();

    return 0;
}
