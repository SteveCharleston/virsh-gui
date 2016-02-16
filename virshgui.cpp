#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "virshgui.h"
#include "ui_virshgui.h"
#include "sshcommunication.h"

using namespace std;

VirshGui::VirshGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirshGui)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    ui->vmListTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    populateBookmarkList();
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(makeSSHConnection()));
    connect(ui->bookmarList, SIGNAL(currentIndexChanged(int)), this, SLOT(fillLoginForm(int)));
    connect(ui->vmListTable, SIGNAL(cellClicked(int, int)), this, SLOT(vmChosen(int, int)));
    connect(ui->refreshVmList, SIGNAL(clicked()), this, SLOT(refreshVmList()));
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
        cout << vm.second.getID() << " " << vm.second.getName() << " " << strstatus << endl;

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

void VirshGui::refreshVmList()
{
    std::cout << "refreshVmList" << std::endl;
    vmlist = ssh->listVMs();
    populateVMList(vmlist);

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

void VirshGui::vmChosen(int row, int column)
{
    Q_UNUSED(column);
    string vmname = ui->vmListTable->item(row, 1)->text().toStdString();
    VMStatus vmstatus = vmlist[vmname].getStatus();
    string strstatus = vmlist[vmname].statusToString(vmstatus);
    string memory = vmlist[vmname].getMemory();
    string vmxml = vmlist[vmname].dumpXML();
    string cpuCount = vmlist[vmname].getCPUCount();
    string ostype = vmlist[vmname].getOSType();
    string arch = vmlist[vmname].getArch();
    vector<string> bootDevs = vmlist[vmname].getBootDevs();

    stringstream bootStream;
    copy(bootDevs.begin(), bootDevs.end(),
            ostream_iterator<string>(bootStream, ", "));
    string bootDevStr = bootStream.str();
    bootDevStr.erase(bootDevStr.end() - 2);

    //string vmxml = ssh->dumpXML(vmname);
    ui->xmlDisplay->setText(QString::fromStdString(vmxml));
    ui->vmnameLabel->setText(QString::fromStdString(vmname));
    ui->statusLabel->setText(QString::fromStdString(strstatus));
    ui->memoryLabel->setText(QString::fromStdString(memory));
    ui->cpucountLabel->setText(QString::fromStdString(cpuCount));
    ui->typeLabel->setText(QString::fromStdString(ostype));
    ui->archLabel->setText(QString::fromStdString(arch));
    ui->bootdevLabel->setText(QString::fromStdString(bootDevStr));
}
