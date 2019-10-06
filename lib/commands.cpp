#include "commands.h"

class StartCommand : public ShellCommand 
{ 
    public:
        StartCommand(vector<string> * args) : ShellCommand(args) { }

        bool execute(ProcessManager *) 
        { 
            cout << "Not yet implemented" << endl;
            return true; 
        }

        bool validate() { return true; }
        void showUsage() {}
};

class KillCommand : public ShellCommand 
{ 
    public:
        KillCommand(vector<string> * args) : ShellCommand(args) { }

        bool execute(ProcessManager * pm)
        {
            pm->kill();
            return true;
        }

        bool validate() { return true; }
        void showUsage() {}
};

class InfoCommand : public ShellCommand
{
    public: 
        InfoCommand(vector<string> * args) : ShellCommand(args) { }

        bool execute(ProcessManager * pm)
        {
            vector<ProcessStat> stats;
            pm->getInfo(&stats);

            cout << "Server running" << endl;
            for (ProcessStat stat : stats)
            {
                cout << "[" << stat.pid << "]{" << stat.restarts << "} : " << stat.maincommand << " " << stat.args << endl;
            }

            return true;
        }

        bool validate() { return true; }
        void showUsage() {}
};

class EchoCommand : public ShellCommand
{
    public:
        EchoCommand(vector<string> * args) : ShellCommand(args) { }

        bool execute(ProcessManager *)
        {
            cout << "Echoing args : " << endl;
            for (size_t i = 1; i < args->size(); i++) 
            {
                cout << "Argument " << i << " = " << args->at(i) << endl;
            }

            return true;
        }

        bool validate() 
        { 
            return args->size() > 1; 
        }

        void showUsage() 
        {
            cout << "Usage : echo [...args]" << endl;
        }
};

class UnknownCommand : public ShellCommand
{
    public:
        UnknownCommand(vector<string> * args) : ShellCommand(args) { }

        bool execute(ProcessManager *)
        {
            cout << "Unknown command : " << args->at(0) << endl;
            return true;
        }

        bool validate() { return true; }
        void showUsage() {}
};

SHELL_COMMANDS ShellCommandFactory::getEntryFromString(string cmd)
{
    if      (cmd == "kill")     return KILL_COMMAND;
    else if (cmd == "info")     return INFO_COMMAND;
    else if (cmd == "stop")     return STOP_COMMAND;
    else if (cmd == "restart")  return RESTART_COMMAND;
    else if (cmd == "start")    return START_COMMAND;
    else if (cmd == "echo")     return ECHO_COMMAND;

    return UNKNOWN_COMMAND;
};

ShellCommand * ShellCommandFactory::fromEnum(SHELL_COMMANDS cmd, vector<string> *args)
{
    switch (cmd)
    {
        case INFO_COMMAND:  return new InfoCommand(args);
        case START_COMMAND: return new StartCommand(args);
        case KILL_COMMAND:  return new KillCommand(args);
        case ECHO_COMMAND:  return new EchoCommand(args);

        default:            return new UnknownCommand(args);
    }
};
