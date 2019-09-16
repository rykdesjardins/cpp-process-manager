#include <signal.h>
#include "lib/shellserver.h"
#include "lib/manager.h"
#include "lib/options.h"

ProcessManager * manager;
ShellServer * shellserver;
 
void handleSigInt(int)
{
    shellserver->stop();
}

int main(int argc, char *argv[]) 
{
    if (argc < 3) 
    {
        cout << "You must specify a process and args" << endl;
        return 1;
    }

    Options opt;
    parseOptions(&opt, argc, argv);
    manager = new ProcessManager(&opt);
    shellserver = new ShellServer(manager);

    for (int i = 0; i < opt.processCount; i++) manager->spawn(opt.processArgs);

    signal(SIGINT, handleSigInt);
    shellserver->start();
    shellserver->block();

    manager->rest();

    delete shellserver;
    delete manager;

    log("Main", "Exiting process normally");
    return 0;
}
