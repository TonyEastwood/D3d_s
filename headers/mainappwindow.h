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
//#include <QProgressDialog>
#include <QProgressBar>
namespace Ui {
class MainAppWindow;
}

class MainAppWindow : public QMainWindow
{
    Q_OBJECT

private:

    bool isExternalClose;
    // const LPCSTR WM_NewMeshName= LPCSTR("WM_NewMesh_D3D-s");
    // const LPCSTR WM_FilePathName  = LPCSTR("WM_FilePathName_D3D-s");

    // UINT WM_NewMesh;
    // UINT WM_FilePath ;


    //deep Integr start
    HWND parentHWND;

    QProgressBar * progress;
    QLabel * labelScanning;
    QPushButton * buttonCancel;
   // QProgressDialog progress;

    // UINT WM_Integrate;  // wParam - parent HWND
    // UINT WM_CloseProgram;
    // UINT WM_ChangeSize;       //wParam - width lParam - high
    // UINT WM_SwitchVisibility;  //wParam 0 - not visible 1 - visible

    //const LPCSTR WM_IntegrateName= PCSTR("WM_Integrate_D3D-s");
    // const LPCSTR WM_CloseProgramName =PCSTR("WM_CloseProgram_D3D-s");
    // const LPCSTR WM_ChangeSizeName  =PCSTR("WM_ChangeSize_D3D-s");
    //const LPCSTR WM_SwitchVisibilityName  =PCSTR("WM_SwitchVisibility_D3D-s");
    //deep integr end


    //QString pathToFile;
protected:
    void showEvent(QShowEvent *ev) override;
    void hideEvent(QHideEvent *event) override;
public:
    explicit MainAppWindow(QWidget *parent = nullptr);

    // bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    void Initialize(bool isIntegrate);
    ~MainAppWindow();
public slots:
    void addMesh();
    //void setSecondOpenglMesh();

    void cancelScanning();

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

    void coutInfo(const QString & info);

    void clearMeshesOpengl();

    void appToClose();
    void appChangeSize(int x,int y,int width,int height);
    void appIntegrate(HWND app);

    //progress bar slots
    void setMaxValue(int value);
    void setCurrentValue(int value);
    void showProgressBar();

    void sendLineRotationAxis(float x1, float y1, float z1, float x2, float y2, float z2);
signals:
    void infoDisplay(const QString &result);

    void signalAppendMesh(QStringList meshesList, QString path);

    void signalClearMeshesData();

    void signalCancelScanning();

    void signalOnClose();

private:
    void initAlignWindow();
    void hideCustomProgressBar();
    void showCustomProgressBar();

    void closeEvent (QCloseEvent *event) override;

    Ui::MainAppWindow *ui;
    OpenGlViewer *openGlViewer; // openGl viewer
    QHBoxLayout *mainLayout;

};

#endif // MAINAPPWINDOW_H
