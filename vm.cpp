#include <iostream>
#include <string>
#include "vm.h"
#include "sshcommunication.h"

using namespace std;

VM::VM() : ssh(), id(), name(), status(VMStatus::unknown) { }

VM::VM(SSHCommunication *ssh, string id, string name, VMStatus status)
{
    this->ssh = ssh;
    this->id = id;
    this->name = name;
    this->status = status;
}

//VM(const VM &vm)
//{
//    ssh = vm.ssh;
//    id = vm.id;
//    name = vm.name;
//    status = vm.status;
//}

VM::~VM() { }

string VM::statusToString(VMStatus status)
{
    string vmstatus;

    if (status == VMStatus::running) {
        vmstatus = "running";
    } else if (status == VMStatus::idle) {
        vmstatus = "idle";
    } else if (status == VMStatus::paused) {
        vmstatus = "paused";
    } else if (status == VMStatus::shutdown) {
        vmstatus = "shutdown";
    } else if (status ==VMStatus::shutoff) {
        vmstatus = "shut off";
    } else if (status == VMStatus::crashed) {
        vmstatus = "crashed";
    } else if (status == VMStatus::dying) {
        vmstatus = "dying";
    } else if (status == VMStatus::pmsuspended) {
        vmstatus = "pmsuspended";
    } else {
        vmstatus = "unknown";
    }

    return vmstatus;
}

string VM::getID()
{
    return id;
}

string VM::getName()
{
    return name;
}

VMStatus VM::getStatus()
{
    return status;
}

ostream & operator<<(ostream &out, VM &vm)
{
    string vmstatus;
    if (vm.status == VMStatus::running) {
        vmstatus = "running";
    } else if (vm.status == VMStatus::idle) {
        vmstatus = "idle";
    } else if (vm.status == VMStatus::paused) {
        vmstatus = "paused";
    } else if (vm.status == VMStatus::shutdown) {
        vmstatus = "shutdown";
    } else if (vm.status ==VMStatus::shutoff) {
        vmstatus = "shutoff";
    } else if (vm.status == VMStatus::crashed) {
        vmstatus = "crashed";
    } else if (vm.status == VMStatus::dying) {
        vmstatus = "dying";
    } else if (vm.status == VMStatus::pmsuspended) {
        vmstatus = "pmsuspended";
    } else {
        vmstatus = "unknown";
    }
    out << vm.id << vm.name << vmstatus;

    return out;
}
