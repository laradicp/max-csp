#ifndef FLAG_HANDLER_H
#define FLAG_HANDLER_H

#include <string>

using namespace std;

class FlagHandler
{
	private:
		bool branching;
		bool sos1Branching;
		bool cumulative;
		bool binarySearch;
		bool decrementalSearch;
		bool incrementalSearch;
		bool heuristic;
		bool noExact;
		bool trivialUB;
		bool trivialLB;
		bool minViolations;
		bool penalize;
		bool checker;
		bool minPaceDelay;
		int branchPriority;

		string heuristicPath;
	
	public:
		FlagHandler(int argc, char** argv);

		void checkValidFlags();

		bool getBranching();
		bool getSos1Branching();
		bool getCumulative();
		bool getBinarySearch();
		bool getDecrementalSearch();
		bool getIncrementalSearch();
		bool getHeuristic();
		bool getNoExact();
		bool getTrivialUB();
		bool getTrivialLB();
		bool getMinViolations();
		bool getPenalize();
		bool getChecker();
		bool getMinPaceDelay();
		int getBranchPriority();

		string getHeuristicPath();
		
};

#endif
