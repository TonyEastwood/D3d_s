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
#include <QSettings>
#include <QDebug>
namespace Ui {
class MainAppWindow;
}

class MainAppWindow : public QMainWindow
{
    Q_OBJECT

private:
    const LPCSTR WM_NewMeshName= LPCSTR("WM_NewMesh_D3D-s");
    const LPCSTR WM_FilePathName  = LPCSTR("WM_FilePathName_D3D-s");

    UINT WM_NewMesh;
    UINT WM_FilePath ;

    QString pathToFile;

public:
    explicit MainAppWindow(  QWidget *parent = nullptr);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    ~MainAppWindow();
public slots:
    void addMesh();
    //void setSecondOpenglMesh();


    void setDrawGrid(bool value);
    void setDrawFaces(bool value);

    void setLight(bool value);

    void saveFirstMesh();
   // void saveSecondMesh();

    void alignSecondMesh();
    void appendSecondToFirst();

    void openAlignFile();

    void exportMlpFile();

    void getMessageCustom();

    public slots:
    void coutInfo(const QString & info);
signals:
    void infoDisplay(const QString &result);

    void signalAppendMesh(QStringList meshesList, QString path);

    void signalClearMeshesData();

private:
    void initAlignWindow();

    Ui::MainAppWindow *ui;
    OpenGlViewer *openGlViewer; // openGl viewer
    QHBoxLayout *mainLayout;

};

#endif // MAINAPPWINDOW_H
