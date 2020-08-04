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
#include <math.h>
#include <QDebug>
#include <QFile>
#include "../vcgapifunctions.h"
#include "../object3d.h"
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

    void alignSecondMesh();
    void appendSecondMeshToFirst();

    void openAlignFile();

    void importMesh(const QString &path);

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

private:

    void drawMeshes(bool isDrawGrid);// True - Grid False - Face

    //temporary const
private:
 //   const uint COUNT_ALIGN_CYCLES=5;
  //  const double ERROR_ALIGN=0.021f;

private:
    std::vector<Object3d> meshes;

   // std::vector<std::tuple<float,float,float>> normalFirstMesh;
   // std::vector<std::tuple<float,float,float>> normalSecondMesh;

    float x_pos, y_pos, rotate_y, rotate_x;  // rotate values
    float prevRotation_x=0;
    float prevRotation_y=0;
    float scaleSpeed=10;// current scale (zoom in\zoom out)

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
