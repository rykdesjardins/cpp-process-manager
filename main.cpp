#include "lib/cli.h"
#include "lib/manager.h"
#include "lib/options.h"

int main(int argc, char *argv[]) 
{
    if (argc < 3) 
    {
        cout << "You must specify a process and args" << endl;
        return 1;
    }

    Options opt;
    parseOptions(&opt, argc, argv);
    ProcessManager * manager = new ProcessManager(&opt);
    ShellClient * cli = new ShellClient(manager);

    for (int i = 0; i < opt.processCount; i++) manager->spawn(opt.processArgs);
    cli->start();
    cli->block();

    manager->rest();

    delete cli;
    delete manager;

    log("Main", "Exiting process normally");
    return 0;
}
