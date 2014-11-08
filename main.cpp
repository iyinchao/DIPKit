#include "dipkit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(dipkit);

    QApplication a(argc, argv);
    DIPKit w;
    w.show();

    return a.exec();
}
