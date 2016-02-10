#ifndef VIRSHGUI_H
#define VIRSHGUI_H

#include <QMainWindow>
#include <map>
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

private:
    Ui::VirshGui *ui;
    SSHCommunication *ssh;
    map<string, VM> vmlist;
    void populateVMList(map<string, VM>);
    void populateBookmarkList();
};

#endif // VIRSHGUI_H
