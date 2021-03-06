#include "../../headers/OpenGlViewer/openglviewer.h"
#include "ui_openglviewer.h"
std::vector<vcg::Point3d>* vcg::PointMatchingScale::fix;
std::vector<vcg::Point3d>* vcg::PointMatchingScale::mov;
vcg::Box3d vcg::PointMatchingScale::b;

OpenGlViewer::OpenGlViewer( QWidget *parent)
    : QGLWidget(parent) {

    ui->setupUi(this);

    drawFirstObject=new MyMesh();
    drawFirstObject->fn=0;

    drawSecondObject=new MyMesh();
    drawSecondObject->fn=0;

    setFormat(QGLFormat(QGL::DoubleBuffer));  // double buff

    light_diffuse[0]=0.7;
    light_diffuse[1]=0.7;
    light_diffuse[2]=0.7;

    light_ambient[0]=0.5;
    light_ambient[1]=0.5;
    light_ambient[2]=0.5;
    light_ambient[3]=0.4;

}

OpenGlViewer::~OpenGlViewer() {
    delete drawFirstObject;
    delete drawSecondObject;
    delete ui;
}

void OpenGlViewer::initializeGL() {

    initializeOpenGLFunctions();
    glDepthFunc(GL_LEQUAL);   // buff deep
    qglClearColor(Qt::white);  // set background

    glEnable(GL_DEPTH_TEST);  // line that we can't see - become invisible
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    //to enable



}
void OpenGlViewer::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)w, (GLint)w);
}

void OpenGlViewer::paintGL() {
    if(!drawFirstObject)
        return;
    //  timerForTest->restart();

    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);  // clear buff image and deep

    glMatrixMode(GL_PROJECTION);   // set the matrix
    glShadeModel(GL_SMOOTH);
    glLoadIdentity();  // load matrix

    glOrtho(-orthoCoefficient * maxOrigin, orthoCoefficient * maxOrigin,
            orthoCoefficient * maxOrigin,-orthoCoefficient * maxOrigin,
            - orthoCoefficient * maxOrigin*5,
            orthoCoefficient * maxOrigin*5);  // set matrix scope. Need get opportunity to

    if(isLight)
    {
        glEnable(GL_LIGHTING);
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        // двухсторонний расчет освещения

        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    }

    // DRAW LINES START
    glPushMatrix(); // save the current matrix

    glRotatef( rotate_x, 1.0, 0.0, 0.0);  // rotate x
    glRotatef( rotate_y, 0.0, 1.0, 0.0);  // rotate y

    glScalef(scaleWheel,scaleWheel,scaleWheel);


    if(isDrawGrid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glColor3f(1.0f, 0.5f, 0.2f);  // outline color (orange)
        drawFirstMesh();

        glColor3f(1.0f, 0.0f, 0.0f);  // outline color (red)
        drawSecondMesh();
    }
    if(isDrawFaces)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glColor3f(0.5f, 0.5f, 0.5f);  // filling color (grey)
        drawFirstMesh();

        glColor3f(0.4f, 0.7f, 0.9f);  // filling color (grey)
        drawSecondMesh();
    }
    glPopMatrix(); // load the unscaled matrix


    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    doubleBuffer();
}

void OpenGlViewer::mousePressEvent(QMouseEvent *event) {
    if (event->button() ==
            Qt::LeftButton) {  // memorize coords x and y mouse when we start clicking
        x_pos = event->x();
        y_pos = event->y();
    }
}

void OpenGlViewer::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        if(x_pos && y_pos)
        {
            rotate_y = (event->x() - x_pos);  // rotate Object on x
            rotate_x = (event->y() + y_pos);  // rotate Object on y
            update();                       // update Form that display Object
        }
    }
}

void OpenGlViewer::mouseReleaseEvent(QMouseEvent *e) {
}

void OpenGlViewer::wheelEvent(QWheelEvent *event) {
    scaleWheel +=
            event->angleDelta().y()*scaleSpeed;  // change scale when scroll wheel
    update();
}


void OpenGlViewer::drawFirstMesh()
{
    if(drawFirstObject)
    {
        uint size=drawFirstObject->face.size();
        glBegin(GL_TRIANGLES);  // START TRIANGLES DRAWING
        for (uint i = 0; i < size; ++i) {
            glNormal3f((*drawFirstObject).face[i].N().X(),(*drawFirstObject).face[i].N().Y(),(*drawFirstObject).face[i].N().Z());
            glVertex3f((*drawFirstObject).face[i].P0(0).X(),(*drawFirstObject).face[i].P0(0).Y(),(*drawFirstObject).face[i].P0(0).Z());
            glVertex3f((*drawFirstObject).face[i].P0(1).X(),(*drawFirstObject).face[i].P0(1).Y(),(*drawFirstObject).face[i].P0(1).Z());
            glVertex3f((*drawFirstObject).face[i].P0(2).X(),(*drawFirstObject).face[i].P0(2).Y(),(*drawFirstObject).face[i].P0(2).Z());

        }
        glEnd();  // END TRIANGLES DRAWING
    }

}

void OpenGlViewer::drawSecondMesh()
{
    if(drawSecondObject)
    {
        uint size=drawSecondObject->face.size();
        glBegin(GL_TRIANGLES);  // START TRIANGLES DRAWING
        for (uint i = 0; i < size; ++i) {
            glNormal3f((*drawSecondObject).face[i].N().X(),(*drawSecondObject).face[i].N().Y(),(*drawSecondObject).face[i].N().Z());
            glVertex3f((*drawSecondObject).face[i].P0(0).X(),(*drawSecondObject).face[i].P0(0).Y(),(*drawSecondObject).face[i].P0(0).Z());
            glVertex3f((*drawSecondObject).face[i].P0(1).X(),(*drawSecondObject).face[i].P0(1).Y(),(*drawSecondObject).face[i].P0(1).Z());
            glVertex3f((*drawSecondObject).face[i].P0(2).X(),(*drawSecondObject).face[i].P0(2).Y(),(*drawSecondObject).face[i].P0(2).Z());
        }
        glEnd();  // END TRIANGLES DRAWING
    }
}


void OpenGlViewer::setFirstMesh(QString path )
{
    rotate_x=0;
    rotate_y=0;
    prevRotation_x=0;
    prevRotation_y=0;
    int err=0;

    err =  vcg::tri::io::Importer<MyMesh>::Open(*drawFirstObject,path.toLocal8Bit());




    if(err) { // all the importers return 0 in case of success
        printf("Error in reading %s: '%s'\n");
        exit(-1);
    }

    if(drawFirstObject)
    {

        vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawFirstObject);

        // calculateNormalFirstObject();


        float elements[6];//minX maxX minY maxY minZ maxZ

        elements[0]=(*drawFirstObject).bbox.min.X();
        elements[1]=(*drawFirstObject).bbox.max.X();
        elements[2]=(*drawFirstObject).bbox.min.Y();
        elements[3]=(*drawFirstObject).bbox.max.Y();
        elements[4]=(*drawFirstObject).bbox.min.Z();
        elements[5]=(*drawFirstObject).bbox.max.Z();

        int length[3];

        length[0]=elements[1]-elements[0];
        length[1]=elements[3]-elements[2];
        length[2]=elements[5]-elements[4];

        scaleSpeed=length[0];

        for(int i=1;i<3;++i)
            if(scaleSpeed<length[i])
                scaleSpeed=length[i];

        scaleSpeed*=0.02;

        maxOrigin=abs(elements[0]);

        for(int i=0;i<6;++i)
            if(maxOrigin<abs(elements[i]))
                maxOrigin=abs(elements[i]);

        scaleWheel=maxOrigin*5;

        light_position[0]=-orthoCoefficient * maxOrigin;
        light_position[1]=-orthoCoefficient * maxOrigin;
        light_position[2]=-orthoCoefficient * maxOrigin;
        light_position[3]=1;


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-(elements[1] + elements[0])/2.0f,-(elements[3] + elements[2])/2.0f,-(elements[5] + elements[4])/2.0f);

    }



}

void OpenGlViewer::setSecondMesh(QString path)
{
    if(drawFirstObject->fn==0)
    {
        QMessageBox::warning(this, "Warning","Please, choose first object");
        return;
    }

    int err=0;

    err =  vcg::tri::io::Importer<MyMesh>::Open(*drawSecondObject,path.toLocal8Bit());




    if(err) { // all the importers return 0 in case of success
        printf("Error in reading %s: '%s'\n");
        exit(-1);
    }

    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawSecondObject);
}

void OpenGlViewer::setLight(bool value)
{
    isLight=value;
    update();
}

void OpenGlViewer::saveSecondMesh()
{
    if(drawSecondObject->fn==0)
    {
        QMessageBox::warning(this, "Warning","Please, choose second object");
        return;
    }
    QString resultPath=QFileDialog::getSaveFileName(this,"Save object","C://",tr("STL (*.stl);;PLY (*.ply)"));
    QFileInfo file1(resultPath);
    if(resultPath.isEmpty())
        return;
    if(file1.completeSuffix()=="ply")
    {
        vcg::tri::io::ExporterPLY<MyMesh>::Save(*drawSecondObject ,resultPath.toLocal8Bit());
        return;
    }
    if(file1.completeSuffix()=="stl")
    {
        vcg::tri::io::ExporterSTL<MyMesh>::Save(*drawSecondObject ,resultPath.toLocal8Bit());
        return;
    }
    QMessageBox::warning(this, "Warning extension","Please select object with another format");
    return;

}

void OpenGlViewer::saveFirstMesh()
{
    if(drawFirstObject->fn==0)
    {
        QMessageBox::warning(this, "Warning","Please, choose second object");
        return;
    }
    QString resultPath=QFileDialog::getSaveFileName(this,"Save object","C://",tr("STL (*.stl);;PLY (*.ply)"));
    QFileInfo file1(resultPath);
    if(resultPath.isEmpty())
        return;
    if(file1.completeSuffix()=="ply")
    {
        vcg::tri::io::ExporterPLY<MyMesh>::Save(*drawFirstObject ,resultPath.toLocal8Bit());
        return;
    }
    if(file1.completeSuffix()=="stl")
    {
        vcg::tri::io::ExporterSTL<MyMesh>::Save(*drawFirstObject ,resultPath.toLocal8Bit());
        return;
    }
    QMessageBox::warning(this, "Warning extension","Please select object with another format");
    return;

}

void OpenGlViewer::alignSecondMesh()
{
    if(drawFirstObject->fn==0 || drawSecondObject->fn==0)
    {
        QMessageBox::warning(this, "Warning","Please, choose two objects");
        return;
    }


    vcg::AlignPair::Result result;
    vcg::AlignPair::Param ap;
    vcg::AlignPair::A2Mesh fix;
    vcg::AlignPair aa;


    // 1) Convert fixed mesh and put it into the grid.
    aa.convertMesh<MyMesh>(*drawFirstObject,fix);



    vcg::AlignPair::A2Grid UG;
    vcg::AlignPair::A2GridVert VG;

    if((*drawFirstObject).fn==0 || ap.UseVertexOnly) {
        fix.initVert(vcg::Matrix44d::Identity());
        vcg::AlignPair::InitFixVert(&fix,ap,VG);
    }
    else {
        fix.init(vcg::Matrix44d::Identity());
        vcg::AlignPair::initFix(&fix, ap, UG);
    }


    // 2) Convert the second mesh and sample a <ap.SampleNum> points on it.
    std::vector<vcg::AlignPair::A2Vertex> tmpmv;
    aa.convertVertex((*drawSecondObject).vert,tmpmv);


    aa.sampleMovVert(tmpmv, ap.SampleNum, ap.SampleMode);

    aa.mov=&tmpmv;
    aa.fix=&fix;
    aa.ap = ap;


    //use identity as first matrix
    vcg::Matrix44d In;
    In.SetIdentity();

    // Perform the ICP algorithm
    aa.align(In,UG,VG,result);

    //rotate m2 using the resulting transformation
    vcg::tri::UpdatePosition<MyMesh>::Matrix(*drawSecondObject, result.Tr, true);
    vcg::tri::UpdateBounding<MyMesh>::Box(*drawSecondObject);

    auto distance=result.computeAvgErr();
    emit setDistanceInLabel(QString("Distance:\ndd="+QString::number(distance.first)+"\ndd="+QString::number(distance.second)));
    update();
}

void OpenGlViewer::appendSecondMeshToFirst()
{
    if((*drawFirstObject).fn==0 || (*drawSecondObject).fn==0)
    {
        QMessageBox::warning(this, "Warning extension","Please select two objects");
        return;
    }
    vcg::tri::Append<MyMesh, MyMesh>::Mesh(*drawFirstObject,*drawSecondObject);

    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawFirstObject);

    (*drawSecondObject).fn=0;
    (*drawSecondObject).face.clear();
    (*drawSecondObject).vert.clear();
    emit setDistanceInLabel(QString(""));
    update();
}

void OpenGlViewer::setShowGrid(bool value)
{
    isDrawGrid=value;
    update();
}

void OpenGlViewer::setShowFaces(bool value)
{
    isDrawFaces=value;
    update();
}

