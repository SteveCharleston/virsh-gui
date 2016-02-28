#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "diskimage.h"
#include "Snapshot.h"
#include "sshcommunication.h"

using namespace std;

Diskimage::Diskimage() : ssh(), path() {  }
Diskimage::Diskimage(SSHCommunication *ssh, string path)
{
    this->ssh = ssh;
    this->path = path;
}

string Diskimage::getPath()
{
    return path;
}

vector<Snapshot> Diskimage::getSnapshots()
{
    string snapshotListLine;
    vector<Snapshot> snapshotsVector;

    regex snapshotRegex(
            "(\\d+)\\s+"
            "(.*[^ ]+)\\s+"
            "(\\d+) "
            "(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})\\s+"
            "(\\d{2}:\\d{2}:\\d{2}.\\d{3})$"
            );

    string cmd = "qemu-img snapshot -l " + path;
    istringstream snapshotListStream(ssh->execCmd(cmd));

    getline(snapshotListStream, snapshotListLine);
    getline(snapshotListStream, snapshotListLine);

    while (getline(snapshotListStream, snapshotListLine)) {
        smatch regex_matches;
        if (! regex_match(snapshotListLine, regex_matches, snapshotRegex)) {
            break;
        }

        string id = regex_matches[1];
        string tag = regex_matches[2];
        string size = regex_matches[3];
        string date = regex_matches[4];
        string clock = regex_matches[5];

        Snapshot snapshot(id, tag, size, date, clock);
        std::cout << snapshot << std::endl;
        snapshotsVector.push_back(snapshot);
    }

    return snapshotsVector;
}
