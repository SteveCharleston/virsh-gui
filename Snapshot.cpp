#include <iostream>
#include <string>
#include "Snapshot.h"

using namespace std;

Snapshot::Snapshot(string id, string tag, string size, string date, string clock)
{
    this->id = id;
    this->tag = tag;
    this->size = size;
    this->date = date;
    this->clock = clock;
}

string Snapshot::getID()
{
    return id;
}

string Snapshot::getTag()
{
    return tag;
}

string Snapshot::getSize()
{
    return size;
}

string Snapshot::getDate()
{
    return date;
}

string Snapshot::getClock()
{
    return clock;
}

ostream & operator<<(ostream &out, Snapshot &snapshot)
{
    out
        << "id: " << snapshot.id
        << ", tag: " << snapshot.tag
        << ", size: " << snapshot.size
        << ", date: " << snapshot.date
        << ", clock: " << snapshot.clock;

    return out;
}
