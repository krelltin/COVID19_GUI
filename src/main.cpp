#include "mainwindow.h"
#include <QApplication>
#include <TApplication.h>
#include <TSystem.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    TApplication rootapp("mmDaq Online Monitoring", &argc, argv);
    rootapp.SetReturnFromRun(true);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
