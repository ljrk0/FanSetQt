#include "fansetqt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FanSetQt w;
    w.show();

    return a.exec();
}
