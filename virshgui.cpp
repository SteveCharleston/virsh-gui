#include "virshgui.h"
#include "ui_virshgui.h"

VirshGui::VirshGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirshGui)
{
    ui->setupUi(this);
}

VirshGui::~VirshGui()
{
    delete ui;
}
