#ifndef CPP_MANAGER_H
#define CPP_MANAGER_H

#include "log.h"
#include "options.h"
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

#include <fcntl.h>
#include <unistd.h>

using namespace std;

struct ProcessStat
{
    int restarts;
    bool running;
    int pid;
    string maincommand;
    string args;
};

class ChildProcess
{
    condition_variable thread_lock;
    unique_lock<mutex> ulock;
    mutex mtx;

    string outputFile;
    string maincommand;
    string args;
    int pid;
    long starttime;
    int restarts = 0;
    bool running;
    bool shouldkill;
    thread t;

    void threadStart();
    void routineStart();

    public:
        ChildProcess(string, string);
        ~ChildProcess();
        int getPID() const { return pid; };
        void setOutputFile(string path) { outputFile = path; };
        ProcessStat getStats();
        void start();
        void stop();
        void kill();
};

class ProcessManager 
{
    mutex mtx;
    vector<ChildProcess*> processes;
    int start(string);
    Options* opt;
    bool running;

    public:
        ProcessManager(Options*);
        ~ProcessManager();
        void getInfo(vector<ProcessStat> *);
        void spawn(string);
        void rest();
        void kill();
        bool isRunning() { return running; };
        mutex& getMutex() { return this->mtx; };
};

#endif

