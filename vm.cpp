#include <iostream>
#include <string>
#include "vm.h"
#include "sshcommunication.h"

using namespace std;

VM::VM(SSHCommunication *ssh, int id, string name, VMStatus status)
{
    this->ssh = ssh;
    this->id = id;
    this->name = name;
    this->status = status;
}

VM::~VM() {}

int VM::getID()
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
