#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "sshcommunication.h"
#include "vm.h"

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

map<string, VM> SSHCommunication::listVMs() {
    string out, tmp;
    string cmd = "virsh list --all";
    istringstream vmlistStream(execCmd(cmd));
    map<string, VM> vmlist;

    getline(vmlistStream, tmp);
    getline(vmlistStream, tmp);

    while (vmlistStream) {
        string id, name, strstatus;
        VMStatus vmstatus;

        vmlistStream >> id >> name >> strstatus; // split on whitespace

        if (strstatus.empty()) {
            break;
        } else if (strstatus == "running") {
            vmstatus = VMStatus::running;
        } else if (strstatus == "idle") {
            vmstatus = VMStatus::idle;
        } else if (strstatus == "paused") {
            vmstatus = VMStatus::paused;
        } else if (strstatus == "shutdown") {
            vmstatus = VMStatus::shutdown;
        } else if (strstatus == "shut") {
            vmstatus = VMStatus::shutoff;
        } else if (strstatus == "crashed") {
            vmstatus = VMStatus::crashed;
        } else if (strstatus == "dying") {
            vmstatus = VMStatus::dying;
        } else if (strstatus == "pmsuspended") {
            vmstatus = VMStatus::pmsuspended;
        } else {
            vmstatus = VMStatus::unknown;
        }

        vmlist[name] = VM(this, id, name, vmstatus);
        //std::cout
        //    << "id: " << id
        //    << " name: " << name
        //    << " status: " << strstatus
        //    << std::endl;
    }

    return vmlist;
}

string SSHCommunication::execCmd(string cmd) {
    int nbytes;
    char buffer[256];
    string output;

    cmd.insert(0, "LANG=C ");

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
