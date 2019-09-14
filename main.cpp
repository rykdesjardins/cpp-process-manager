#include "lib/cli.h"
#include "lib/manager.h"

int main(int argc, char *argv[]) 
{
    if (argc < 3) 
    {
        cout << "You must specify a process and args" << endl;
        return 1;
    }

    ProcessManager * manager = new ProcessManager(argv[1]);
    ShellClient * cli = new ShellClient(manager);

    manager->spawn(argv[2]);
    cli->start();

    manager->rest();

    delete cli;
    delete manager;

    return 0;
}
