#ifndef CPP_CLI_H
#define CPP_CLI_H

#include "manager.h"

class ShellClient
{
    ProcessManager * pm;
    public:
        ShellClient(ProcessManager*);
        void start();
};

#endif
