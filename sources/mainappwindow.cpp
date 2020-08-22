#include "headers/mainappwindow.h"
#include "ui_mainappwindow.h"

#include <QDebug>
MainAppWindow::MainAppWindow(  QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainAppWindow)
{

    WM_NewMesh = RegisterWindowMessageA(WM_NewMeshName);
    WM_FilePath = RegisterWindowMessageA(WM_FilePathName);



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


    QMenuBar * menuBar = new QMenuBar(this);

    menuBar->addMenu(fileMenu);
    //menuBar->addMenu(functions);



    mainLayout = new QHBoxLayout();

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(openGlViewer,90);
    mainLayout->addLayout(Vbox,10);

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

    //connect(exportMlp, &QAction::triggered, this, &MainAppWindow::exportMlpFile);





    // getMessageCustom(this);
    // t1.detach();
}

bool MainAppWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{

    MSG *msg = static_cast<MSG*>(message);
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

        emit signalClearMeshesData();

        return true;
    }


    //  TranslateMessage(&msg); // интерпретируем сообщения
    // DispatchMessage(&msg); // передаём сообщения обратно ОС

    //  }


    return false;
}

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

    MSG msg;



    if(GetMessage(&msg, NULL, NULL, NULL)>0){ // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        if(msg.message==WM_NewMesh)
        {
            QStringList stringList;
            if(!pathToFile.isEmpty())
            {
                QFile file(pathToFile);
                QByteArray data;
                if(file.open(QIODevice::ReadOnly))
                {
                    while(!file.atEnd())
                    {
                        stringList.append(QString::fromStdString(file.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0]);
                    }

                }
                file.close();
                emit infoDisplay("MessageParse start");
                for(auto &i:stringList)
                    emit infoDisplay(i);
                emit infoDisplay("MessageParse end");
                //   qDebug()<<data;
            }
        }

        else if(msg.message==WM_FilePath)
        {
            QSettings m("HKEY_CURRENT_USER\\Software\\D3D-s\\AlignLab",QSettings::Registry64Format);
            // qDebug()<<GetRegistryValueString(HKEY_CURRENT_USER, 'Software\D3D-s\AlignLab',  'PathToMeshList');
            QString val = m.value("PathToMeshList").toString();
            pathToFile=val;
            emit infoDisplay(val.toLocal8Bit());
        }
     

        //  TranslateMessage(&msg); // интерпретируем сообщения
        // DispatchMessage(&msg); // передаём сообщения обратно ОС

    }

   


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


