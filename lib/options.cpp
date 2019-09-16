#include "options.h"

void affectOptions(Options * opt, char * name, char * value)
{
    if (strncmp(name, "-o", 2) == 0)
    {
        opt->outputFile = value;
    }
    else if (strncmp(name, "-n", 2) == 0)
    {
        opt->processCount = strtol(value, NULL, 0);   
    }
    else if (strncmp(name, "-s", 2) == 0)
    {
        opt->shellSockFile = value;
    }
};

void parseOptions(Options * opt, int count, char** args)
{
    bool handlingArg = false;
    bool handlingProcess = false;
    int i = 1;
    for (; i < count; i++)
    {
        if (handlingArg)
        {
            affectOptions(opt, args[i-1], args[i]);
            handlingArg = false;
        } 
        else if (handlingProcess) 
        {
            opt->processArgs += args[i];
            if (i != count - 1) opt->processArgs += " ";
        }
        else 
        {
            string s = args[i];
            if (s[0] == '-')
            {
                handlingArg = true;
            }
            else 
            {
                handlingProcess = true;
                opt->processName = s;
                opt->valid = true;
            }
        }
    }
}
