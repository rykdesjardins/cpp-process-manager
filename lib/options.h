#ifndef CPP_OPTIONS_H
#define CPP_OPTIONS_H

#include <string.h>
#include <stdlib.h>
#include <string>

using namespace std;

struct Options
{
    bool valid = false;
    string processName;
    string processArgs = "";
    string outputFile = "output.log";
    string shellSockFile = "./cli.sock";
    int processCount = 1;
};

void parseOptions(Options*, int, char**);

#endif
