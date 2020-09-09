#ifndef MAINWINDOWFORMESSAGERECEIVE_H
#define MAINWINDOWFORMESSAGERECEIVE_H

#include <QMainWindow>
#include "headers/mainappwindow.h"

namespace Ui {
class MainWindowForMessageReceive;
}

class MainWindowForMessageReceive : public QMainWindow
{
    Q_OBJECT
private:
    const LPCSTR WM_NewMeshName= LPCSTR("WM_NewMesh_D3D-s");
    const LPCSTR WM_FilePathName  = LPCSTR("WM_FilePathName_D3D-s");

    UINT WM_NewMesh;
    UINT WM_FilePath ;


    //deep Integr start

    UINT WM_Integrate;  // wParam - parent HWND
    UINT WM_CloseProgram;
    UINT WM_ChangeSize;       //wParam - width lParam - high
    UINT WM_SwitchVisibility;  //wParam 0 - not visible 1 - visible

    const LPCSTR WM_IntegrateName= PCSTR("WM_Integrate_D3D-s");
    const LPCSTR WM_CloseProgramName =PCSTR("WM_CloseProgram_D3D-s");
    const LPCSTR WM_ChangeSizeName  =PCSTR("WM_ChangeSize_D3D-s");
    const LPCSTR WM_SwitchVisibilityName  =PCSTR("WM_SwitchVisibility_D3D-s");
    //deep integr end


    QString pathToFile;
public:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    explicit MainWindowForMessageReceive(QWidget *parent = nullptr);
    ~MainWindowForMessageReceive();
signals:

    void signalAppendMesh(QStringList meshesList, QString path);

    void signalClearMeshesData();

    void signalSetQuantity(int quantity);

    void appToClose();
    void appChangeSize(int x,int y,int width,int height);
    void appHide();
    void appShow();
    void appIntegrate(HWND app);






private:
    Ui::MainWindowForMessageReceive *ui;
};

#endif // MAINWINDOWFORMESSAGERECEIVE_H
