#include "shellserver.h"
#include "commands.h"

using namespace std;

SocketServer::SocketServer(const Options * opt)
{
    this->sockpath = opt->shellSockFile.c_str();
}

void SocketServer::start()
{
    log("Shell", sockpath);
    this->socketfd = socket(AF_UNIX, SOCK_STREAM, 0);

    log("Shell", "Creating socket address structure");
    sock_addr.sun_family = AF_UNIX;
    strncpy(sock_addr.sun_path, this->sockpath, sizeof(sock_addr.sun_path));
    this->sock_addr_len = sizeof(sock_addr);

    log("Shell", "Binding sock file");
    int optval = 1;
    setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    bind(this->socketfd, (struct sockaddr *) &sock_addr, sock_addr_len);

    log("Shell", "Listening to incoming connections");
    listen(this->socketfd, 5);
}

ShellServer::ShellServer(ProcessManager *manager)
{
    this->pm = manager;
    this->server = new SocketServer(pm->getOptions());
}

SocketClient * SocketServer::accept()
{
    return new SocketClient(
        ::accept(this->socketfd, (struct sockaddr *)&sock_addr, &sock_addr_len)
    );
}

SocketClient::SocketClient(socket_fd fd)
{
    this->socketfd = fd;
}

void SocketClient::close()
{
    ::close(this->socketfd);
}

string SocketClient::getMessage()
{
    size_t rsize = recv(this->socketfd, buffer, buffer_size, 0);
    return string(buffer, rsize);
}

ShellServer::~ShellServer()
{
    delete server;
}

void ShellServer::threadStart()
{
    this->server->start();
    ShellCommandFactory factory;

    while (pm->isRunning())
    {
        log("Shell", "Accepting next incoming connection");
        SocketClient * client = this->server->accept();

        log("Shell", "Incoming connection accepted");
        string command = client->getMessage();

        log("Shell", ("About to interpret : " + command).c_str());
        vector<string> args;
        stringstream ss(command);

        string s;
        while (getline(ss, s, ' ')) if (s.size() != 0) args.push_back(s);

        if (args.size() != 0)
        {
            SHELL_COMMANDS shellcmd = factory.getEntryFromString(args.at(0));
            ShellCommand * cmd = factory.fromEnum(shellcmd, &args);
            if (cmd->validate()) cmd->execute(pm);
            else cmd->showUsage();

            delete cmd;
        }

        client->close();
        delete client;
    }

    close(this->fd);
}

void ShellServer::stop() 
{
    socket_fd tmpfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un sock_addr;
    sock_addr.sun_family = AF_UNIX;
    strncpy(sock_addr.sun_path, pm->getOptions()->shellSockFile.c_str(), sizeof(sock_addr.sun_path));
    unsigned int sock_addr_len = sizeof(sock_addr);

    connect(tmpfd, (struct sockaddr *) &sock_addr, sock_addr_len);

    const char * killcmd = "kill";
    send(tmpfd, killcmd, strlen(killcmd), 0);

    close(tmpfd);
}

void ShellServer::start()
{
    this->t = thread(&ShellServer::threadStart, this);
}

void ShellServer::block()
{
    this->t.join();
}
