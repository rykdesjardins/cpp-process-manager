#include "manager.h"

ProcessManager::ProcessManager(Options * options)
{
    log("ProcessManager", "Creating Process Manager");
    this->opt = options;
    this->running = true;
}

ProcessManager::~ProcessManager()
{
    log("ProcessManager", "Object destruction");
}

void ProcessManager::spawn(string args)
{
    log("ProcessManager", "Creating new Child Process, pushing reference to vector");
    ChildProcess * proc = new ChildProcess(this->opt->processName, args);
    if (!this->opt->outputFile.empty())
    {
        proc->setOutputFile(this->opt->outputFile);
    }

    proc->start();
    this->processes.push_back(proc);
}

void ProcessManager::getInfo(vector<ProcessStat> *stats)
{
    for (ChildProcess* proc : this->processes)
    {
        stats->push_back(proc->getStats());
    }
}

void ProcessManager::rest()
{
    log("ProcessManager", "Ready to rest; flipping running booleans");
    mtx.lock();
    for (ChildProcess* proc : this->processes)
    {
        proc->stop();
        delete proc;
    }
    this->processes.clear();
    this->running = false;
    mtx.unlock();
}

void ProcessManager::kill()
{
    log("ProcessManager", "Killing all pids and joining threads");
    mtx.lock();
    for (ChildProcess* proc : this->processes)
    {
        proc->kill();
        delete proc;
    }
    this->processes.clear();
    this->running = false;
    mtx.unlock();
}

ChildProcess::ChildProcess(string maincommand, string args)
{
    log("ChildProcess", ("Creating Child Process using command " + maincommand).c_str());
    this->maincommand = maincommand;
    this->args = args;

    this->ulock = unique_lock<mutex>(this->mtx);
}

ChildProcess::~ChildProcess()
{
    log("ChildProcess", "Deleting Child Process");
}

void ChildProcess::routineStart()
{
    while (this->running) 
    {
        this->restarts++;
        this->pid = fork();

        if (this->pid == 0)
        {
            if (!this->outputFile.empty())
            {
                int fd = open(this->outputFile.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
                dup2(fd, 1);
                dup2(fd, 2);

                close(fd);
            }

            execl(("/usr/bin/" + maincommand).c_str(), maincommand.c_str(), this->args.c_str(), NULL);
        } else {
            waitpid(this->pid, NULL, 0);
        }
    }
}

void ChildProcess::threadStart()
{
    log("ChildProcess", "From thread, making system call though exec");

    thread routine(&ChildProcess::routineStart, this);

    this->thread_lock.wait(this->ulock);
    if (this->shouldkill) {
        ::kill(this->pid, SIGKILL);
    }

    routine.join();
}

void ChildProcess::start()
{
    log("ChildProcess", "Child process spawning a new thread");
    this->running = true;
    this->starttime = time(nullptr);
    this->t = thread(&ChildProcess::threadStart, this);
}

void ChildProcess::stop()
{
    log("ChildProcess", "Child process stopped, waiting for exit code");
    this->running = false;
    this->thread_lock.notify_one();
    this->t.join();
}

void ChildProcess::kill()
{
    log("ChildProcess", "Forcefully killing process");
    this->shouldkill = true;
    this->stop();
}

ProcessStat ChildProcess::getStats()
{
    return ProcessStat {
        this->restarts,
        this->running,
        this->pid,
        this->maincommand,
        this->args
    };
}
