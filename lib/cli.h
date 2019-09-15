#ifndef CPP_CLI_H
#define CPP_CLI_H

#include "manager.h"
#include <csignal>
#include <iostream>
#include <sstream>
#include <thread>

class ShellClient
{
    static ProcessManager * pm;
    thread t;
    void threadStart();

    public:
        ShellClient(ProcessManager*);
        static ProcessManager * getProcessManager() { return pm; };
        void start();
        void block();
};

#endif
