#include <QApplication>

#include "headers/mainappwindow.h"
#include "headers/mainwindow.h"
#include "headers/mainwindowformessagereceive.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    MainAppWindow mainWin;


    MainWindowForMessageReceive messageReceiver;
    //connect(this, &MainAppWindow::infoDisplay, this, &MainAppWindow::coutInfo);

    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalAppendMesh,&mainWin,&MainAppWindow::signalAppendMesh);
    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalClearMeshesData,&mainWin,&MainAppWindow::signalClearMeshesData);
    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::appIntegrate,&mainWin,&MainAppWindow::appIntegrate);
    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::appChangeSize,&mainWin,&MainAppWindow::appChangeSize);
    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::appHide,&mainWin,&MainAppWindow::hide);
    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::appShow,&mainWin,&MainAppWindow::show);
    QObject::connect(&messageReceiver,&MainWindowForMessageReceive::appToClose,&mainWin,&MainAppWindow::appToClose);


    messageReceiver.showMinimized();
    messageReceiver.hide();


    mainWin.setMinimumSize(QSize(500,500));
    mainWin.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mainWin.setWindowTitle("D3Ds");
    //mainWin.setStyleSheet(" background-color:"+CONSTANTS::backgroundColor+";");
    mainWin.setContentsMargins(0,0,0,0);

    if(argc==2)
    {
        //progress bar
        QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalSetQuantity,&mainWin,&MainAppWindow::setMaxValue);
        QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalSetValue,&mainWin,&MainAppWindow::setCurrentValue);
        QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalShowProgressBar,&mainWin,&MainAppWindow::showProgressBar);
        QObject::connect(&mainWin,&MainAppWindow::signalCancelScanning,&messageReceiver,&MainWindowForMessageReceive::cancelScanning);
        mainWin.Initialize(true);
        HWND hw=(HWND)std::stoull(argv[1]);
        //HWND hw=(HWND)atoi(argv[1]);
        emit messageReceiver.appIntegrate(hw);

    }
    else if(argc==4)
    {
        //progress bar
        QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalSetQuantity,&mainWin,&MainAppWindow::setMaxValue);
        QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalSetValue,&mainWin,&MainAppWindow::setCurrentValue);
        QObject::connect(&messageReceiver,&MainWindowForMessageReceive::signalShowProgressBar,&mainWin,&MainAppWindow::showProgressBar);
        QObject::connect(&mainWin,&MainAppWindow::signalCancelScanning,&messageReceiver,&MainWindowForMessageReceive::cancelScanning);
        mainWin.Initialize(true);
        HWND hw=(HWND)std::stoull(argv[1]);
        int width=QString(argv[2]).toInt();
        int height=QString(argv[3]).toInt();
        messageReceiver.appIntegrate(hw);
        emit messageReceiver.appChangeSize(0,0,width,height);
    }
    else  mainWin.Initialize(false);
    mainWin.show();



    return a.exec();
}
