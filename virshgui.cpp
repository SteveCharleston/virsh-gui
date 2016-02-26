#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "virshgui.h"
#include "ui_virshgui.h"
#include "sshcommunication.h"
#include "basic-xml-syntax-highlighter/basic-xml-syntax-highlighter/BasicXMLSyntaxHighlighter.h"

using namespace std;

string join(vector<string> list, const char* delim)
{
    stringstream liststream;
    copy(list.begin(), list.end(),
            ostream_iterator<string>(liststream, delim));
    string joinstr = liststream.str();
    joinstr.erase(joinstr.end() - 2);

    return joinstr;
}

VirshGui::VirshGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirshGui)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    ui->vmListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vmListTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Name" << "Status");
    populateBookmarkList();
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(makeSSHConnection()));
    connect(ui->bookmarList, SIGNAL(currentIndexChanged(int)), this, SLOT(fillLoginForm(int)));
    connect(ui->vmListTable, SIGNAL(cellClicked(int, int)), this, SLOT(vmChosen(int, int)));
    connect(ui->refreshVmList, SIGNAL(clicked()), this, SLOT(refreshVmList()));
    connect(ui->startStopButton, SIGNAL(clicked(bool)), this, SLOT(toggleVMStatus()));
    connect(ui->rebootButton, SIGNAL(clicked(bool)), this, SLOT(rebootVM()));
}

VirshGui::~VirshGui()
{
    delete ui;
}

void VirshGui::makeSSHConnection()
{
    string user = ui->userEdit->text().toStdString();
    string password = ui->passwordEdit->text().toStdString();
    string host = ui->hostEdit->text().toStdString();
    int port = ui->portEdit->text().toInt();
    ssh = new SSHCommunication(user, password, host, port);

    vmlist = ssh->listVMs();
    populateVMList(vmlist);
}

void VirshGui::populateVMList(map<string, VM> vmlist)
{
    bool listIsEmpty = ui->vmListTable->rowCount() == 0;
    int row = 0;
    for (auto vm : vmlist) {
        if (listIsEmpty) {
            ui->vmListTable->setRowCount(row + 1);
        }
        string strstatus = VM::statusToString(vm.second.getStatus());
        //cout << vm.second.getID() << " " << vm.second.getName() << " " << strstatus << endl;

        QTableWidgetItem *id = new QTableWidgetItem(
                QString::fromStdString(vm.second.getID()));
        QTableWidgetItem *name = new QTableWidgetItem(
                QString::fromStdString(vm.second.getName()));
        QTableWidgetItem *status = new QTableWidgetItem(
                QString::fromStdString(strstatus));

        ui->vmListTable->setItem(row, 0, id);
        ui->vmListTable->setItem(row, 1, name);
        ui->vmListTable->setItem(row, 2, status);

        row++;
    }
}

void VirshGui::populateBookmarkList()
{
    ifstream bookmarkFile;
    string line;

    bookmarkFile.open("bookmarks.xml", ios::in);
    if (! bookmarkFile.is_open()) {
        return;
    }

    ui->bookmarList->addItem("");
    while (getline(bookmarkFile, line)) {
        istringstream linestream(line);
        ostringstream out;
        string host, port, user, password;

        linestream >> host >> port >> user >> password;

        if (! host.empty() && host[0] != '#') {
            //std::cout << host << std::endl;
            out << user << "@" << host << ":" << port;
            ui->bookmarList->addItem(out.str().c_str());
        }
    }

    bookmarkFile.close();
}

void VirshGui::toggleVMStatus()
{
    string vmname = ui->vmnameLabel->text().toStdString();
    VM vm = vmlist[vmname];
    if (vm.getStatus() == VMStatus::shutoff) {
        vm.start();
    } else if (vm.getStatus() == VMStatus::running) {
        vm.destroy();
    }

    refreshVmList();
}

void VirshGui::rebootVM()
{
    
}

void VirshGui::refreshVmList()
{
    //std::cout << "refreshVmList" << std::endl;
    vmlist = ssh->listVMs();
    populateVMList(vmlist);
    if (! ui->vmnameLabel->text().isEmpty()) {
        populateVMInfos(ui->vmnameLabel->text().toStdString());
    }
}

void VirshGui::fillLoginForm(int hostidx)
{
    ifstream bookmarkFile;
    string line;
    string host, port, user, password;
    int count = 1;

    if (hostidx == 0) {
        ui->hostEdit->setText("");
        ui->portEdit->setText("");
        ui->userEdit->setText("");
        ui->passwordEdit->setText("");

        return;
    }

    bookmarkFile.open("bookmarks.xml", ios::in);
    if (! bookmarkFile.is_open()) {
        return;
    }


    while (getline(bookmarkFile, line) && count <= hostidx) {
        //std::cout << count << ": " << line << std::endl;
        istringstream linestream(line);
        linestream >> host >> port >> user >> password;
        if (host[0] != '#' && ! line.empty()) {
            count++;
        }
    }

    ui->hostEdit->setText(host.c_str());
    ui->portEdit->setText(port.c_str());
    ui->userEdit->setText(user.c_str());
    ui->passwordEdit->setText(password.c_str());

    bookmarkFile.close();
}

void VirshGui::populateVMInfos(string vmname)
{
    VMStatus vmstatus = vmlist[vmname].getStatus();
    string strstatus = vmlist[vmname].statusToString(vmstatus);
    string memory = vmlist[vmname].getMemory();
    string vmxml = vmlist[vmname].dumpXML();
    string cpuCount = vmlist[vmname].getCPUCount();
    string ostype = vmlist[vmname].getOSType();
    string arch = vmlist[vmname].getArch();
    vector<string> bootDevs = vmlist[vmname].getBootDevs();
    vector<string> hvFeatures = vmlist[vmname].getHVFeatures();
    vector<string> cpuFeatures = vmlist[vmname].getCPUFeatures();

    string bootDevStr = join(bootDevs, ", ");
    string hvFeatureStr = join(hvFeatures, ", ");
    string cpuFeatureStr = join(cpuFeatures, ", ");

    if (vmstatus == VMStatus::shutoff) {
        ui->startStopButton->setText("VM starten");
        ui->rebootButton->setDisabled(true);
    } else if (vmstatus == VMStatus::running) {
        ui->startStopButton->setText("VM ausschalten");
    } else {
        ui->startStopButton->setText("keine Aktion");
        ui->startStopButton->setDisabled(true);
        ui->rebootButton->setDisabled(true);
    }

    ui->xmlDisplay->setText(QString::fromStdString(vmxml));
    ui->vmnameLabel->setText(QString::fromStdString(vmname));
    ui->statusLabel->setText(QString::fromStdString(strstatus));
    ui->memoryLabel->setText(QString::fromStdString(memory));
    ui->cpucountLabel->setText(QString::fromStdString(cpuCount));
    ui->typeLabel->setText(QString::fromStdString(ostype));
    ui->archLabel->setText(QString::fromStdString(arch));
    ui->bootdevLabel->setText(QString::fromStdString(bootDevStr));
    ui->hvFeaturesLabel->setText(QString::fromStdString(hvFeatureStr));
    ui->hvFeaturesLabel->setWordWrap(true);
    ui->cpuFeaturesLabel->setText(QString::fromStdString(cpuFeatureStr));
    ui->cpuFeaturesLabel->setWordWrap(true);
}

void VirshGui::vmChosen(int row, int column)
{
    Q_UNUSED(column);

    BasicXMLSyntaxHighlighter *highlighter = new BasicXMLSyntaxHighlighter(ui->xmlDisplay);
    Q_UNUSED(highlighter);

    string vmname = ui->vmListTable->item(row, 1)->text().toStdString();
    populateVMInfos(vmname);
}
