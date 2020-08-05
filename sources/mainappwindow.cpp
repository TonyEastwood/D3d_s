#include "headers/mainappwindow.h"
#include "ui_mainappwindow.h"

#include <QDebug>
MainAppWindow::MainAppWindow(  QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainAppWindow)
{


    // this->setStyleSheet("QMenu::item { background-color: #262527; selection-color: white; } ");
    //this->setStyleSheet("QMenu  { background-color: #262527; selection-color: white; border: 1px solid black;} ");
    // this->setStyleSheet("QMenu::item:selected { background-color: green; } ");
    //  this


    // this->setStyleSheet("color:"+CONSTANTS::textColorMenu+";  background-color:"+CONSTANTS::backgroundColor+"; ");
    QMenu *fileMenu = new QMenu(tr("&File"), this);

    // fileMenu->setStyleSheet(" QMenu::title { background-color: red;} ");
    // fileMenu->setStyleSheet(" QMenu::item {   selection-background-color:"+CONSTANTS::selectedMenuItemColor+";color:"+CONSTANTS::textColorMenu+";} ");

    // fileMenu->setStyleSheet("");


    QAction *  openFirstMesh = fileMenu->addAction(tr("Open first mesh"));
    QAction *  openSecondMesh = fileMenu->addAction(tr("Open second mesh"));
    fileMenu->addSeparator();
    QAction *  saveFirstMesh = fileMenu->addAction(tr("Save first mesh"));
    QAction *  saveSecondMesh = fileMenu->addAction(tr("Save second mesh"));
    fileMenu->addSeparator();
    QAction *  openAlignFile = fileMenu->addAction(tr("Load align file"));
    fileMenu->addSeparator();
    QAction *  exportMlp = fileMenu->addAction(tr("export MLP file"));
    fileMenu->addSeparator();
    QAction *  exitAction = fileMenu->addAction(tr("E&xit"));

    QFont font;
    font.setPointSize(10);


    QLabel * drawGrid=new QLabel("Enable Grid");
    QLabel * drawFace=new QLabel("Enable Faces");
    QLabel * lightLabel=new QLabel("Enable Light");

    QLabel * distanceLabel=new QLabel("");

    drawGrid->setFont(font);
    drawFace->setFont(font);
    lightLabel->setFont(font);

    QPushButton * buttonAlign=new QPushButton("Align pair");
    QPushButton * buttonMerge=new QPushButton("Merge objects");

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

    horizLay3->addWidget(lightLabel);
    horizLay3->addWidget(isLightOn);

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
    lightLabel->setAlignment(Qt::AlignTop);


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
    Vbox->addWidget(distanceLabel,10000,Qt::AlignTop);






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

    connect(buttonMerge, &QPushButton::clicked, this, &MainAppWindow::appendSecondToFirst);
    connect(buttonAlign, &QPushButton::clicked, this, &MainAppWindow::alignSecondMesh);
    connect(isFaceGrid, &QCheckBox::toggled, this, &MainAppWindow::setDrawFaces);
    connect(isDrawGrid, &QCheckBox::toggled, this, &MainAppWindow::setDrawGrid);
    connect(isLightOn, &QCheckBox::toggled, this, &MainAppWindow::setLight);
    connect(openFirstMesh, &QAction::triggered, this, &MainAppWindow::setFirstOpenglMesh);
    connect(openSecondMesh, &QAction::triggered, this, &MainAppWindow::setSecondOpenglMesh);
    connect(openAlignFile, &QAction::triggered, this, &MainAppWindow::openAlignFile);
    connect(saveSecondMesh, &QAction::triggered, this, &MainAppWindow::saveSecondMesh);
    connect(saveFirstMesh, &QAction::triggered, this, &MainAppWindow::saveFirstMesh);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(openGlViewer, &OpenGlViewer::setDistanceInLabel, distanceLabel, &QLabel::setText);
    connect(exportMlp, &QAction::triggered, this, &MainAppWindow::exportMlpFile);

}

MainAppWindow::~MainAppWindow()
{
    delete mainLayout;
    delete openGlViewer;
    delete ui;
}

void MainAppWindow::setFirstOpenglMesh()
{
    QString path=QFileDialog::getOpenFileName(this,"Open object","C://",tr("STL (*.stl);;PLY (*.ply)" ));

    if(path.isEmpty())
        return;
    QFileInfo file1(path);

    if(file1.completeSuffix()!="ply" && file1.completeSuffix()!="stl")
    {
        QMessageBox::warning(this, "Warning extension","Please select object with another format");
        return;

    }
    openGlViewer->setFirstMesh(path);



}

void MainAppWindow::setSecondOpenglMesh()
{
    QString path=QFileDialog::getOpenFileName(this,"Open object","C://",tr("STL (*.stl);;PLY (*.ply)" ));
    if(path.isEmpty())
        return;
    QFileInfo file1(path);

    if(file1.completeSuffix()!="ply" && file1.completeSuffix()!="stl")
    {
        QMessageBox::warning(this, "Warning extension","Please select object with another format");
        return;
    }
    openGlViewer->setSecondMesh(path);
}


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

void MainAppWindow::saveSecondMesh()
{
    openGlViewer->saveSecondMesh();
}

void MainAppWindow::alignSecondMesh()
{
    openGlViewer->alignSecondMesh(nullptr,nullptr);
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
    openGlViewer->exportAsMLP();
}


