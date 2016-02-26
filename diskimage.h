#ifndef DISKIMAGE_H_A1M5ZUBG
#define DISKIMAGE_H_A1M5ZUBG
#include <string>
#include "libssh/libsshpp.hpp"

using namespace std;

class SSHCommunication;
class Diskimage
{
public:
    Diskimage();
    Diskimage (SSHCommunication *ssh, string path);

private:
    SSHCommunication *ssh;
    string path;
};

#endif /* end of include guard: DISKIMAGE_H_A1M5ZUBG */
