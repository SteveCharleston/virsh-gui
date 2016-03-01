#ifndef VM_H
#define VM_H
#include <iostream>
#include <string>
#include <vector>
#include "libssh/libsshpp.hpp"
#include "diskimage.h"

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
    string getMemory();
    string dumpXML();
    void start();
    void shutdown();
    void reboot();
    void destroy();
    string getUUID();
    string getOSType();
    vector<Diskimage> getHDDImages();
    vector<string> getHVFeatures();
    vector<string> getCPUFeatures();
    string getArch();
    string getCPUCount();
    string getVNCPort();
    vector<string> getBootDevs();
    static string statusToString(VMStatus status);
    friend ostream & operator<<(ostream &out, VM &vm);

private:
    SSHCommunication *ssh;
    string id;
    string name;
    VMStatus status;
};
#endif /* ifndef VM_H */
