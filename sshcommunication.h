#ifndef SSHCOMMUNICATION_H
#define SSHCOMMUNICATION_H
#include <iostream>
#include <map>
#include <string>
#include "libsshpp.hpp"
#include "vm.h"

using namespace std;
class SSHCommunication
{
public:
    SSHCommunication(string user, string password, string host, int port);
    string execCmd(string cmd);
    void writeFile(string filename, string content);
    string getHost();
    int getLastExitCode();
    string getLastStdout();
    string getLastStderr();
    map<string, VM> listVMs();
    //string dumpXML(string vmname);
private:
    ssh::Session *sshConnection;
    string user;
    string password;
    string host;
    int port;
    int lastExitCode;
    string lastStdout;
    string lastStderr;
};

#endif // SSHCOMMUNICATION_H
