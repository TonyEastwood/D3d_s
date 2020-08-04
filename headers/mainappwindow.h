#ifndef MAINAPPWINDOW_H
#define MAINAPPWINDOW_H

#include <QMainWindow>
#include "OpenGlViewer/openglviewer.h"
#include "Design.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QLabel>
#include <QMenu>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QStyleFactory>
#include "customtable.h"
namespace Ui {
class MainAppWindow;
}

class MainAppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainAppWindow(  QWidget *parent = nullptr);
    ~MainAppWindow();
public slots:
   // void setFirstOpenglMesh();
    void importMesh();
    //void setSecondOpenglMesh();


    void setDrawGrid(bool value);
    void setDrawFaces(bool value);

    void setLight(bool value);

    void exportMesh();
 //   void saveSecondMesh();

    void alignSecondMesh();
    void appendSecondToFirst();

    void openAlignFile();
private:
    void initAlignWindow();



    Ui::MainAppWindow *ui;
    OpenGlViewer *openGlViewer; // openGl viewer
    QHBoxLayout *mainLayout;
    CustomTable * objectViewer;


};

#endif // MAINAPPWINDOW_H
