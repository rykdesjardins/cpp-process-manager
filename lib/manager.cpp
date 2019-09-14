#include "manager.h"

ProcessManager::ProcessManager(string command)
{
    log("ProcessManager", "Creating Process Manager");
    this->maincommand = command;
}

ProcessManager::~ProcessManager()
{
    log("ProcessManager", "Object destruction");
}

void ProcessManager::spawn(string args)
{
    log("ProcessManager", "Creating new Child Process, pushing reference to vector");
    ChildProcess * proc = new ChildProcess(this->maincommand, args);
    proc->start();
    this->processes.push_back(proc);
}

void ProcessManager::rest()
{
    log("ProcessManager", "Ready to rest; flipping running booleans");
    for (ChildProcess* proc : this->processes)
    {
        proc->stop();
    }
}

void ProcessManager::kill()
{
    log("ProcessManager", "Killing all pids and joining threads");
    for (ChildProcess* proc : this->processes)
    {
        proc->kill();
    }
}

ChildProcess::ChildProcess(string maincommand, string args)
{
    log("ChildProcess", "Creating Child Process");
    this->maincommand = maincommand;
    this->args = args;

    this->ulock = unique_lock<mutex>(this->mtx);
}

ChildProcess::~ChildProcess()
{
    log("ChildProcess", "Deleting Child Process");
}

void ChildProcess::threadStart()
{
    log("ChildProcess", "From thread, making system call though exec");

    thread ct([=] {
        while (this->running) 
        {
            this->pid = fork();

            if (this->pid == 0)
            {
                execl(("/usr/bin/" + maincommand).c_str(), maincommand.c_str(), this->args.c_str(), NULL);
            }

            waitpid(this->pid, NULL, 0);
        }
    });

    this->thread_lock.wait(this->ulock);
    if (this->shouldkill) {
        ::kill(this->pid, SIGKILL);
    }

    ct.join();
    quick_exit(0);
}

void ChildProcess::start()
{
    log("ChildProcess", "Child process spawing a new thread");
    this->running = true;
    this->t = thread([=] { threadStart(); });
}

void ChildProcess::stop()
{
    log("ChildProcess", "Child process stopped, waiting for exit code");
    this->running = false;
    this->thread_lock.notify_one();
}

void ChildProcess::kill()
{
    log("ChildProcess", "Forcefully killing process");
    this->shouldkill = true;
    this->stop();
}
