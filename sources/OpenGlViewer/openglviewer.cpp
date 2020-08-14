#include "../../headers/OpenGlViewer/openglviewer.h"
#include "ui_openglviewer.h"
std::vector<vcg::Point3d>* vcg::PointMatchingScale::fix;
std::vector<vcg::Point3d>* vcg::PointMatchingScale::mov;
vcg::Box3d vcg::PointMatchingScale::b;

const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "layout(location = 1) in vec3 normal;\n"
                                   "uniform mat4 projMatrix;\n"
                                   "uniform mat4 transMatrix;\n"
        // "out vec3 ourColor;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"

        "void main()\n"
        "{\n"
        "vec4 tempPos = vec4(position, 1.0);\n"
        "Normal =normal;\n"
        "FragPos =vec3(transMatrix*vec4(position,1.0f));\n"
        "gl_Position = projMatrix * transMatrix*  tempPos;\n"
        "}\0";


// "ourColor = color;\n"

const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "in vec3 FragPos;\n"
                                     "in vec3 Normal;\n"

                                     "uniform vec3 LightPosition;\n"
                                     "uniform vec3 ourColor;\n"
                                     "uniform vec3 lightColor;\n"
                                     "void main()\n"
                                     "{\n"

                                     "vec3 ambient = 0.1f *lightColor;\n"

                                     "vec3 norm =normalize(Normal);\n"
        //"vec3 lightDir =normalize(LightPosition-FragPos);\n"
        "vec3 lightDir =normalize(vec3(0.5,0.5,0.5));\n"

        "float diff=max(dot(norm,lightDir),0.0);\n"
        "vec3 diffuse=diff*lightColor;\n"
        //  "color = vec4 (lightColor*ourColor, 1.0f);\n"
        "vec3 result=(ambient+diffuse)*ourColor;\n"
        "color=vec4(result,1.0f);\n"
        //"color =ourColor;\n"

        "}\n\0";

//   "color = ourColor;\n"

OpenGlViewer::OpenGlViewer( QWidget *parent)
    : QGLWidget(parent) {

    ui->setupUi(this);

    rotation.setVector(0,0,0);
    // rotationAxis=QVector3D(0,0,0);

    sizeDrawVertex=0;

    drawVertex=nullptr;

    translateX=0;
    translateY=0;

    translateSpeed=5;

    drawFirstObject=new MyMesh();


    ratioWidthHeight=1;

    drawSecondObject=new MyMesh();


    setFormat(QGLFormat(QGL::DoubleBuffer));  // double buff

    light_diffuse[0]=0.5;
    light_diffuse[1]=0.5;
    light_diffuse[2]=0.5;
    light_diffuse[2]=0.5;

    light_ambient[0]=0.4;
    light_ambient[1]=0.4;
    light_ambient[2]=0.4;
    light_ambient[3]=0.4;

    minMaxXYZ[0]=minMaxXYZ[2]=minMaxXYZ[4]=100000000;
    minMaxXYZ[1]=minMaxXYZ[3]=minMaxXYZ[5]=-100000000;

    scaleSpeed = 0.001;
    //  scaleWheel = orthoCoefficient * maxOrigin*0.1;
    //    scaleSpeed = 0.001;
    scaleWheel = 1;

    light_position[0]=0;
    light_position[1]=0;
    light_position[2]=1;
    light_position[3]=0;


    light_position2[0]=0;
    light_position2[1]=0;
    light_position2[2]=-1;
    light_position2[3]=0;

    rotate_x=0;
    rotate_y=0;

    translateX=0;
    translateY=0;


}

OpenGlViewer::~OpenGlViewer() {
    delete drawFirstObject;
    delete drawSecondObject;
    if(drawVertex!=nullptr)
        delete [] drawVertex;
    delete ui;
}

void OpenGlViewer::initializeGL() {

    initializeOpenGLFunctions();

    f = QOpenGLContext::currentContext()->extraFunctions();

    GLuint vertexShader=f->glCreateShader(GL_VERTEX_SHADER);
    f->glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    f->glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = f->glCreateShader(GL_FRAGMENT_SHADER);
    f->glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    f->glCompileShader(fragmentShader);
    // Check for compile time errors
    f->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    shaderProgram = f->glCreateProgram();
    f->glAttachShader(shaderProgram, vertexShader);
    f->glAttachShader(shaderProgram, fragmentShader);
    f->glLinkProgram(shaderProgram);
    // Check for linking errors
    f->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        f->glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    f->glDeleteShader(vertexShader);
    f->glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers


    f->glGenVertexArrays(1, &VAO);


    f->glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    f->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //  f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

    f->glBindVertexArray(0); // Unbind VAO


    f->glDepthFunc(GL_LEQUAL);   // buff deep
    f->glEnable(GL_DEPTH_TEST);  // line that we can't see - become invisible

    f->glEnable(GL_COLOR_MATERIAL);
    f->glEnable(GL_NORMALIZE);
    // Game loop
    f->glUseProgram(shaderProgram);
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

    GPUobjectColor =  f->glGetUniformLocation(shaderProgram, "ourColor");
    GPUtransformMatrix=f->glGetUniformLocation(shaderProgram, "transMatrix");
    GPUprojectionMatrix=f->glGetUniformLocation(shaderProgram, "projMatrix");
    GPUlightPosition=f->glGetUniformLocation(shaderProgram, "LightPosition");
    GPUlightColor=f->glGetUniformLocation(shaderProgram, "lightColor");
}
void OpenGlViewer::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // qglClearColor(BACKGROUND_COLOR);
    glViewport(0, 0, (GLint)w, (GLint)h);
    aspect = qreal(w) / qreal(h);
}

void OpenGlViewer::paintGL() {
    f->glClearColor(BACKGROUND_COLOR[0],BACKGROUND_COLOR[1],BACKGROUND_COLOR[2],1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_transform.setToIdentity();
    m_projection.setToIdentity();

    m_transform.rotate(rotation);
    m_transform.translate(-(minMaxXYZ[1] + minMaxXYZ[0])/2.0f,-(minMaxXYZ[3] + minMaxXYZ[2])/2.0f,-(minMaxXYZ[4] + minMaxXYZ[5])/2.0f);

    m_projection.ortho(-maxOrigin*scaleWheel*aspect,maxOrigin*scaleWheel*aspect,
                       -maxOrigin*scaleWheel,maxOrigin*scaleWheel,
                       -maxOrigin*scaleWheel*3,maxOrigin*scaleWheel*3);

    //set transform matrix in geometry shader
    f->glUniformMatrix4fv(GPUtransformMatrix,1,GL_FALSE,m_transform.constData());
    //set projection matrix in geometry shader
    f->glUniformMatrix4fv(GPUprojectionMatrix,1,GL_FALSE,m_projection.constData());
    //set light position in geometry shader
    f->glUniform3f(GPUlightPosition,light_position[0],light_position[1],light_position[2]);
    //set color light in geometry shader (white)
    f->glUniform3f(GPUlightColor,1,1,1);

    f->glBindVertexArray(VAO);

    if(isDrawFaces)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw faces first mesh
        f->glUniform3f(GPUobjectColor, 0.5f, 0.5f, 0.5f);   //color of faces first mesh
        f->glDrawArrays(GL_TRIANGLES , 0, sizeDrawVertexFirstObject/6);

        //draw faces second mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 0.5f, 1.0f);   //color of faces second mesh
        f->glDrawArrays(GL_TRIANGLES , sizeDrawVertexFirstObject/6, sizeDrawVertex/6);
    }

    if(isDrawGrid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //draw grid first mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 0.0f, 0.7f);   //color of grid first mesh
        f->glDrawArrays(GL_TRIANGLES , 0, sizeDrawVertexFirstObject/6);

        //draw grid second mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 1.0f, 0.0f);//color of grid second mesh
        f->glDrawArrays(GL_TRIANGLES , sizeDrawVertexFirstObject/6, sizeDrawVertex/6);
    }

    f->glBindVertexArray(0);
}



void OpenGlViewer::mouseMoveEvent(QMouseEvent *e) {

    //    if (e->buttons() & Qt::LeftButton && cameraMove) {

    //        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    //        QVector2D n = QVector2D(diff.x(),diff.y()).normalized();

    //       // x_pos = e->x();
    //      //  y_pos = e->y();
    //        translateX -= n.x()* translateSpeed;
    //        translateY -= n.y() * translateSpeed;
    //        qDebug()<<"transX="<<translateX<<" transY="<<translateY;

    //        mousePressPosition=QVector2D(e->localPos());
    //       // prevRotation_x = x_pos;
    //       // prevRotation_y = y_pos;
    //        update();  // update Form that display Object
    //        qDebug() << "Shift";
    //        return;
    //    }
    //    else
    if (e->buttons() & Qt::LeftButton) {
        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

        //QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
        // qreal acc = diff.length() / 100.0;

        // Calculate new rotation axis as weighted sum
        //rotationAxis = ( QVector3D(diff.y(), diff.x(), 0.0).normalized() * diff.length() / 100.0).normalized();

        rotation = QQuaternion::fromAxisAndAngle((QVector3D(diff.y(), diff.x(), 0.0).normalized() * diff.length() / 100.0).normalized(), rotationSpeed) * rotation;

        mousePressPosition=QVector2D(e->localPos());
        // Request an update
        update();
    }
}
void OpenGlViewer::mousePressEvent(QMouseEvent *e) {
    mousePressPosition = QVector2D(e->localPos());
}

void OpenGlViewer::mouseReleaseEvent(QMouseEvent *e) {

}

void OpenGlViewer::wheelEvent(QWheelEvent *event) {
    if(scaleWheel<0.1)
    {
        scaleWheel=0.1;
        return;
    }
    scaleWheel +=
            event->angleDelta().y()*scaleSpeed;  // change scale when scroll wheel

    //  qDebug()<<"scale="<<scaleWheel;
    update();
}

void OpenGlViewer::keyPressEvent(QKeyEvent *event)
{
    //    if (event->key() == Qt::Key_Shift) {
    //        cameraMove = true;
    //    }
}

void OpenGlViewer::keyReleaseEvent(QKeyEvent *event)
{
    //    if (event->key() == Qt::Key_Shift) {
    //        cameraMove = false;
    //    }
}

void OpenGlViewer::findMinMaxForStl(MyMesh *_object)
{
    for(int i=0;i<_object->vn;++i)
    {
        if(minMaxXYZ[0]>_object->vert[i].P().X())
            minMaxXYZ[0]=_object->vert[i].P().X();
        if(minMaxXYZ[1]<_object->vert[i].P().X())
            minMaxXYZ[1]=_object->vert[i].P().X();

        if(minMaxXYZ[2]>_object->vert[i].P().Y())
            minMaxXYZ[2]=_object->vert[i].P().Y();
        if(minMaxXYZ[3]<_object->vert[i].P().Y())
            minMaxXYZ[3]=_object->vert[i].P().Y();

        if(minMaxXYZ[4]>_object->vert[i].P().Z())
            minMaxXYZ[4]=_object->vert[i].P().Z();
        if(minMaxXYZ[5]<_object->vert[i].P().Z())
            minMaxXYZ[5]=_object->vert[i].P().Z();
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
    if((*drawFirstObject).fn!=0)
    {
        minMaxXYZ[0]=minMaxXYZ[2]=minMaxXYZ[4]=100000000;
        minMaxXYZ[1]=minMaxXYZ[3]=minMaxXYZ[5]=-100000000;
        findMinMaxForStl(drawFirstObject);
    }

    if((*drawSecondObject).fn!=0)
        findMinMaxForStl(drawSecondObject);


    double distanceX=abs((minMaxXYZ[1]-minMaxXYZ[0]));
    double distanceY=abs((minMaxXYZ[3]-minMaxXYZ[2]));
    double distanceZ=abs((minMaxXYZ[5]-minMaxXYZ[4]));


    //transformMatrix.setToIdentity();
    maxOrigin=distanceX;

    if(maxOrigin<distanceY)
        maxOrigin=distanceY;
    if(maxOrigin<distanceZ)
        maxOrigin=distanceZ;

  //  glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();
    //glTranslatef(-(minMaxXYZ[1] + minMaxXYZ[0])/2.0f,-(minMaxXYZ[3] + minMaxXYZ[2])/2.0f,-(minMaxXYZ[4] + minMaxXYZ[5])/2.0f);

}

void OpenGlViewer::updateDrawVertex()
{

    if(drawVertex!=nullptr)
        delete[] drawVertex;

    int n=18;

    sizeDrawVertex=n*(drawFirstObject->face.size()+drawSecondObject->face.size());


    drawVertex=new GLfloat[sizeDrawVertex];


    sizeDrawVertexFirstObject=drawFirstObject->face.size();

    for(int i=0;i<sizeDrawVertexFirstObject;++i)
    {
        drawVertex[n*i  ]=(*drawFirstObject).face[i].P0(0).X();
        drawVertex[n*i+1]=(*drawFirstObject).face[i].P0(0).Y();
        drawVertex[n*i+2]=(*drawFirstObject).face[i].P0(0).Z();

        drawVertex[n*i+3]=(*drawFirstObject).face[i].N().X();
        drawVertex[n*i+4]=(*drawFirstObject).face[i].N().Y();
        drawVertex[n*i+5]=(*drawFirstObject).face[i].N().Z();

        drawVertex[n*i+6]=(*drawFirstObject).face[i].P0(1).X();
        drawVertex[n*i+7]=(*drawFirstObject).face[i].P0(1).Y();
        drawVertex[n*i+8]=(*drawFirstObject).face[i].P0(1).Z();

        drawVertex[n*i+9]=(*drawFirstObject).face[i].N().X();
        drawVertex[n*i+10]=(*drawFirstObject).face[i].N().Y();
        drawVertex[n*i+11]=(*drawFirstObject).face[i].N().Z();

        drawVertex[n*i+12]=(*drawFirstObject).face[i].P0(2).X();
        drawVertex[n*i+13]=(*drawFirstObject).face[i].P0(2).Y();
        drawVertex[n*i+14]=(*drawFirstObject).face[i].P0(2).Z();

        drawVertex[n*i+15]=(*drawFirstObject).face[i].N().X();
        drawVertex[n*i+16]=(*drawFirstObject).face[i].N().Y();
        drawVertex[n*i+17]=(*drawFirstObject).face[i].N().Z();
    }
    sizeDrawVertexFirstObject*=n;

    int  size2=drawSecondObject->face.size();

    for(int i=0;i<size2;++i)
    {
        drawVertex[sizeDrawVertexFirstObject + n*i  ]=(*drawSecondObject).face[i].P0(0).X();
        drawVertex[sizeDrawVertexFirstObject +n*i+1]=(*drawSecondObject).face[i].P0(0).Y();
        drawVertex[sizeDrawVertexFirstObject +n*i+2]=(*drawSecondObject).face[i].P0(0).Z();

        drawVertex[sizeDrawVertexFirstObject +n*i+3]=(*drawSecondObject).face[i].N().X();
        drawVertex[sizeDrawVertexFirstObject +n*i+4]=(*drawSecondObject).face[i].N().Y();
        drawVertex[sizeDrawVertexFirstObject +n*i+5]=(*drawSecondObject).face[i].N().Z();

        drawVertex[sizeDrawVertexFirstObject +n*i+6]=(*drawSecondObject).face[i].P0(1).X();
        drawVertex[sizeDrawVertexFirstObject +n*i+7]=(*drawSecondObject).face[i].P0(1).Y();
        drawVertex[sizeDrawVertexFirstObject +n*i+8]=(*drawSecondObject).face[i].P0(1).Z();

        drawVertex[sizeDrawVertexFirstObject +n*i+9]=(*drawSecondObject).face[i].N().X();
        drawVertex[sizeDrawVertexFirstObject +n*i+10]=(*drawSecondObject).face[i].N().Y();
        drawVertex[sizeDrawVertexFirstObject +n*i+11]=(*drawSecondObject).face[i].N().Z();

        drawVertex[sizeDrawVertexFirstObject +n*i+12]=(*drawSecondObject).face[i].P0(2).X();
        drawVertex[sizeDrawVertexFirstObject +n*i+13]=(*drawSecondObject).face[i].P0(2).Y();
        drawVertex[sizeDrawVertexFirstObject +n*i+14]=(*drawSecondObject).face[i].P0(2).Z();

        drawVertex[sizeDrawVertexFirstObject +n*i+15]=(*drawSecondObject).face[i].N().X();
        drawVertex[sizeDrawVertexFirstObject +n*i+16]=(*drawSecondObject).face[i].N().Y();
        drawVertex[sizeDrawVertexFirstObject +n*i+17]=(*drawSecondObject).face[i].N().Z();
    }


    f->glBufferData(GL_ARRAY_BUFFER,  sizeDrawVertex * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    f->glBufferData(GL_ARRAY_BUFFER,  sizeDrawVertex * sizeof(GLfloat), drawVertex, GL_STATIC_DRAW);
    update();
}

void OpenGlViewer::exportAsMLP(QString filename)
{
    if(vectorContentMLP.empty() || filename.isEmpty())
    {
        QMessageBox::warning(this,"Warning", "Load align meshes");
        qDebug()<<"Vector exportMLP empty";
        return;
    }
    // QString exportPath=QFileDialog::getSaveFileName(this,"Save object","C://",tr("MLP (*.mlp)"));
    // if(exportPath.isEmpty())
    //    return;


    QByteArray mlpFileContent="<!DOCTYPE MeshLabDocument>\n"
                              "<MeshLabProject>\n"
                              "<MeshGroup>\n";

    QStringList matrixElements;
    int mlpSize=vectorContentMLP.size();

    for(int i=0;i<mlpSize;++i)
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

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
        file.write(mlpFileContent);
    file.close();
}


void OpenGlViewer::drawFirstMesh()
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

void OpenGlViewer::drawSecondMesh()
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

bool OpenGlViewer::setFirstMesh(QString path, bool isNeedToDraw)
{

    if(drawFirstObject!=nullptr)
        delete drawFirstObject;
    drawFirstObject=new MyMesh();

    if(vcg::tri::io::Importer<MyMesh>::Open(*drawFirstObject,path.toLocal8Bit())) { // all the importers return 0 in case of success
        printf("Error in reading %s: '%s'\n");
        // QMessageBox::warning(this,"Error", "Can't open file "+path);
        drawFirstObject->Clear();
        return false;
    }

    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawFirstObject);

    if(isNeedToDraw)
    {
        InitMaxOrigin();
        updateDrawVertex();
    }
    return true;
}

bool OpenGlViewer::setSecondMesh(QString path,bool isNeedToDraw)
{

    if(drawFirstObject==nullptr)
    {
        QMessageBox::warning(this, "Warning","Please, choose first object");
        return false;
    }
    if(drawSecondObject!=nullptr)
        delete drawSecondObject;
    drawSecondObject=new MyMesh();

    if(vcg::tri::io::Importer<MyMesh>::Open(*drawSecondObject,path.toLocal8Bit())) { // all the importers return 0 in case of success
        drawSecondObject->Clear();
        printf("Error in reading %s: '%s'\n");
        // QMessageBox::warning(this,"Error", "Can't open file "+path);
        return false;
    }
    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawSecondObject);

    if(isNeedToDraw)
    {
        InitMaxOrigin();
        updateDrawVertex();
    }
    return true;
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

    if(resultTransformMatrix!=nullptr)
        resultTransformMatrix->SetIdentity();

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

        if(distance.first<previousError)
        {
            previousResult=result;
            previousError=distance.first;
            if(resultTransformMatrix!=nullptr)
                (*resultTransformMatrix)=(*resultTransformMatrix)*result.Tr;
        }
        else{
            vcg::tri::UpdatePosition<MyMesh>::Matrix(*drawSecondObject,vcg::Inverse(result.Tr), true);
            vcg::tri::UpdatePosition<MyMesh>::Matrix(*drawSecondObject, previousResult.Tr, true);
            vcg::tri::UpdateBounding<MyMesh>::Box(*drawSecondObject);
            distance=previousResult.computeAvgErr();
            break;
        }

    }
    if(distance.first>ERROR_ALIGN)
    {
        if(isVisible!=nullptr)
            (*isVisible)=false;
        emit setDistanceInLabel(QString("Defective mesh\nDistance="+QString::number(distance.first)+">"+QString::number(ERROR_ALIGN)));
    }
    else
    {
        if(isVisible!=nullptr)
            (*isVisible)=true;
        emit setDistanceInLabel(QString("Distance:\ndd="+QString::number(distance.first)+"\ndd="+QString::number(distance.first)));
    }
    updateDrawVertex();
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
    updateDrawVertex();
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

    //path to file with meshes that need to align
    QString pathAlignMeshes=QFileDialog::getOpenFileName(this,"Open file with collect of meshes" );
    QString exportPath=pathAlignMeshes.split(".")[0]+".mlp";

    QFileInfo fileInfo(pathAlignMeshes);

    //get path to dir where file with meshes located
    const QString pathToDir=pathAlignMeshes.mid(0,pathAlignMeshes.size()-fileInfo.fileName().size());

    if(pathAlignMeshes.isEmpty())
        return;

    QFile fileWithAlignMeshes(pathAlignMeshes);
    if(fileWithAlignMeshes.open(QIODevice::ReadOnly))
    {
        QString fileName;
        QByteArray isEmpty;
        do{
            isEmpty=fileWithAlignMeshes.readLine();
            if(isEmpty.isEmpty() || isEmpty=="\r\n" || isEmpty=="\n")
                continue;

            fileName=QString::fromStdString(isEmpty.toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0];

            if(!QFileInfo(pathToDir+fileName).exists())
                continue;
        }while(!setFirstMesh(pathToDir+fileName,false) && !fileWithAlignMeshes.atEnd());

        vectorContentMLP.push_back({fileName,identityMatrix,"1"}); //add First mesh to MPL vector with Identity matrix

        //read all meshes from file and import it
        vcg::Matrix44d tempMatrix;
        bool isVisible=false;

        while(!fileWithAlignMeshes.atEnd())
        {
            isEmpty=fileWithAlignMeshes.readLine();
            if(isEmpty.isEmpty() || isEmpty=="\r\n" || isEmpty=="\n")
                continue;

            fileName=QString::fromStdString(isEmpty.toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0];

            if(!QFileInfo(pathToDir+fileName).exists() || !setSecondMesh(pathToDir+fileName,false))
                continue;

            alignSecondMesh(&tempMatrix,&isVisible);

            if(isVisible)
            {
                vectorContentMLP.push_back({fileName,vcgMatrixToString(tempMatrix),"1"});  //if visible push actual matrix data with visible ==1
                appendSecondMeshToFirst();
            }
            else
                vectorContentMLP.push_back({fileName,identityMatrix,"0"}); //if non visible, - push Matrix Identity with visible ==0
        }
    }
    fileWithAlignMeshes.close();
    exportAsMLP(exportPath);

    InitMaxOrigin();
    updateDrawVertex();
}


//void OpenGlViewer::compareObjects()
//{
////    qDebug()<<"COmpare first object start";
////    for(int i=0;i<testFirstObject.vn;++i)
////    {
////        if(testFirstObject.vert[i].P().X()-(*drawFirstObject).vert[i].P().X()!=0)
////        {
////            qDebug()<<"["<<i<<"]="<<testFirstObject.vert[i].P().X()-(*drawFirstObject).vert[i].P().X();
////        }
////    }
////       qDebug()<<"COmpare first object finished";

//       qDebug()<<"COmpare second object start";
//       for(int i=0;i<testSecondObject.vn;++i)
//       {
//           if(testSecondObject.vert[i].P().X()-(*drawSecondObject).vert[i].P().X()!=0)
//           {
//               qDebug()<<"["<<i<<"]="<<testSecondObject.vert[i].P().X()-(*drawSecondObject).vert[i].P().X();
//           }
//       }
//          qDebug()<<"COmpare second object finished";
//}

//void OpenGlViewer::coutMatrix(vcg::Matrix44d *resultTransformMatrix)
//{
//    qDebug()<<"Matrix cout start:";
//    for(int i=0;i<4;++i)
//    {
//        for(int j=0;j<4;++j)
//        {
//            qDebug()<<"Element["<<i<<","<<j<<"]="<<resultTransformMatrix->ElementAt(i,j);
//        }
//    }
//    qDebug()<<"Matrix cout end:";
//}
