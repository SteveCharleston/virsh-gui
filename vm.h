#ifndef VM_H
#define VM_H
#include <iostream>
#include <string>
#include <libssh/libsshpp.hpp>

using namespace std;
class SSHCommunication;
enum class VMStatus {
        running,
        idle,
        paused,
        shutdown,
        shutoff,
        crashed,
        dying,
        pmsuspended,
};
class VM
{
public:
    VM (SSHCommunication *ssh, int id, string name, VMStatus status);
    ~VM ();
    int getID();
    string getName();
    VMStatus getStatus();

private:
    SSHCommunication *ssh;
    int id;
    string name;
    VMStatus status;
};
#endif /* ifndef VM_H */
