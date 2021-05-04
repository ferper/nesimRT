#include "mainScreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScreen w;
    w.setWindowTitle(QString("Neuronal Tool - ") + QString::number(VERSION));
    w.show();
    return a.exec();
}
