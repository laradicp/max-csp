#include "InstanceGenerator.h"
#include <string.h>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Correct usage: ./instance-generator.exe <path>" << endl;
        exit(1);
    }

    if((argc > 2)&&(strcmp(argv[2], "-lit") == 0))
    {
        InstanceGenerator instanceGenerator(argv[1], true);
    }
    else
    {
        InstanceGenerator instanceGenerator(argv[1]);
    }

    return 0;
}
