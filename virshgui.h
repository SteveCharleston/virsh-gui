#ifndef VIRSHGUI_H
#define VIRSHGUI_H

#include <QMainWindow>
#include "sshcommunication.h"

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

private:
    Ui::VirshGui *ui;
    SSHCommunication *ssh;
};

#endif // VIRSHGUI_H
