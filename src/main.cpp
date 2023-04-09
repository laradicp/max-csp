#include "Data.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Correct usage: ./max-csp.exe <path>" << endl;
        exit(1);
    }

    Data data(argv[1]);

    return 0;
}
