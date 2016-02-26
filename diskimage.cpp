#include <iostream>
#include <string>
#include "diskimage.h"

using namespace std;

Diskimage::Diskimage() : ssh(), path() {  }
Diskimage::Diskimage(SSHCommunication *ssh, string path)
{
    this->ssh = ssh;
    this->path = path;
}
