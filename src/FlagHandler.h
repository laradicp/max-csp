#ifndef FLAG_HANDLER_H
#define FLAG_HANDLER_H

using namespace std;

class FlagHandler
{
	private:
		bool sos1Branching;
		bool cumulative;
		bool binarySearch;
		bool descIterativeSearch;
		bool ascIterativeSearch;
	
	public:
		FlagHandler(int argc, char** argv);
		bool getSos1Branching();
		bool getCumulative();
		bool getBinarySearch();
		bool getDescIterativeSearch();
		bool getAscIterativeSearch();
};

#endif
