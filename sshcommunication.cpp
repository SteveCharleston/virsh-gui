#include <iostream>
#include <map>
#include <string>
#include <sstream>
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

map<string, string> SSHCommunication::listVMs() {
    string out, tmp;
    string cmd = "virsh list --all";
    istringstream vmlistStream(execCmd(cmd));
    map<string, string> vmlist;

    getline(vmlistStream, tmp);
    getline(vmlistStream, tmp);

    string id, name, status;
    vmlistStream >> id;
    vmlistStream >> name;
    vmlistStream >> status;
    //while (getline(vmlistStream, tmp)) {
    //    out.append(tmp);
    //}

    std::cout
        << "id: " << id
        << "name: " << name
        << "status: " << status
        << std::endl;

    return vmlist;
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