#include <string>
#include "virshgui.h"
#include "ui_virshgui.h"
#include "sshcommunication.h"

VirshGui::VirshGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirshGui)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    ui->vmListTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(makeSSHConnection()));
}

VirshGui::~VirshGui()
{
    delete ui;
}

void VirshGui::makeSSHConnection()
{
    //bool ok;
    string user = ui->userEdit->text().toStdString();
    string password = ui->passwordEdit->text().toStdString();
    string host = ui->hostEdit->text().toStdString();
    int port = ui->portEdit->text().toInt();
    ssh = new SSHCommunication(user, password, host, port);

    auto vmlist = ssh->listVMs();
    populateVMList(vmlist);
}

void VirshGui::populateVMList(map<string, VM> vmlist)
{
    int row = 0;
    for (auto vm : vmlist) {
        ui->vmListTable->setRowCount(row + 1);
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
        //ui->vmListTable->setItem(0, col, new QTableWidgetItem("foo"));
        //ui->vmListTable->setItem(1, col, new QTableWidgetItem("bar"));
        //ui->vmListTable->setItem(2, col, new QTableWidgetItem("baz"));
        row++;
    }
}
