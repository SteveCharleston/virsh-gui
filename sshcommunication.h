#ifndef SSHCOMMUNICATION_H
#define SSHCOMMUNICATION_H
#include <iostream>
#include <map>
#include <string>
#include <libssh/libsshpp.hpp>

using namespace std;
class SSHCommunication
{
public:
    SSHCommunication(string user, string password, string host, int port);
    map<string, string> listVMs();
    string execCmd(string cmd);
private:
    ssh::Session *sshConnection;
    string user;
    string password;
    string host;
    int port;
};

#endif // SSHCOMMUNICATION_H