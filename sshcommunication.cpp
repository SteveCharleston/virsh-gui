#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <QtDebug>
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
    //sshConnection->setOption(SSH_OPTIONS_LOG_VERBOSITY, SSH_LOG_PROTOCOL);
    sshConnection->setOption(SSH_OPTIONS_LOG_VERBOSITY, SSH_LOG_NOLOG);
    sshConnection->setOption(SSH_OPTIONS_PORT, &port);

    sshConnection->connect();

    sshConnection->userauthPassword(password.c_str());
}

string SSHCommunication::execCmd(string cmd) {
    int nbytes;
    char buffer[256];
    string output;
    lastStdout = "";
    lastStderr = "";

    cmd.insert(0, "LANG=C ");

    ssh::Channel chan(*sshConnection);
    chan.openSession();
    // test
    //chan.requestPty();
    //chan.changePtySize(80, 24);
    //chan.requestShell();
    //nwritten = chan.write(cmd.c_str(), cmd.length());
    // end test
    chan.requestExec(cmd.c_str());
    nbytes = chan.read(buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        string strBuf(buffer, nbytes);
        output.append(strBuf);
        lastStdout.append(strBuf);
        nbytes = chan.read(buffer, sizeof(buffer), 0);
    }

    nbytes = chan.read(buffer, sizeof(buffer), 1);
    while (nbytes > 0) {
        string strBuf(buffer, nbytes);
        output.append(strBuf);
        nbytes = chan.read(buffer, sizeof(buffer), 1);
    }

    lastExitCode = chan.getExitStatus();

    chan.sendEof();
    chan.close();

    return output;
}

string SSHCommunication::getLastStdout()
{
    return lastStdout;
}

string SSHCommunication::getLastStderr()
{
    return lastStderr;
}

int SSHCommunication::getLastExitCode()
{
    return lastExitCode;
}

map<string, VM> SSHCommunication::listVMs() {
    string out, vmlistLine;
    string cmd = "virsh list --all";
    istringstream vmlistStream(execCmd(cmd));
    map<string, VM> vmlist;

    getline(vmlistStream, vmlistLine);
    getline(vmlistStream, vmlistLine);

    while (getline(vmlistStream, vmlistLine)) {
        istringstream vmlistLineStream(vmlistLine);
        string id, name, strstatus;
        VMStatus vmstatus;

        vmlistLineStream >> id >> name >> strstatus; // split on whitespace
        //cout << id << name << strstatus << endl;

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

string SSHCommunication::getHost()
{
    return host;
}

//string SSHCommunication::dumpXML(string vmname)
//{
//    string cmd = "virsh dumpxml " + vmname;
//    return execCmd(cmd);
//}

