#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(285,120);
    w.setFixedSize(285, 120);
    w.show();
    return a.exec();
}
