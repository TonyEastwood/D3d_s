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
#include <GL/GLU.h>
//#include <GLFW/glfw3.h>
//#include <GL/glfw3.h>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QElapsedTimer >
namespace Ui {
class OpenGlViewer;
}

class OpenGlViewer : public QGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGlViewer(  QWidget *parent = nullptr);
    ~OpenGlViewer() override;

    bool addMesh(QString path,bool isNeedToDraw=true);
    // bool setSecondMesh(QString path,bool isNeedToDraw=true);

    void setLight(bool value);

    void saveFirstMesh();
    //void saveSecondMesh();

    void alignSecondMesh(MyMesh * firstMesh, MyMesh * secondMesh, vcg::Matrix44d * resultTransformMatrix, bool * isVisible);
    void appendSecondMeshToFirst(MyMesh * firstMesh=nullptr, MyMesh * secondMesh=nullptr);

    void openAlignFile();

    void exportAsMLP(QString path);

signals:
    void setDistanceInLabel(QString textDistance);
public slots:
    void setShowGrid(bool value);
    void setShowFaces(bool value);


    void addedMeshesToAlign(QStringList meshesList, QString path);
    void clearMeshes();

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
    void findMinMaxForStl(MyMesh * _object);


    QString vcgMatrixToString(const vcg::Matrix44d & resultTransformMatrix);


    void InitMaxOrigin();

    void updateDrawVertex();

    void drawFirstMesh();
    void drawSecondMesh();

    void clearMeshesVector();

    //  void drawTestCube();
    //temporary const
private:
    QElapsedTimer  * timer;

    const uint COUNT_ALIGN_CYCLES=1;
    const double ERROR_ALIGN=0.006f;

    std::vector<QString> WRvectorFileNames;
    std::vector<vcg::Matrix44d> WRvectorMatrix;
    std::vector<bool> WRvectorVisible;


    const GLclampf BACKGROUND_COLOR[3]={0.709,0.588, 0.921}; //White background

    const GLclampf BACKGROUND_GRADIENT_TOP[3]={0,0, 0}; // background gradient top
    const GLclampf BACKGROUND_GRADIENT_BOT[3]={0.44,0.44,0.88}; // background gradient bottom

    const std::tuple<float,float,float> MESH1_FACES_COLOR={0.5f, 0.5f, 0.5f}; //color faces mesh1
    const std::tuple<float,float,float> MESH1_GRID_COLOR={1.0f, 0.5f, 0.2f}; // color grid mesh1

    const std::tuple<float,float,float> MESH2_FACES_COLOR={0.4f, 0.7f, 0.9f}; //color faces mesh2
    const std::tuple<float,float,float> MESH2_GRID_COLOR={1.0f, 0.0f, 0.0f}; // color grid mesh2

    const QString identityMatrix="1 0 0 0 "
                                 "0 1 0 0 "
                                 "0 0 1 0 "
                                 "0 0 0 1";


private:
    GLfloat * drawVertex;   //vertex that contain Vertex shader
    uint sizeDrawVertex=0;  //size all of vertex that need to draw


    GLint GPUobjectColor;
    GLint GPUtransformMatrix;
    GLint GPUprojectionMatrix;
    GLint GPUlightColor;

    GLint GPUtopcolor;
    GLint GPUbotcolor;

    GLint GPUlightPosition;
    GLint GPUlightPosition2;

    GLuint VBO, VAO;
    GLuint background_vao =0;

    GLuint shaderProgram;

    GLuint shaderProgramBackground;

    QOpenGLExtraFunctions *f;


    QString distanceInfo;

    QVector2D mousePressPosition;       //save mouse position
    // QVector3D rotationAxis;

    QQuaternion rotation;
    QMatrix4x4 m_transform;
    QMatrix4x4 m_projection;
    qreal aspect;

    std::vector<QStringList> vectorContentMLP;

    QMatrix4x4 matrix;

    float minMaxXYZ[6];//minX maxX minY maxY minZ maxZ


    std::vector<MyMesh*> meshes;
    //    MyMesh *  drawFirstObject;                     // object that need to draw
    //    MyMesh *  drawSecondObject;                     // object that need to draw

    float x_pos, y_pos, rotate_y, rotate_x;  // rotate values
    float translateX, translateY;
    float rotationSpeed=3.0f;
    float translateSpeed=0.3f;

    double ratioWidthHeight;

    float scaleSpeed=0.001;// current scale (zoom in\zoom out)

    bool cameraMove=false;

    Ui::OpenGlViewer *ui;

    GLfloat light_diffuse[3];
    GLfloat light_ambient[4];
    GLfloat light_position[4];

    GLfloat light_position2[4];

    int maxOrigin=-100000;  // max value of coords x/y/z
    int orthoCoefficient=3;
    float scaleWheel = 25;

    bool isDrawGrid=false;
    bool isDrawFaces=true;
    bool isLight=false;

};

#endif  // OPENGLVIEWER_H
