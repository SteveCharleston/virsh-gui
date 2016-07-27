#ifndef VIRSHGUI_H
#define VIRSHGUI_H

#include <QMainWindow>
#include <map>
#include <QPushButton>
#include <QLineEdit>
#include "sshcommunication.h"

using namespace std;

namespace Ui {
class VirshGui;
}

class VirshGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit VirshGui(QWidget *parent = 0);
    ~VirshGui();

public slots:
    void makeSSHConnection();
    void fillLoginForm(int);
    void vmChosen(int row, int column);
    void refreshVmList();
    void toggleVMStatus();
    void shutdownVM();
    void rebootVM();
    void clearSnapshotSelectionsExcept(int snapshotTableIndex);
    void applySnapshot();
    void createSnapshot(string hddPath, string vmname, string snapshotName);
    void filterVMs(QString filter);
    void vncDisplay();
    void testButton();
    void toggleAddSnapshotButton(QPushButton *addSnapButton, QLineEdit *addSnapNameEdit);
    void savexml();

private:
    Ui::VirshGui *ui;
    SSHCommunication *ssh;
    map<string, VM> vmlist;
    void handleDisconnect();
    void populateVMList(map<string, VM>);
    void populateBookmarkList();
    void populateVMInfos(string vmname);
};

#endif // VIRSHGUI_H
