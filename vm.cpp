#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2/tinyxml2.h"
#include "vm.h"
#include "sshcommunication.h"
#include "diskimage.h"

using namespace std;
using namespace tinyxml2;

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

string VM::dumpXML()
{
    string cmd = "virsh dumpxml " + name;
    return ssh->execCmd(cmd);
}

void VM::start()
{
    string cmd = "virsh start " + name;
    ssh->execCmd(cmd);
}

void VM::reboot()
{
    string cmd = "virsh reboot " + name;
    ssh->execCmd(cmd);
}

void VM::destroy()
{
    string cmd = "virsh destroy " + name;
    ssh->execCmd(cmd);
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

string VM::getOSType()
{
    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    string ostype = doc.FirstChild()
        ->FirstChildElement("os")
        ->FirstChildElement("type")
        ->GetText();

    return ostype;
}

string VM::getArch()
{
    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    string arch = doc.FirstChild()
        ->FirstChildElement("os")
        ->FirstChildElement("type")
        ->Attribute("arch");

    if (arch.empty()) {
        arch = "-";
    }

    return arch;
}

vector<string> VM::getBootDevs()
{
    std::vector<string> boot;

    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    XMLNode * bootNode = doc.FirstChild()
        ->FirstChildElement("os")
        ->FirstChildElement("boot"); // init for loop

    for(; bootNode != NULL; bootNode = bootNode->NextSiblingElement("boot")) {
        boot.push_back(bootNode->ToElement()->Attribute("dev"));
    }

    return boot;
}

vector<Diskimage> VM::getHDDImages()
{
    std::vector<Diskimage> diskImages;

    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    XMLNode * devicesNode = doc.FirstChild()
        ->FirstChildElement("devices");

    XMLNode * diskNode = devicesNode->FirstChildElement("disk");

    for(; diskNode != NULL; diskNode = diskNode->NextSiblingElement("disk")) {
        string type(diskNode->ToElement()->Attribute("type"));
        string device(diskNode->ToElement()->Attribute("device"));
        if (type == "file" && device == "disk") {
            string sourcefile = diskNode
                ->FirstChildElement("source")
                ->Attribute("file");
            //std::cout << sourcefile << std::endl;
            diskImages.push_back(Diskimage(ssh, sourcefile));
        }
    }

    return diskImages;
}

vector<string> VM::getCPUFeatures()
{
    std::vector<string> cpuFeatures;

    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    XMLNode * cpuNode = doc.FirstChild()
        ->FirstChildElement("cpu");

    if (! cpuNode) {
        return cpuFeatures; // no features so empty list
    }

    XMLNode * cpuFeatureNode = cpuNode->FirstChildElement("feature");

    while (cpuFeatureNode) {
        cpuFeatures.push_back(cpuFeatureNode->ToElement()->Attribute("name"));
        cpuFeatureNode = cpuFeatureNode->NextSiblingElement("feature");
    }

    return cpuFeatures;
}

vector<string> VM::getHVFeatures()
{
    std::vector<string> features;

    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    XMLNode * featureNode = doc.FirstChild()
        ->FirstChildElement("features");

    if (featureNode->FirstChildElement("acpi")) {
        features.push_back("acpi");
    }

    if (featureNode->FirstChildElement("pae")) {
        features.push_back("pae");
    }

    if (featureNode->FirstChildElement("apic")) {
        features.push_back("apic");
    }

    if (featureNode->FirstChildElement("hap")) {
        features.push_back("hap");
    }

    if (featureNode->FirstChildElement("viridian")) {
        features.push_back("viridian");
    }

    if (featureNode->FirstChildElement("privent")) {
        features.push_back("privent");
    }

    if (featureNode->FirstChildElement("pvspinlock")) {
        features.push_back("pvspinlock");
    }

    if (featureNode->FirstChildElement("gic")) {
        features.push_back("gic");
    }

    return features;
}

string VM::getMemory()
{
    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    XMLElement *memoryElem = doc.FirstChild()
        ->FirstChildElement("memory");
    string memory = memoryElem->GetText();
    string memoryUnit = memoryElem->Attribute("unit");

    return memory + " " + memoryUnit;
}

string VM::getCPUCount()
{
    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    string cpuCount = doc.FirstChild()
        ->FirstChildElement("vcpu")->GetText();

    return cpuCount;
}

string VM::getUUID()
{
    tinyxml2::XMLDocument doc;
    doc.Parse(dumpXML().c_str());
    string uuid = doc.FirstChild()
        ->FirstChildElement("uuid")->GetText();

    return uuid;
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
