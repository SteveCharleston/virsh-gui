#ifndef VM_H
#define VM_H
#include <iostream>
#include <string>
#include <libssh/libsshpp.hpp>

using namespace std;
class SSHCommunication;
enum class VMStatus {
        unknown,
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
    VM();
    VM (SSHCommunication *ssh, string id, string name, VMStatus status);
    //VM(const VM &vm);
    ~VM ();
    string getID();
    string getName();
    VMStatus getStatus();
    static string statusToString(VMStatus status);
    friend ostream & operator<<(ostream &out, VM &vm);

private:
    SSHCommunication *ssh;
    string id;
    string name;
    VMStatus status;
};
#endif /* ifndef VM_H */
