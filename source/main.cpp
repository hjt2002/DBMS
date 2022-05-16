#include "mainwindow.h"
#include <QApplication>
#include "path_global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.show();
    return a.exec();
}
