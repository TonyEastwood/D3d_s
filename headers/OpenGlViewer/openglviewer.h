#ifndef OPENGLVIEWER_H
#define OPENGLVIEWER_H


#include <QGLWidget>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include "headers/vcgUtils.h"
#include <math.h>
#include <QDebug>
#include <QFile>

namespace Ui {
class OpenGlViewer;
}

class OpenGlViewer : public QGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGlViewer(  QWidget *parent = nullptr);
    ~OpenGlViewer() override;

    void setFirstMesh(QString path);
    void setSecondMesh(QString path);

    void setLight(bool value);

    void saveFirstMesh();
    void saveSecondMesh();

    void alignSecondMesh(vcg::Matrix44d * resultTransformMatrix, bool * isVisible);
    void appendSecondMeshToFirst();

     void openAlignFile();

     void exportAsMLP();

 signals:
    void setDistanceInLabel(QString textDistance);
public slots:
    void setShowGrid(bool value);
    void setShowFaces(bool value);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;  // object roration
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event) override;  // object scale
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    QString vcgMatrixToString(const vcg::Matrix44d & resultTransformMatrix);



    void calculateNormalFirstObject();
    void calculateNormalSecondObject();

    void drawFirstMesh();
    void drawSecondMesh();

    void calcNormal(std::vector<std::tuple<float,float,float>> & normalArray, const float &Vx,const float &Vy,const float &Vz,const float &Sx,const float &Sy, const float &Sz);

    //temporary const
private:
    const uint COUNT_ALIGN_CYCLES=5;
    const double ERROR_ALIGN=0.0021f;

    const QColor BACKGROUND_COLOR=QColor(255,255,255); //White background

    const std::tuple<float,float,float> MESH1_FACES_COLOR={0.5f, 0.5f, 0.5f}; //color faces mesh1
    const std::tuple<float,float,float> MESH1_GRID_COLOR={1.0f, 0.5f, 0.2f}; // color grid mesh1

    const std::tuple<float,float,float> MESH2_FACES_COLOR={0.4f, 0.7f, 0.9f}; //color faces mesh2
    const std::tuple<float,float,float> MESH2_GRID_COLOR={1.0f, 0.0f, 0.0f}; // color grid mesh2


private:
    std::vector<QStringList> vectorContentMLP;

    MyMesh *  drawFirstObject;                     // object that need to draw
    MyMesh *  drawSecondObject;                     // object that need to draw

    QString identityMatrix="1 0 0 0 "
                           "0 1 0 0 "
                           "0 0 1 0 "
                           "0 0 0 1";

   // std::vector<std::tuple<float,float,float>> normalFirstMesh;
   // std::vector<std::tuple<float,float,float>> normalSecondMesh;

    float x_pos, y_pos, rotate_y, rotate_x;  // rotate values
    float translateX, translateY;
    float rotationSpeed=0.3f;
    float translateSpeed=0.3f;

    double ratioWidthHeight;

    float prevRotation_x=0;
    float prevRotation_y=0;
    float scaleSpeed=10;// current scale (zoom in\zoom out)

    bool cameraMove=false;

    Ui::OpenGlViewer *ui;

    GLfloat light_diffuse[3];
    GLfloat light_ambient[4];
    GLfloat light_position[4];

    int maxOrigin=-100000;  // max value of coords x/y/z
    int orthoCoefficient=50;
    int scaleWheel = 10000;

    bool isDrawGrid=false;
    bool isDrawFaces=true;
    bool isLight=false;

};

#endif  // OPENGLVIEWER_H
