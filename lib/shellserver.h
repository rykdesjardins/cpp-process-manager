#ifndef CPP_CLI_H
#define CPP_CLI_H

#include "manager.h"
#include <csignal>
#include <iostream>
#include <sstream>
#include <thread>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

typedef int socket_fd;

class SocketClient
{
    static const int buffer_size = 1024;
    socket_fd socketfd;
    char buffer[buffer_size] = { 0 };

    public:
        SocketClient(socket_fd);
        void close();
        string getMessage();
};

class SocketServer 
{
    const char * sockpath;
    socket_fd socketfd;
    struct sockaddr_un sock_addr;
    unsigned int sock_addr_len;

    public:
        SocketServer(const Options *);
        void start();

        SocketClient * accept();
};

class ShellServer
{
    ProcessManager * pm;
    SocketServer * server;
    socket_fd fd;
    thread t;
    void threadStart();

    public:
        ShellServer(ProcessManager*);
        ~ShellServer();
        void start();
        void stop();
        void block();
};

#endif
