#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <map>

using namespace std;

struct GapTime
{
    double gap;
    double time;
    double timeNotExceeded;
    int count;
    int countNotExceeded;
    int optimalCount;
    int smallestCount;
    GapTime(double gap, double time, double timeNotExceeded, int count, int countNotExceeded,
        int optimalCount, int smallestCount) :
        gap(gap), time(time), timeNotExceeded(timeNotExceeded), count(count), countNotExceeded(countNotExceeded),
        optimalCount(optimalCount), smallestCount(smallestCount) {}
};

GapTime readFile(string gapTimePath)
{
    ifstream inputFile(gapTimePath);

    if(!inputFile.is_open())
    {
        cout << "Error opening file " << gapTimePath << " for reading" << endl;
        exit(1);
    }

    double gap;
    double time;
    double timeNotExceeded;
    int count;
    int countNotExceeded;
    int optimalCount;
    int smallestCount;

    inputFile >> gap;
    inputFile >> time;
    inputFile >> timeNotExceeded;
    inputFile >> count;
    inputFile >> countNotExceeded;
    inputFile >> optimalCount;
    inputFile >> smallestCount;

    inputFile.close();

    return GapTime(gap, time, timeNotExceeded, count, countNotExceeded, optimalCount, smallestCount);
}

string getMethodName(string method)
{
    if(method.find("regular") != string::npos)
    {
        return "F1";
    }
    if(method.find("branching") != string::npos)
    {
        return "F2";
    }
    else if(method.find("sos1") != string::npos)
    {
        if(method.find("asc") != string::npos)
        {
            return "F2$_I$";
        }
        else if(method.find("desc") != string::npos)
        {
            return "F2$_D$";
        }
        else
        {
            return "F2$_B$";
        }
    }
    else if(method.find("asc") != string::npos)
    {
        return "I$_I$";
    }
    else if(method.find("desc") != string::npos)
    {
        return "I$_D$";
    }
    else
    {
        return "I$_B$";
    }

    return method;
}

bool isInitialized(string path)
{
    if(path.find("heur") != string::npos)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        cout << "Usage: ./gaps-to-latex -<initialization>" << endl;
        exit(1);
    }

    bool initialization = strcmp(argv[1], "-true") == 0 ? true : false;

    vector<string> instanceSets = {
        "real",
        "literature"
    };
    vector<string> methods = {
        "regular/heuristic-primal", "regular",
        "asc-iterative/heuristic", "asc-iterative/combinatorial",
        "desc-iterative/combinatorial/heuristic-primal", "desc-iterative/combinatorial",
        "binary/heuristic-combinatorial", "binary/combinatorial",
        "branching/heuristic-primal", "branching",
        "sos1/asc-iterative/combinatorial", "sos1/asc-iterative/heuristic",
        "sos1/desc-iterative/combinatorial/heuristic-primal", "sos1/desc-iterative/combinatorial",
        "sos1/heuristic-primal", "sos1"
    };

    for(int i = 0; i < instanceSets.size(); i++)
    {
        cout << "\\hline method & avg. gap (\\\%) & optimal (\\\%) & smallest (\\\%) & avg. time (s) & avg. time$^*$ (s) \\\\ \\hline" << endl;
        for(int j = 0; j < methods.size(); j++)
        {
            GapTime gapTime = readFile(
                instanceSets[i] + "/" + methods[j] + "/gap-time.txt"
            );

            cout.setf(ios::fixed,ios::floatfield);
            cout.precision(2);

            if(initialization == isInitialized(methods[j]))
            {
                cout << getMethodName(methods[j]) << " & " << 
                    gapTime.gap*100/gapTime.count << " & " <<
                    gapTime.optimalCount*100.0/gapTime.count << " & " <<
                    gapTime.smallestCount*100.0/gapTime.count << " & " <<
                    gapTime.time/gapTime.count << " & " <<
                    gapTime.timeNotExceeded/gapTime.countNotExceeded << " \\\\" << endl;
            }
        }
        cout << "\\hline" << endl << endl;
    }

    return 0;
}
