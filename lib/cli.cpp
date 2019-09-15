#include "cli.h"
#include "commands.h"

using namespace std;

ProcessManager* ShellClient::pm;
ShellClient::ShellClient(ProcessManager *manager)
{
    ShellClient::pm = manager;
}

void ShellClient::threadStart()
{
    ShellCommandFactory factory;
    string command;

    while (pm->isRunning())
    {
        getline(cin, command, '\n');

        vector<string> args;
        stringstream ss(command);

        string s;
        while (getline(ss, s, ' ')) if (s.size() != 0) args.push_back(s);

        if (args.size() != 0)
        {
            SHELL_COMMANDS shellcmd = factory.getEntryFromString(args.at(0));
            ShellCommand * cmd = factory.fromEnum(shellcmd, &args);
            if (cmd->validate()) cmd->execute(pm);
            else cmd->showUsage();

            delete cmd;
        }
    }
}

void killFromShell(int) 
{
    log("Shell", "Received SIGINT, requesting kill from process manager");
    ShellClient::getProcessManager()->kill();

    log("Shell", "This might require to press 'Enter' for the shell thread to exit");
    log("Shell", "If the shutdown process is stuck here, simply press 'Enter'");
}

void ShellClient::start()
{
    this->t = thread([=] { this->threadStart(); });

    signal(SIGINT, killFromShell);
}

void ShellClient::block()
{
    this->t.join();
}
