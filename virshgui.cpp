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
    cout << ssh->listVMs() << endl;
}
