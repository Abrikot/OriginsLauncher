#include <QApplication>
#include "mainwindow.h"
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //DownloadWindow* m = DownloadWindow::createDownloadWindow();
    //m->setVisible(true);
    new MainWindow();

    return a.exec();
}
