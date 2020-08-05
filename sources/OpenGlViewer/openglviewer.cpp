#include "../../headers/OpenGlViewer/openglviewer.h"
#include "ui_openglviewer.h"
std::vector<vcg::Point3d>* vcg::PointMatchingScale::fix;
std::vector<vcg::Point3d>* vcg::PointMatchingScale::mov;
vcg::Box3d vcg::PointMatchingScale::b;

OpenGlViewer::OpenGlViewer( QWidget *parent)
    : QGLWidget(parent) {

    ui->setupUi(this);

    translateX=0;
    translateY=0;

    drawFirstObject=new MyMesh();
    drawFirstObject->fn=0;

    ratioWidthHeight=1;

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

    minMaxXYZ[0]=minMaxXYZ[2]=minMaxXYZ[4]=100000000;
    minMaxXYZ[1]=minMaxXYZ[3]=minMaxXYZ[5]=-100000000;

}

OpenGlViewer::~OpenGlViewer() {
    delete drawFirstObject;
    delete drawSecondObject;
    delete ui;
}

void OpenGlViewer::initializeGL() {

    initializeOpenGLFunctions();
    glDepthFunc(GL_LEQUAL);   // buff deep
    qglClearColor(BACKGROUND_COLOR);  // set background

    glEnable(GL_DEPTH_TEST);  // line that we can't see - become invisible
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    //to enable



}
void OpenGlViewer::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)w, (GLint)h);
    ratioWidthHeight = (double)w / h;
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

    glTranslatef(translateX, translateY, 0);

    glRotatef( rotate_x, 1.0, 0.0, 0.0);  // rotate x
    glRotatef( rotate_y, 0.0, 1.0, 0.0);  // rotate y

    glScalef(scaleWheel,scaleWheel* ratioWidthHeight,scaleWheel);

    if(isDrawGrid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glColor3f(std::get<0>(MESH1_GRID_COLOR),std::get<1>(MESH1_GRID_COLOR),std::get<2>(MESH1_GRID_COLOR));  // outline color (orange)
        drawFirstMesh();

        glColor3f(std::get<0>(MESH2_GRID_COLOR),std::get<1>(MESH2_GRID_COLOR),std::get<2>(MESH2_GRID_COLOR));  // outline color (red)
        drawSecondMesh();
    }
    if(isDrawFaces)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glColor3f(std::get<0>(MESH1_FACES_COLOR),std::get<1>(MESH1_FACES_COLOR),std::get<2>(MESH1_FACES_COLOR));  // filling color (grey)
        drawFirstMesh();

        glColor3f(std::get<0>(MESH2_FACES_COLOR),std::get<1>(MESH2_FACES_COLOR),std::get<2>(MESH2_FACES_COLOR));   // filling color (grey)
        drawSecondMesh();
    }
    glPopMatrix(); // load the unscaled matrix


    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    doubleBuffer();
}



void OpenGlViewer::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && cameraMove) {
        x_pos = event->x();
        y_pos = event->y();
        translateX += (x_pos - prevRotation_x) * translateSpeed;
        translateY += (y_pos - prevRotation_y) * translateSpeed;

        prevRotation_x = x_pos;
        prevRotation_y = y_pos;
        update();  // update Form that display Object
        qDebug() << "Shift";
        return;
    }
    if (event->buttons() & Qt::LeftButton) {
        x_pos = event->x();
        y_pos = event->y();
        rotate_y += (x_pos - prevRotation_x) *rotationSpeed;
        rotate_x += (y_pos - prevRotation_y) * rotationSpeed;
        // rotate_y = (rotate_y > 360.0f) ? 360.0f : rotate_y - 360.0f;
        //rotate_x = (rotate_x > 360.0f) ? 360.0f : rotate_x - 360.0f;

        prevRotation_x = x_pos;
        prevRotation_y = y_pos;
        update();  // update Form that display Object
    }
}
void OpenGlViewer::mousePressEvent(QMouseEvent *event) {
    if (event->button() ==
            Qt::LeftButton) {  // memorize coords x and y mouse when we start clicking
        x_pos = event->x();
        y_pos = event->y();
        prevRotation_x = x_pos;
        prevRotation_y = y_pos;
    }
}

void OpenGlViewer::mouseReleaseEvent(QMouseEvent *e) {
}

void OpenGlViewer::wheelEvent(QWheelEvent *event) {
    scaleWheel +=
            event->angleDelta().y()*scaleSpeed;  // change scale when scroll wheel
    update();
}

void OpenGlViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        cameraMove = true;
        qDebug() << "Pressed shift";
    }
}

void OpenGlViewer::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        cameraMove = false;
        qDebug() << "Pressed shift";
    }
}

QString OpenGlViewer::vcgMatrixToString(const vcg::Matrix44d &resultTransformMatrix)
{
    QString result;
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
        {
            auto we=resultTransformMatrix.ElementAt(i,j);
            result.append(QString::number(we,'f',10)+" ");
        }
    return result;
}

void OpenGlViewer::InitMaxOrigin()
{
    rotate_x=0;
    rotate_y=0;
    prevRotation_x=0;
    prevRotation_y=0;

    translateX=0;
    translateY=0;

    if((*drawSecondObject).fn==0)
    {
        minMaxXYZ[0]=(*drawFirstObject).bbox.min.X();
        minMaxXYZ[1]=(*drawFirstObject).bbox.max.X();
        minMaxXYZ[2]=(*drawFirstObject).bbox.min.Y();
        minMaxXYZ[3]=(*drawFirstObject).bbox.max.Y();
        minMaxXYZ[4]=(*drawFirstObject).bbox.min.Z();
        minMaxXYZ[5]=(*drawFirstObject).bbox.max.Z();
    }
    else{
            minMaxXYZ[0]=(*drawFirstObject).bbox.min.X()>(*drawSecondObject).bbox.min.X()? (*drawSecondObject).bbox.min.X(): (*drawFirstObject).bbox.min.X();
            minMaxXYZ[1]=(*drawFirstObject).bbox.max.X()<(*drawSecondObject).bbox.max.X()? (*drawSecondObject).bbox.max.X(): (*drawFirstObject).bbox.max.X();
            minMaxXYZ[2]=(*drawFirstObject).bbox.min.Y()>(*drawSecondObject).bbox.min.Y()? (*drawSecondObject).bbox.min.Y(): (*drawFirstObject).bbox.min.Y();
            minMaxXYZ[3]=(*drawFirstObject).bbox.max.Y()<(*drawSecondObject).bbox.max.Y()? (*drawSecondObject).bbox.max.Y(): (*drawFirstObject).bbox.max.Y();
            minMaxXYZ[4]=(*drawFirstObject).bbox.min.Z()>(*drawSecondObject).bbox.min.Z()? (*drawSecondObject).bbox.min.Z(): (*drawFirstObject).bbox.min.Z();
            minMaxXYZ[5]=(*drawFirstObject).bbox.max.Z()<(*drawSecondObject).bbox.max.Z()? (*drawSecondObject).bbox.max.Z(): (*drawFirstObject).bbox.max.Z();

    }


    maxOrigin=abs(minMaxXYZ[0]);

    for(int i=0;i<6;++i)
        if(maxOrigin<abs(minMaxXYZ[i]))
            maxOrigin=abs(minMaxXYZ[i]);

    translateSpeed=maxOrigin*orthoCoefficient*0.005;

    //   scaleWheel=maxOrigin*5;
    scaleSpeed = orthoCoefficient*maxOrigin * 0.0001;
    scaleWheel = orthoCoefficient * maxOrigin*0.1;

    light_position[0]=-orthoCoefficient * maxOrigin;
    light_position[1]=-orthoCoefficient * maxOrigin;
    light_position[2]=-orthoCoefficient * maxOrigin;
    light_position[3]=1;


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-(minMaxXYZ[1] + minMaxXYZ[0])/2.0f,-(minMaxXYZ[3] + minMaxXYZ[2])/2.0f,-(minMaxXYZ[5] + minMaxXYZ[4])/2.0f);

}

void OpenGlViewer::exportAsMLP()
{
    if(vectorContentMLP.empty())
    {
        QMessageBox::warning(this,"Warning", "Load align meshes");
        qDebug()<<"Vector exportMLP empty";
        return;
    }
    QString exportPath=QFileDialog::getSaveFileName(this,"Save object","C://",tr("MLP (*.mlp)"));
    if(exportPath.isEmpty())
        return;


    QByteArray mlpFileContent="<!DOCTYPE MeshLabDocument>\n"
                              "<MeshLabProject>\n"
                              "<MeshGroup>\n";

    QStringList matrixElements;
    for(int i=0;i<vectorContentMLP.size();++i)
    {
        matrixElements = vectorContentMLP[i][1].split(QRegExp("\\s+"), QString::SkipEmptyParts);

        mlpFileContent.append("<MLMesh label=\""+vectorContentMLP[i][0]+"\" filename=\""+vectorContentMLP[i][0]+"\" visible=\""+vectorContentMLP[i][2]+"\">\n"
                                                                                                                                                       "<MLMatrix44>\n"+
                matrixElements[0]+" "+matrixElements[1]+" "+matrixElements[2]+" "+matrixElements[3]+" \n"+
                matrixElements[4]+" "+matrixElements[5]+" "+matrixElements[6]+" "+matrixElements[7]+" \n"+
                matrixElements[8]+" "+matrixElements[9]+" "+matrixElements[10]+" "+matrixElements[11]+" \n"+
                matrixElements[12]+" "+matrixElements[13]+" "+matrixElements[14]+" "+matrixElements[15]+" \n"+
                "</MLMatrix44>\n");

        mlpFileContent.append("<RenderingOption pointColor=\"131 149 69 255\" pointSize=\"3\" wireColor=\"64 64 64 255\" wireWidth=\"1\" boxColor=\"234 234 234 255\" solidColor=\"192 192 192 255\">100001000000000000000100000001011000001010100000000100111010000111001001</RenderingOption>\n"
                              "</MLMesh>\n");
    }


    mlpFileContent.append("</MeshGroup>\n"
                          "<RasterGroup/>\n"
                          "</MeshLabProject>\n");

    QFile file(exportPath);
    if(file.open(QIODevice::WriteOnly))
        file.write(mlpFileContent);
    file.close();

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
    //    rotate_x=0;
    //    rotate_y=0;
    //    prevRotation_x=0;
    //    prevRotation_y=0;

    //    translateX=0;
    //    translateY=0;
    int err=0;

    err =  vcg::tri::io::Importer<MyMesh>::Open(*drawFirstObject,path.toLocal8Bit());




    if(err) { // all the importers return 0 in case of success
        printf("Error in reading %s: '%s'\n");
        exit(-1);
    }



    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawFirstObject);

    // calculateNormalFirstObject();

    InitMaxOrigin();
    //        minMaxXYZ[0]=(*drawFirstObject).bbox.min.X();
    //        minMaxXYZ[1]=(*drawFirstObject).bbox.max.X();
    //        minMaxXYZ[2]=(*drawFirstObject).bbox.min.Y();
    //        minMaxXYZ[3]=(*drawFirstObject).bbox.max.Y();
    //        minMaxXYZ[4]=(*drawFirstObject).bbox.min.Z();
    //        minMaxXYZ[5]=(*drawFirstObject).bbox.max.Z();

    //        int length[3];

    //        length[0]=minMaxXYZ[1]-minMaxXYZ[0];
    //        length[1]=minMaxXYZ[3]-minMaxXYZ[2];
    //        length[2]=minMaxXYZ[5]-minMaxXYZ[4];

    //        scaleSpeed=length[0];

    //        for(int i=1;i<3;++i)
    //            if(scaleSpeed<length[i])
    //                scaleSpeed=length[i];







    update();


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

    InitMaxOrigin( );

    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawSecondObject);
    update();
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

void OpenGlViewer::alignSecondMesh(vcg::Matrix44d * resultTransformMatrix=nullptr, bool * isVisible=nullptr)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if(drawFirstObject->fn==0 || drawSecondObject->fn==0)
    {
        QMessageBox::warning(this, "Warning","Please, choose two objects");
        return;
    }



    vcg::AlignPair::Result result;

    vcg::AlignPair::Result previousResult;

    vcg::AlignPair::Param ap;
    vcg::AlignPair::A2Mesh fix;
    vcg::AlignPair aa;
    vcg::AlignPair::A2Grid UG;
    vcg::AlignPair::A2GridVert VG;
    std::vector<vcg::AlignPair::A2Vertex> tmpmv;


    // 1) Convert fixed mesh and put it into the grid.





    aa.convertMesh<MyMesh>(*drawFirstObject,fix);



    if((*drawFirstObject).fn==0 || ap.UseVertexOnly) {
        fix.initVert(vcg::Matrix44d::Identity());
        vcg::AlignPair::InitFixVert(&fix,ap,VG);
    }
    else {
        fix.init(vcg::Matrix44d::Identity());
        vcg::AlignPair::initFix(&fix, ap, UG);
    }



    // 2) Convert the second mesh and sample a <ap.SampleNum> points on it.
    float previousError=100000;
    std::pair<double,double> distance={10000,10000};
    for(uint i=0;i<COUNT_ALIGN_CYCLES;++i)
    {
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



        distance=result.computeAvgErr();

        if(distance.second==distance.first || isnan(distance.second) || isnan(distance.first))
        {
            if(isVisible!=nullptr)
                (*isVisible)=false;
            emit setDistanceInLabel("Defective mesh, distance so long");
            QApplication::restoreOverrideCursor();
            return;
        }

        if(distance.second<previousError)
        {
            //  qDebug()<<"prev x="<<(*drawSecondObject).face[10523].P(0).X();
            previousResult=result;
            previousError=distance.second;
            if(resultTransformMatrix!=nullptr)
                (*resultTransformMatrix)=result.Tr;
            // qDebug()<<"Prev value"<<distance.second;
        }
        else{
            //  qDebug()<<"new Prev Value="<<distance.second;
            vcg::tri::UpdatePosition<MyMesh>::Matrix(*drawSecondObject,vcg::Inverse(result.Tr), true);
            vcg::tri::UpdatePosition<MyMesh>::Matrix(*drawSecondObject, previousResult.Tr, true);
            vcg::tri::UpdateBounding<MyMesh>::Box(*drawSecondObject);
            //  qDebug()<<"new x="<<(*drawSecondObject).face[10523].P(0).X();
            distance=previousResult.computeAvgErr();
            if(resultTransformMatrix!=nullptr)
                (*resultTransformMatrix)=previousResult.Tr;
            // qDebug()<<"New value value"<<distance.second;
            break;
        }
        //   qDebug()<<"iteration="<<i;

    }
    if(distance.second>ERROR_ALIGN)
    {
        if(isVisible!=nullptr)
            (*isVisible)=false;
        emit setDistanceInLabel(QString("Defective mesh\nDistance="+QString::number(distance.second)+">"+QString::number(ERROR_ALIGN)));
    }
    else
    {
        if(isVisible!=nullptr)
            (*isVisible)=true;
        emit setDistanceInLabel(QString("Distance:\ndd="+QString::number(distance.first)+"\ndd="+QString::number(distance.second)));
    }
    update();
    QApplication::restoreOverrideCursor();
}

void OpenGlViewer::appendSecondMeshToFirst()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
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
    QApplication::restoreOverrideCursor();
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

void OpenGlViewer::openAlignFile()
{
    vectorContentMLP.clear();
    //exportAsMLP({{"FileName.ply","0 61 3 53 2 3 6 3 223 2 1 3 2 4 21 32","1"},{"FileName2.ply","0 61 3 53 2 3 6 3 223 2 1 3 2 4 21 32","0"},{"FileName.ply","0 61 3 53 2 3 6 3 223 2 1 3 2 4 21 32","1"}});

    //path to file with meshes that need to align
    QString pathAlignMeshes=QFileDialog::getOpenFileName(this,"Open file with collect of meshes" );


    QFileInfo fileInfo(pathAlignMeshes);

    //get path to dir where file with meshes located
    QString pathToDir=pathAlignMeshes.mid(0,pathAlignMeshes.size()-fileInfo.fileName().size());

    if(pathAlignMeshes.isEmpty())
        return;

    QFile fileWithAlignMeshes(pathAlignMeshes);
    if(fileWithAlignMeshes.open(QIODevice::ReadOnly))
    {
        QString fileName=QString::fromStdString(fileWithAlignMeshes.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0];

        setFirstMesh(pathToDir+fileName);
        vectorContentMLP.push_back({fileName,identityMatrix,"1"}); //add First mesh to MPL vector with Identity matrix

        //read all meshes from file and import it
        vcg::Matrix44d tempMatrix;
        bool isVisible=false;


        while(!fileWithAlignMeshes.atEnd())
        {
            fileName=QString::fromStdString(fileWithAlignMeshes.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0];
            setSecondMesh(pathToDir+fileName);
            alignSecondMesh(&tempMatrix,&isVisible);

            if(isVisible)
            {
                vectorContentMLP.push_back({fileName,vcgMatrixToString(tempMatrix),"1"});  //if visible push actual matrix data with visible ==1
                appendSecondMeshToFirst();
            }
            else{
                vectorContentMLP.push_back({fileName,identityMatrix,"0"}); //if non visible, - push Matrix Identity with visible ==0
            }

        }
    }
    update();
    fileWithAlignMeshes.close();

}


