#ifndef SNAPSHOT_H_EA08JPYN
#define SNAPSHOT_H_EA08JPYN

#include <string>

using namespace std;

class Snapshot
{
public:
    Snapshot (string id, string tag, string size, string date, string clock);
    string getID();
    string getTag();
    string getSize();
    string getDate();
    string getClock();
    friend ostream & operator<<(ostream &out, Snapshot &snapshot);

private:
    string id;
    string tag;
    string size;
    string date;
    string clock;
};

#endif /* end of include guard: SNAPSHOT_H_EA08JPYN */
