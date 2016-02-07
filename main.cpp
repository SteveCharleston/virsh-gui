#include <QApplication>
#include "virshgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VirshGui w;
    w.show();

    return a.exec();
}
