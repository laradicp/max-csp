#include "FlagHandler.h"
#include <string.h>
#include <iostream>

FlagHandler::FlagHandler(int argc, char** argv)
{
    branching = false;
    sos1Branching = false;
    cumulative = false;
    binarySearch = false;
    decrementalSearch = false;
    incrementalSearch = false;
    heuristic = false;
    noExact = false;
    trivialUB = false;
    trivialLB = false;
    minViolations = false;
    penalize = false;
    checker = false;
    minPaceDelay = false;
    branchPriority = 0;
    
    int begin = 2;
    if((begin < argc)&&(argv[begin][0] != '-')) // the third argument is the path for the heuristic solution
    {
        heuristicPath = argv[begin];
        begin = 3;
        heuristic = true;
    }

	for(int i = begin; i < argc; i++)
    {
        if(strcmp(argv[i], "-branch") == 0)
        {
            branching = true;
        }
        else if(strcmp(argv[i], "-minviolations") == 0)
        {
            minViolations = true;
        }
        else if(strcmp(argv[i], "-penalize") == 0)
        {
            penalize = true;
        }
        else if(strcmp(argv[i], "-sos1") == 0)
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
        else if(strcmp(argv[i], "-decsearch") == 0)
        {
            decrementalSearch = true;
        }
        else if(strcmp(argv[i], "-incsearch") == 0)
        {
            incrementalSearch = true;
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
        else if(strcmp(argv[i], "-checker") == 0)
        {
            checker = true;
        }
        else if(strcmp(argv[i], "-minpacedelay") == 0)
        {
            minPaceDelay = true;
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
        branching = true;

        if(binarySearch)
        {
            cout << "Cannot use both -sos1 and -binsearch flags" << endl;
            exit(1);
        }

        if(decrementalSearch)
        {
            branchPriority = -1;
            decrementalSearch = false;
        }

        if(incrementalSearch)
        {
            branchPriority = 1;
            incrementalSearch = false;
        }
    }

    if(binarySearch)
    {
        if(decrementalSearch)
        {
            cout << "Cannot use both -binsearch and -decsearch flags" << endl;
            exit(1);
        }

        if(incrementalSearch)
        {
            cout << "Cannot use both -binsearch and -incsearch flags" << endl;
            exit(1);
        }
    }

    if(decrementalSearch&&incrementalSearch)
    {
        cout << "Cannot use both -decsearch and -incsearch flags" << endl;
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

        if(decrementalSearch)
        {
            cout << "Cannot use both -noexact and -decsearch flags" << endl;
            exit(1);
        }

        if(incrementalSearch)
        {
            cout << "Cannot use both -noexact and -incsearch flags" << endl;
            exit(1);
        }
    }

    if(penalize&&!minViolations)
    {
        cout << "Flag -penalize cannot be used without flag -minviolations" << endl;
        exit(1);
    }
}

bool FlagHandler::getBranching()
{
    return branching;
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

bool FlagHandler::getDecrementalSearch()
{
	return decrementalSearch;
}

bool FlagHandler::getIncrementalSearch()
{
	return incrementalSearch;
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

bool FlagHandler::getMinViolations()
{
    return minViolations;
}

bool FlagHandler::getPenalize()
{
    return penalize;
}

bool FlagHandler::getChecker()
{
    return checker;
}

bool FlagHandler::getMinPaceDelay()
{
    return minPaceDelay;
}

int FlagHandler::getBranchPriority()
{
    return branchPriority;
}
