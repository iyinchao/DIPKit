#include "dipkit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DIPKit w;
    w.show();

    return a.exec();
}
