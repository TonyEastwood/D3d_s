#include "../headers/mainwindowformessagereceive.h"
#include "ui_mainwindowformessagereceive.h"

MainWindowForMessageReceive::MainWindowForMessageReceive(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowForMessageReceive)
{
        WM_NewMesh = RegisterWindowMessageA(WM_NewMeshName);
        WM_FilePath = RegisterWindowMessageA(WM_FilePathName);


        WM_Integrate= RegisterWindowMessageA(WM_IntegrateName);  // wParam - parent HWND
        WM_CloseProgram= RegisterWindowMessageA(WM_CloseProgramName);
        WM_ChangeSize= RegisterWindowMessageA(WM_ChangeSizeName);       //wParam - width lParam - high
        WM_SwitchVisibility= RegisterWindowMessageA(WM_SwitchVisibilityName);  //wParam 0 - not visible 1 - visible

    ui->setupUi(this);
}
bool MainWindowForMessageReceive::nativeEvent(const QByteArray &eventType, void *message, long *result)
{

    MSG *msg = static_cast<MSG*>(message);

    qDebug()<<"Message2=="<<msg->message;
    //if(GetMessage(&msg, NULL, NULL, NULL)>0){ // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
    if(msg->message==WM_NewMesh)
    {
        QStringList stringList;
        if(!pathToFile.isEmpty())
        {
            QFile file(pathToFile);
            QByteArray data;
            if(file.open(QIODevice::ReadOnly))
            {
                while(!file.atEnd())
                    stringList.append(QString::fromStdString(file.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0]);
            }
            file.close();

            //emit infoDisplay("MessageParse start");
            // for(auto &i:stringList)
            //   emit infoDisplay(i);
            //emit infoDisplay("MessageParse end");
            //   qDebug()<<data;
            emit signalAppendMesh(stringList,pathToFile);
            qDebug()<<"Emit signal append mesh 2";
        }
        return true;
    }

    if(msg->message==WM_FilePath)
    {
        QSettings m("HKEY_CURRENT_USER\\Software\\D3D-s\\AlignLab",QSettings::Registry64Format);
        // qDebug()<<GetRegistryValueString(HKEY_CURRENT_USER, 'Software\D3D-s\AlignLab',  'PathToMeshList');
        QString val = m.value("PathToMeshList").toString();
        pathToFile=val;
        // emit infoDisplay(val.toLocal8Bit());
        qDebug()<<"Emit signal clear MeshesData2";
        emit signalClearMeshesData();

        val = m.value("quantity").toString();
        emit signalSetQuantity(val.toInt());

        return true;
    }

    if(msg->message==WM_Integrate)
    {
       // HWND hwndNewParent=(HWND) msg->wParam;

        emit appIntegrate((HWND) msg->wParam);

        return true;
    }

    if(msg->message==WM_ChangeSize)
    {
        emit appChangeSize(0,0,(int)msg->wParam,(int)msg->lParam);
        //this->setGeometry(0,0,(int)msg->wParam,(int)msg->lParam);
        return true;
    }
    if(msg->message==WM_SwitchVisibility)
    {
        if((int)msg->wParam==0)
            emit appHide();
        else appShow();

        return true;
    }

    if(msg->message==WM_CloseProgram)
    {
        emit appToClose();
       // SetParent((HWND)this->winId(),parentHWND);
        close();

        return true;
    }


    //  TranslateMessage(&msg); // интерпретируем сообщения
    // DispatchMessage(&msg); // передаём сообщения обратно ОС

    //  }


    return false;
}

MainWindowForMessageReceive::~MainWindowForMessageReceive()
{
    delete ui;
}
