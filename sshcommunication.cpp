#include <iostream>
#include <string>
#include "sshcommunication.h"

using namespace std;

SSHCommunication::SSHCommunication(string user, string password, string host, int port)
{
    this->user = user;
    this->password = password;
    this->host = host;
    this->port = port;

    sshConnection = new ssh::Session();
    sshConnection->setOption(SSH_OPTIONS_HOST, host.c_str());
    sshConnection->setOption(SSH_OPTIONS_USER, user.c_str());
    sshConnection->setOption(SSH_OPTIONS_LOG_VERBOSITY, SSH_LOG_PROTOCOL);
    sshConnection->setOption(SSH_OPTIONS_PORT, &port);

    sshConnection->connect();

    sshConnection->userauthPassword(password.c_str());
}

string SSHCommunication::listVMs() {
    string cmd = "virsh list --all";

    return execCmd(cmd);
}

string SSHCommunication::execCmd(string cmd) {
    int nbytes;
    char buffer[256];
    string output;

    ssh::Channel chan(*sshConnection);
    chan.openSession();
    chan.requestExec(cmd.c_str());
    nbytes = chan.read(buffer, sizeof(buffer), 0);

    while (nbytes > 0) {
        string strBuf(buffer, nbytes);
        output.append(strBuf);
        nbytes = chan.read(buffer, sizeof(buffer), 0);
    }

    return output;
}
