#include "mainwindow.h"
#include <QApplication>


//This will be rewriten with qml frontend
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
