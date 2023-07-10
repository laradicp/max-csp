#ifndef FLAG_HANDLER_H
#define FLAG_HANDLER_H

#include <string>

using namespace std;

class FlagHandler
{
	private:
		bool sos1Branching;
		bool cumulative;
		bool binarySearch;
		bool descIterativeSearch;
		bool ascIterativeSearch;
		bool heuristic;
		bool noExact;
		bool trivialUB;
		bool trivialLB;
		bool minViolations;
		bool penalize;
		bool checker;
		bool minPaceDelay;

		string heuristicPath;
	
	public:
		FlagHandler(int argc, char** argv);

		void checkValidFlags();

		bool getSos1Branching();
		bool getCumulative();
		bool getBinarySearch();
		bool getDescIterativeSearch();
		bool getAscIterativeSearch();
		bool getHeuristic();
		bool getNoExact();
		bool getTrivialUB();
		bool getTrivialLB();
		bool getMinViolations();
		bool getPenalize();
		bool getChecker();
		bool getMinPaceDelay();

		string getHeuristicPath();
		
};

#endif
