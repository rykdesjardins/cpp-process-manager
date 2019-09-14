#ifndef CPP_MANAGER_H
#define CPP_MANAGER_H

#include "log.h"
#include <vector>
#include <signal.h>
#include <thread>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <condition_variable>
#include <sys/wait.h>
#include <mutex>

using namespace std;

class ChildProcess
{
    condition_variable thread_lock;
    unique_lock<mutex> ulock;
    mutex mtx;

    string maincommand;
    string args;
    int pid;
    bool running;
    bool shouldkill;
    thread t;

    void threadStart();

    public:
        ChildProcess(string, string);
        ~ChildProcess();
        int getPID() const { return pid; };
        void start();
        void stop();
        void kill();
};

class ProcessManager 
{
    mutex mtx;
    string maincommand;
    vector<ChildProcess*> processes;
    int start(string);

    public:
        ProcessManager(string command);
        ~ProcessManager();
        void spawn(string);
        void rest();
        void kill();
        mutex& getMutex() { return this->mtx; };
};

#endif

