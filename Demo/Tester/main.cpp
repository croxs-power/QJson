#include <QCoreApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainWindow w;
    w.Test();
    return  0;

    return a.exec();
}
