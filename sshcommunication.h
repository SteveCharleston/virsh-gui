#ifndef SSHCOMMUNICATION_H
#define SSHCOMMUNICATION_H
#include <iostream>
#include <map>
#include <string>
#include <libssh/libsshpp.hpp>
#include "vm.h"

using namespace std;
class SSHCommunication
{
public:
    SSHCommunication(string user, string password, string host, int port);
    string execCmd(string cmd);
    map<string, VM> listVMs();
    //string dumpXML(string vmname);
private:
    ssh::Session *sshConnection;
    string user;
    string password;
    string host;
    int port;
};

#endif // SSHCOMMUNICATION_H
