#include "FlagHandler.h"
#include <string.h>
#include <iostream>

FlagHandler::FlagHandler(int argc, char** argv)
{
    sos1Branching = false;
    cumulative = false;
    binarySearch = false;
    descIterativeSearch = false;
    ascIterativeSearch = false;
    heuristic = false;
    noExact = false;
    trivialUB = false;
    trivialLB = false;
    
    int begin = 2;
    if((begin < argc)&&(argv[begin][0] != '-')) // the third argument is the path for the heuristic solution
    {
        heuristicPath = argv[begin];
        begin = 3;
        heuristic = true;
    }

	for(int i = begin; i < argc; i++)
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
            binarySearch = true;
        }
        else if(strcmp(argv[i], "-descitsearch") == 0)
        {
            descIterativeSearch = true;
        }
        else if(strcmp(argv[i], "-ascitsearch") == 0)
        {
            ascIterativeSearch = true;
        }
        else if(strcmp(argv[i], "-heuristic") == 0)
        {
            heuristic = true;
        }
        else if(strcmp(argv[i], "-noexact") == 0)
        {
            noExact = true;
        }
        else if(strcmp(argv[i], "-trivialub") == 0)
        {
            trivialUB = true;
        }
        else if(strcmp(argv[i], "-triviallb") == 0)
        {
            trivialLB = true;
        }
        else
        {
            cout << "Invalid flag: " << argv[i] << endl;
            exit(1);
        }
    }

    checkValidFlags();
}

void FlagHandler::checkValidFlags()
{
    if(sos1Branching)
    {
        if(binarySearch)
        {
            cout << "Cannot use both -sos1 and -binsearch flags" << endl;
            exit(1);
        }

        if(descIterativeSearch)
        {
            cout << "Cannot use both -sos1 and -descitsearch flags" << endl;
            exit(1);
        }

        if(ascIterativeSearch)
        {
            cout << "Cannot use both -sos1 and -ascitsearch flags" << endl;
            exit(1);
        }
    }

    if(binarySearch)
    {
        if(descIterativeSearch)
        {
            cout << "Cannot use both -binsearch and -descitsearch flags" << endl;
            exit(1);
        }

        if(ascIterativeSearch)
        {
            cout << "Cannot use both -binsearch and -ascitsearch flags" << endl;
            exit(1);
        }
    }

    if(descIterativeSearch&&ascIterativeSearch)
    {
        cout << "Cannot use both -descitsearch and -ascitsearch flags" << endl;
        exit(1);
    }

    if(noExact)
    {
        if(sos1Branching)
        {
            cout << "Cannot use both -noexact and -sos1 flags" << endl;
            exit(1);
        }

        if(binarySearch)
        {
            cout << "Cannot use both -noexact and -binsearch flags" << endl;
            exit(1);
        }

        if(descIterativeSearch)
        {
            cout << "Cannot use both -noexact and -descitsearch flags" << endl;
            exit(1);
        }

        if(ascIterativeSearch)
        {
            cout << "Cannot use both -noexact and -ascitsearch flags" << endl;
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

bool FlagHandler::getHeuristic()
{
    return heuristic;
}

bool FlagHandler::getNoExact()
{
    return noExact;
}

bool FlagHandler::getTrivialUB()
{
    return trivialUB;
}

bool FlagHandler::getTrivialLB()
{
    return trivialLB;
}

string FlagHandler::getHeuristicPath()
{
    return heuristicPath;
}
