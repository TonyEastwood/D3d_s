#include <QApplication>

#include "headers/mainappwindow.h"
#include "headers/mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainAppWindow mainWin;
    mainWin.setMinimumSize(QSize(500,500));
    mainWin.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mainWin.setWindowTitle("D3Ds");
    //mainWin.setStyleSheet(" background-color:"+CONSTANTS::backgroundColor+";");
    mainWin.showMaximized();
    return a.exec();
}
