#ifndef VIRSHGUI_H
#define VIRSHGUI_H

#include <QMainWindow>

namespace Ui {
class VirshGui;
}

class VirshGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit VirshGui(QWidget *parent = 0);
    ~VirshGui();

private:
    Ui::VirshGui *ui;
};

#endif // VIRSHGUI_H
