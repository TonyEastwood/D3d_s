#include "headers/mainappwindow.h"
#include "ui_mainappwindow.h"

#include <QDebug>
MainAppWindow::MainAppWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainAppWindow)
{


}

void MainAppWindow::Initialize(bool isIntegrate)
{
    //    WM_NewMesh = RegisterWindowMessageA(WM_NewMeshName);
    //    WM_FilePath = RegisterWindowMessageA(WM_FilePathName);


    //   WM_Integrate= RegisterWindowMessageA(WM_IntegrateName);  // wParam - parent HWND
    //    WM_CloseProgram= RegisterWindowMessageA(WM_CloseProgramName);
    //    WM_ChangeSize= RegisterWindowMessageA(WM_ChangeSizeName);       //wParam - width lParam - high
    //    WM_SwitchVisibility= RegisterWindowMessageA(WM_SwitchVisibilityName);  //wParam 0 - not visible 1 - visible



    // this->setStyleSheet("QMenu::item { background-color: #262527; selection-color: white; } ");
    //this->setStyleSheet("QMenu  { background-color: #262527; selection-color: white; border: 1px solid black;} ");
    // this->setStyleSheet("QMenu::item:selected { background-color: green; } ");
    //  this


    // this->setStyleSheet("color:"+CONSTANTS::textColorMenu+";  background-color:"+CONSTANTS::backgroundColor+"; ");
    QMenu *fileMenu = new QMenu(tr("&File"), this);

    // fileMenu->setStyleSheet(" QMenu::title { background-color: red;} ");
    // fileMenu->setStyleSheet(" QMenu::item {   selection-background-color:"+CONSTANTS::selectedMenuItemColor+";color:"+CONSTANTS::textColorMenu+";} ");

    // fileMenu->setStyleSheet("");


    QAction *  openFirstMesh = fileMenu->addAction(tr("Add mesh"));
    // QAction *  openSecondMesh = fileMenu->addAction(tr("Open second mesh"));
    fileMenu->addSeparator();
    QAction *  saveFirstMesh = fileMenu->addAction(tr("Save first mesh"));
    // QAction *  saveSecondMesh = fileMenu->addAction(tr("Save second mesh"));
    fileMenu->addSeparator();
    QAction *  openAlignFile = fileMenu->addAction(tr("Load mesh file to align"));
    fileMenu->addSeparator();
    // QAction *  exportMlp = fileMenu->addAction(tr("Export mlp file"));
    // fileMenu->addSeparator();
    QAction *  exitAction = fileMenu->addAction(tr("E&xit"));

    QFont font;
    font.setPointSize(10);


    QLabel * drawGrid=new QLabel("Enable Grid");
    QLabel * drawFace=new QLabel("Enable Faces");
    // QLabel * lightLabel=new QLabel("Enable Light");

    QLabel * distanceLabel=new QLabel("");

    drawGrid->setFont(font);
    drawFace->setFont(font);
    // lightLabel->setFont(font);

    QPushButton * buttonAlign=new QPushButton("Align pair");
    QPushButton * buttonMerge=new QPushButton("Merge objects");
    QPushButton * clearMeshes=new QPushButton("Clear meshes");

    QCheckBox * isDrawGrid=new QCheckBox();
    QCheckBox * isFaceGrid=new QCheckBox();
    QCheckBox * isLightOn=new QCheckBox();

    isDrawGrid->setChecked(false);
    isFaceGrid->setChecked(true);
    isLightOn->setChecked(false);


    QHBoxLayout * horizLay=new QHBoxLayout();
    QHBoxLayout * horizLay2=new QHBoxLayout();
    QHBoxLayout * horizLay3=new QHBoxLayout();

    horizLay->addWidget(drawGrid);
    horizLay->addWidget(isDrawGrid);


    horizLay2->addWidget(drawFace);
    horizLay2->addWidget(isFaceGrid);

    // horizLay3->addWidget(lightLabel);
    // horizLay3->addWidget(isLightOn);

    horizLay->setContentsMargins(0,0,5,0);
    horizLay->setSpacing(5);
    horizLay->setMargin(0);
    horizLay->setStretch(0,0);

    horizLay2->setContentsMargins(0,0,5,0);
    horizLay2->setSpacing(5);
    horizLay2->setMargin(0);
    horizLay2->setStretch(0,0);

    horizLay3->setContentsMargins(0,0,5,0);
    horizLay3->setSpacing(5);
    horizLay3->setMargin(0);
    horizLay3->setStretch(0,0);

    drawGrid->setAlignment(Qt::AlignTop);
    drawFace->setAlignment(Qt::AlignBottom);
    // lightLabel->setAlignment(Qt::AlignTop);


    //     isFaceGrid->setAlignment(Qt::AlignTop);

    QVBoxLayout * Vbox=new QVBoxLayout(this);

    Vbox->setContentsMargins(0,0,0,0);
    Vbox->setSpacing(0);
    Vbox->setMargin(0);
    Vbox->setStretch(0,0);

    horizLay->setAlignment(Qt::AlignTop);
    horizLay2->setAlignment(Qt::AlignTop);
    horizLay3->setAlignment(Qt::AlignTop);

    Vbox->addLayout(horizLay,1);
    Vbox->addLayout(horizLay2,7);
    Vbox->addLayout(horizLay3,400);
    Vbox->addWidget(buttonAlign,1000,Qt::AlignTop);
    Vbox->addWidget(buttonMerge,5000,Qt::AlignTop);
    Vbox->addWidget(clearMeshes,40000,Qt::AlignTop);
    Vbox->addWidget(distanceLabel,70000,Qt::AlignTop);







    ui->setupUi(this);
    openGlViewer = new OpenGlViewer();
    openGlViewer->setFocusPolicy(Qt::FocusPolicy::StrongFocus);






    //menuBar->addMenu(functions);



    mainLayout = new QHBoxLayout();
    //   mainLayout->addWidget(openGlViewer,90);
    this->setContentsMargins(0,0,0,0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(openGlViewer,90);
    mainLayout->layout()->setSpacing(0);



    QVBoxLayout * VboxIntegrate=new QVBoxLayout(this);
    progress=new QProgressBar(this);
    labelScanning=new QLabel("Scanning in process...");
    buttonCancel=new QPushButton("Cancel");

    progress->setAlignment(Qt::AlignCenter);

    buttonCancel->setFixedSize(100,30);



    hideCustomProgressBar();
   // buttonCancel->resize(buttonCancel->width()/2,buttonCancel->height());



    VboxIntegrate->setContentsMargins(0,0,0,0);
    VboxIntegrate->setSpacing(0);
    VboxIntegrate->setMargin(10);
    VboxIntegrate->setStretch(0,0);

    VboxIntegrate->addWidget(labelScanning,0,Qt::AlignTop);
    VboxIntegrate->addWidget(progress,1,Qt::AlignTop);
    VboxIntegrate->addWidget(buttonCancel,30,Qt::AlignTop);
    VboxIntegrate->setSizeConstraint(QLayout::SetFixedSize);



    if(!isIntegrate)
    {
        QMenuBar * menuBar = new QMenuBar(this);
        menuBar->addMenu(fileMenu);
        mainLayout->setMenuBar(menuBar);
        mainLayout->addLayout(Vbox,10);
    }
    else{
        mainLayout->addLayout(VboxIntegrate);
    }

    ui->centralwidget->setContentsMargins(0,0,0,0);
    statusBar()->hide();

    //ui->centralwidget->layout()->setSpacing(0);
    ui->centralwidget->setLayout(mainLayout);
    ui->centralwidget->show();

    //    getMessageCustom

    //  timer=new QTimer(this);


    // timer->start(100);

    connect(this, &MainAppWindow::infoDisplay, this, &MainAppWindow::coutInfo);

    connect(buttonMerge, &QPushButton::clicked, this, &MainAppWindow::appendSecondToFirst);
    connect(clearMeshes, &QPushButton::clicked, this, &MainAppWindow::clearMeshesOpengl);
    connect(buttonAlign, &QPushButton::clicked, this, &MainAppWindow::alignSecondMesh);
    //connect(buttonAlign, &QPushButton::clicked, this, &MainAppWindow::getMessageCustom);

    connect(isFaceGrid, &QCheckBox::toggled, this, &MainAppWindow::setDrawFaces);
    connect(isDrawGrid, &QCheckBox::toggled, this, &MainAppWindow::setDrawGrid);
    connect(isLightOn, &QCheckBox::toggled, this, &MainAppWindow::setLight);
    connect(openFirstMesh, &QAction::triggered, this, &MainAppWindow::addMesh);
    //  connect(openSecondMesh, &QAction::triggered, this, &MainAppWindow::setSecondOpenglMesh);
    connect(openAlignFile, &QAction::triggered, this, &MainAppWindow::openAlignFile);
    // connect(saveSecondMesh, &QAction::triggered, this, &MainAppWindow::saveSecondMesh);
    connect(saveFirstMesh, &QAction::triggered, this, &MainAppWindow::saveFirstMesh);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(openGlViewer, &OpenGlViewer::setDistanceInLabel, distanceLabel, &QLabel::setText);

    connect(this,&MainAppWindow::signalAppendMesh,openGlViewer,  &OpenGlViewer::addedMeshesToAlign, Qt::ConnectionType::QueuedConnection);
    connect(this,&MainAppWindow::signalClearMeshesData,openGlViewer,  &OpenGlViewer::clearMeshes, Qt::ConnectionType::QueuedConnection);

    // connect(&progress,&QProgressDialog::canceled,this, &MainAppWindow::cancelScanning );
    connect(buttonCancel,&QPushButton::clicked,this, &MainAppWindow::cancelScanning );

    progress->setMinimum(1);
    progress->setMaximum(2);
    progress->setValue(2);
    // progress->setAutoClose(true);
    //progress->setAutoReset(true);


    //connect(exportMlp, &QAction::triggered, this, &MainAppWindow::exportMlpFile);





    // getMessageCustom(this);
    // t1.detach();
}

//bool MainAppWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
//{


//    MSG *msg = static_cast<MSG*>(message);
//    if(msg->message==WM_Integrate)
//    {
//        HWND hwndNewParent=(HWND) msg->wParam;
//        this->setStyleSheet("border: none;");
//        this->setGeometry(0,0,this->width(),this->height());
//        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//        this->show();

//        parentHWND=GetParent((HWND)this->winId());



//        if(hwndNewParent)
//        {
//            SetParent((HWND)this->winId(),hwndNewParent);
//        }


//        return true;
//    }
//    /*    qDebug()<<"Message=="<<msg->message;
//    //if(GetMessage(&msg, NULL, NULL, NULL)>0){ // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
//    if(msg->message==WM_NewMesh)
//    {
//        QStringList stringList;
//        if(!pathToFile.isEmpty())
//        {
//            QFile file(pathToFile);
//            QByteArray data;
//            if(file.open(QIODevice::ReadOnly))
//            {
//                while(!file.atEnd())
//                    stringList.append(QString::fromStdString(file.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0]);
//            }
//            file.close();

//            //emit infoDisplay("MessageParse start");
//            // for(auto &i:stringList)
//            //   emit infoDisplay(i);
//            //emit infoDisplay("MessageParse end");
//            //   qDebug()<<data;
//            emit signalAppendMesh(stringList,pathToFile);
//        }
//        return true;
//    }

//    if(msg->message==WM_FilePath)
//    {
//        QSettings m("HKEY_CURRENT_USER\\Software\\D3D-s\\AlignLab",QSettings::Registry64Format);
//        // qDebug()<<GetRegistryValueString(HKEY_CURRENT_USER, 'Software\D3D-s\AlignLab',  'PathToMeshList');
//        QString val = m.value("PathToMeshList").toString();
//        pathToFile=val;
//        // emit infoDisplay(val.toLocal8Bit());

//        emit signalClearMeshesData();

//        return true;
//    }*/



//    /* if(msg->message==WM_ChangeSize)
//    {
//        this->setGeometry(0,0,(int)msg->wParam,(int)msg->lParam);
//        return true;
//    }
//    if(msg->message==WM_SwitchVisibility)
//    {
//        if((int)msg->wParam==0)
//            hide();
//        else show();

//        return true;
//    }

//    if(msg->message==WM_CloseProgram)
//    {
//        SetParent((HWND)this->winId(),parentHWND);
//        close();

//        return true;
//    }


//    //  TranslateMessage(&msg); // интерпретируем сообщения
//    // DispatchMessage(&msg); // передаём сообщения обратно ОС

//    //  }*/


//    return false;
//}

MainAppWindow::~MainAppWindow()
{
    delete mainLayout;
    delete openGlViewer;
    delete ui;
}

void MainAppWindow::addMesh()
{
    QString path=QFileDialog::getOpenFileName(this,"Open object",QString(),tr("STL (*.stl);;PLY (*.ply)" ));

    if(path.isEmpty())
        return;
    QFileInfo file1(path);

    if(file1.completeSuffix()!="ply" && file1.completeSuffix()!="stl")
    {
        QMessageBox::warning(this, "Warning extension","Please select object with another format");
        return;

    }
    openGlViewer->addMesh(path);



}

void MainAppWindow::cancelScanning()
{
    qDebug()<<"Cancel scanning";
    emit signalCancelScanning();
}

//void MainAppWindow::setSecondOpenglMesh()
//{
//    QString path=QFileDialog::getOpenFileName(this,"Open object",QString(),tr("STL (*.stl);;PLY (*.ply)" ));
//    if(path.isEmpty())
//        return;
//    QFileInfo file1(path);

//    if(file1.completeSuffix()!="ply" && file1.completeSuffix()!="stl")
//    {
//        QMessageBox::warning(this, "Warning extension","Please select object with another format");
//        return;
//    }
//    openGlViewer->setSecondMesh(path);
//}


void MainAppWindow::setDrawGrid(bool value)
{
    openGlViewer->setShowGrid(value);
}

void MainAppWindow::setDrawFaces(bool value)
{
    openGlViewer->setShowFaces(value);
}

void MainAppWindow::setLight(bool value)
{
    openGlViewer->setLight(value);
}

void MainAppWindow::saveFirstMesh()
{
    openGlViewer->saveFirstMesh();
}

//void MainAppWindow::saveSecondMesh()
//{
//    openGlViewer->saveSecondMesh();
//}

void MainAppWindow::alignSecondMesh()
{
    openGlViewer->alignSecondMesh(nullptr,nullptr,nullptr,nullptr);
}

void MainAppWindow::appendSecondToFirst()
{
    openGlViewer->appendSecondMeshToFirst();
}

void MainAppWindow::openAlignFile()
{
    openGlViewer->openAlignFile();
}

void MainAppWindow::exportMlpFile()
{
    //openGlViewer->exportAsMLP();
}

void MainAppWindow::getMessageCustom()
{

    //    MSG msg;



    //    if(GetMessage(&msg, NULL, NULL, NULL)>0){ // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
    //        if(msg.message==WM_NewMesh)
    //        {
    //            QStringList stringList;
    //            if(!pathToFile.isEmpty())
    //            {
    //                QFile file(pathToFile);
    //                QByteArray data;
    //                if(file.open(QIODevice::ReadOnly))
    //                {
    //                    while(!file.atEnd())
    //                    {
    //                        stringList.append(QString::fromStdString(file.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0]);
    //                    }

    //                }
    //                file.close();
    //                emit infoDisplay("MessageParse start");
    //                for(auto &i:stringList)
    //                    emit infoDisplay(i);
    //                emit infoDisplay("MessageParse end");
    //                //   qDebug()<<data;
    //            }
    //        }

    //        else if(msg.message==WM_FilePath)
    //        {
    //            QSettings m("HKEY_CURRENT_USER\\Software\\D3D-s\\AlignLab",QSettings::Registry64Format);
    //            // qDebug()<<GetRegistryValueString(HKEY_CURRENT_USER, 'Software\D3D-s\AlignLab',  'PathToMeshList');
    //            QString val = m.value("PathToMeshList").toString();
    //            pathToFile=val;
    //            emit infoDisplay(val.toLocal8Bit());
    //        }


    //  TranslateMessage(&msg); // интерпретируем сообщения
    // DispatchMessage(&msg); // передаём сообщения обратно ОС

    //  }




}

void MainAppWindow::coutInfo(const QString & info)
{
    qDebug()<<info;
    // std::cout<<info.toStdString()<<std::endl;
}

void MainAppWindow::clearMeshesOpengl()
{
    openGlViewer->clearMeshes();
}

void MainAppWindow::appToClose()
{
    if(parentHWND)
        SetParent((HWND)this->winId(),parentHWND);
    close();
}

void MainAppWindow::appChangeSize(int x, int y, int width, int height)
{
    this->setGeometry(x,y,width,height);
}

void MainAppWindow::appIntegrate(HWND app)
{

    this->setStyleSheet("border: none;");
    this->setGeometry(0,0,this->width(),this->height());
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->show();

    parentHWND=GetParent((HWND)this->winId());

    if(app)
    {
        SetParent((HWND)this->winId(),app);
    }
}

void MainAppWindow::setMaxValue(int value)
{
    progress->setMaximum(value);
}

void MainAppWindow::setCurrentValue(int value)
{
    progress->setValue(value);
    // progress->setLabelText("Scanning in progress...("+QString::number(value)+"/"+QString::number(progress.maximum())+")");
    if(value>=progress->maximum())
    {
        //progress->hide();
        hideCustomProgressBar();
        progress->setValue(1);
    }else showCustomProgressBar();//progress->show();
}

void MainAppWindow::showProgressBar()
{
    //   progress->setLabelText("Scanning in progress...("+QString::number(progress.value())+"/"+QString::number(progress.maximum())+")");
    progress->setMinimum(0);
    //  progress->setWindowModality(Qt::WindowModal);
    hideCustomProgressBar();
    // progress->hide();
}

void MainAppWindow::hideCustomProgressBar()
{
    labelScanning->setVisible(false);
    //buttonCancel->setVisible(false);
    progress->setVisible(false);
}

void MainAppWindow::showCustomProgressBar()
{
    labelScanning->setVisible(true);
   // buttonCancel->setVisible(true);
    progress->setVisible(true);
}


