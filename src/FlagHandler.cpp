#include "FlagHandler.h"
#include <string.h>
#include <iostream>

FlagHandler::FlagHandler(int argc, char** argv)
{
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
}

bool FlagHandler::getSos1Branching()
{
	return sos1Branching;
}

bool FlagHandler::getCumulative()
{
	return cumulative;
}

bool FlagHandler::getBinarySearch()
{
	return binarySearch;
}

bool FlagHandler::getDescIterativeSearch()
{
	return descIterativeSearch;
}

bool FlagHandler::getAscIterativeSearch()
{
	return ascIterativeSearch;
}
