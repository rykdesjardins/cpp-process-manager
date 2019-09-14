#include "cli.h"
#include <iostream>

using namespace std;

ShellClient::ShellClient(ProcessManager *manager)
{
    this->pm = manager;
}

void ShellClient::start()
{
    string command;

    while (command != "exit")
    {
        cin >> command;

        if (command == "kill") 
        {
            this->pm->kill();
            command = "exit";
        }
    }
}
