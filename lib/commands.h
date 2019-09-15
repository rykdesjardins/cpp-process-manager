#ifndef CPP_COMMANDS_H
#define CPP_COMMANDS_H

#include "manager.h"
#include <string>
#include <vector>

using namespace std;

enum SHELL_COMMANDS 
{
    UNKNOWN_COMMAND = 0,

    START_COMMAND = 1,
    STOP_COMMAND = 2,
    RESTART_COMMAND = 3,

    INFO_COMMAND = 97,
    ECHO_COMMAND = 98,
    KILL_COMMAND = 99,
};

class ShellCommand 
{
    protected:
        vector<string> * args;

    public:
        ShellCommand(vector<string> *a) : args(a) {};
        virtual ~ShellCommand() = default;

        virtual bool execute(ProcessManager*) = 0;
        virtual bool validate() = 0;
        virtual void showUsage() = 0;
};

class ShellCommandFactory 
{
    public:
        SHELL_COMMANDS getEntryFromString(string);
        ShellCommand * fromEnum(SHELL_COMMANDS, vector<string>*);
};

#endif
