#include "instance-generator.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "Correct usage: ./instance-generator.exe <path>" << endl;
        exit(1);
    }

    InstanceGenerator generator(argv[1]);

    return 0;
}
