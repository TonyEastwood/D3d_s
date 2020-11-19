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
        "out vec4 myColor;\n"

        "void main()\n"
        "{\n"
        "vec4 tempPos = vec4(position, 1.0);\n"
        "Normal =vec3(projMatrix * transMatrix*vec4(-normal,0.0));\n"
        "FragPos =vec3(projMatrix *transMatrix*vec4(position,1.0f));\n"
        "gl_Position = projMatrix * transMatrix*  tempPos;\n"

        "vec3 normal_cameraspace = normalize((projMatrix * transMatrix* vec4(-normal,0)).xyz);\n"
        "vec3 cameraVector = normalize(vec3(0, 0, 1) - (projMatrix * transMatrix*  vec4(position, 1)).xyz);\n"
        "float cosTheta = clamp( dot( normal_cameraspace, cameraVector ), 0,1 );\n"
        "myColor = vec4(0.3 * vec3(0.9, 0.9, 0.9) + cosTheta * vec3(0.5, 0.5, 0.5), 1);\n"
        "}\0";


// "ourColor = color;\n"

const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "in vec3 FragPos;\n"
                                     "in vec3 Normal;\n"
                                     "in vec4 myColor;\n"

                                     "uniform vec3 LightPosition;\n"
                                     "uniform vec3 LightPosition2;\n"
                                     "uniform vec3 ourColor;\n"
                                     "uniform vec3 lightColor;\n"
                                     "void main()\n"
                                     "{\n"

        //                                     "vec3 ambient = 0.3f *lightColor;\n"

        //                                     "vec3 norm =normalize(Normal);\n"
        //                                     "vec3 lightDir =normalize(LightPosition-FragPos);\n"
        //                                     "vec3 lightDir2 =normalize(LightPosition2-FragPos);\n"
        //        //"vec3 lightDir =normalize(vec3(0.5,0.5,0.5));\n"
        //        "float diff;\n"
        //        "float diff2;\n"
        //        //"if(max(dot(norm,lightDir),0.0)>max(dot(norm,lightDir2),0.0)) \n"
        //        " diff=clamp(max(dot(norm,lightDir),0.0),0.0,1.0);\n"
        //        //  "else \n"
        //        " diff2=clamp(max(dot(norm,lightDir2),0.0),0.0,1.0);\n"
        //        "vec3 diffuse=clamp(diff+diff2, 0.0, 1.0)*lightColor;\n"
        //        //  "color = vec4 (lightColor*ourColor, 1.0f);\n"
        //        "vec3 result=clamp((ambient+diffuse),0.0,1.0)*ourColor;\n"



        "color=myColor;\n"
        //"color =ourColor;\n"
        "}\n\0";

//   "color = ourColor;\n"

const GLchar* vertexShaderBackground = "#version 330 core\n"
                                       " out vec2 v_uv;\n"

                                       "void main()\n"
                                       "{\n"

                                       " uint idx = uint(gl_VertexID);\n"
                                       " gl_Position = vec4( idx & 1U, idx >> 1U, 0.0, 0.5 ) * 4.0 - 1.0;\n"
                                       " v_uv = vec2( gl_Position.xy * 0.5 + 0.5 );\n"

                                       "}\n\0";

const GLchar* fragmentShaderBackground = "#version 330 core\n"
                                         "uniform vec4 top_color;\n"
                                         "uniform vec4 bot_color;\n"
                                         "in vec2 v_uv;\n"
                                         " out vec4 frag_color;\n"
                                         "void main()\n"
                                         "{\n"


                                         " frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;\n"

                                         "}\n\0";



OpenGlViewer::OpenGlViewer( QWidget *parent)
    : QGLWidget(parent) {

    ui->setupUi(this);

    timer=new QElapsedTimer();

    tempRotationMatrix.setToIdentity();
    //rotation.setVector(0,0,0);
    rotation.setVector(-0.173981   ,0.70098    ,0.659057);
    rotation.setScalar(-0.209778);
    //rotation.normalize();


    sizeDrawVertex=0;
    distanceInfo.clear();
    drawVertex=nullptr;

    translateSpeed=5;

    ratioWidthHeight=1;

    meshes.clear();
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
    scaleWheel = 2;

    //    light_position[0]=0;
    //    light_position[1]=-50;
    //    light_position[2]=-10;
    //    light_position[3]=0;


    //    light_position2[0]=maxOrigin;
    //    light_position2[1]=maxOrigin;
    //    light_position2[2]=-maxOrigin;
    //    light_position2[3]=0;

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
    for(int i=0;i<meshes.size();++i)
        delete meshes[i];

    if(drawVertex!=nullptr)
        delete [] drawVertex;
    delete ui;

    QApplication::restoreOverrideCursor();
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



    //BACKGROUND SHADER START
    GLuint vertexShader2=f->glCreateShader(GL_VERTEX_SHADER);
    f->glShaderSource(vertexShader2, 1, &vertexShaderBackground, NULL);
    f->glCompileShader(vertexShader2);
    // Check for compile time errors

    f->glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    GLuint fragmentShader2 = f->glCreateShader(GL_FRAGMENT_SHADER);
    f->glShaderSource(fragmentShader2, 1, &fragmentShaderBackground, NULL);
    f->glCompileShader(fragmentShader2);
    // Check for compile time errors
    f->glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    shaderProgramBackground = f->glCreateProgram();
    f->glAttachShader(shaderProgramBackground, vertexShader2);
    f->glAttachShader(shaderProgramBackground, fragmentShader2);
    f->glLinkProgram(shaderProgramBackground);
    // Check for linking errors
    f->glGetProgramiv(shaderProgramBackground, GL_LINK_STATUS, &success);
    if (!success) {
        f->glGetProgramInfoLog(shaderProgramBackground, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    f->glDeleteShader(vertexShader2);
    f->glDeleteShader(fragmentShader2);

    //BACKGROUND SHADER END



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
    f->glGenVertexArrays(2, &background_vao);

    f->glUseProgram(shaderProgram);

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

    GPUobjectColor =  f->glGetUniformLocation(shaderProgram, "ourColor");
    GPUtransformMatrix=f->glGetUniformLocation(shaderProgram, "transMatrix");
    GPUprojectionMatrix=f->glGetUniformLocation(shaderProgram, "projMatrix");
    GPUlightPosition=f->glGetUniformLocation(shaderProgram, "LightPosition");
    GPUlightPosition2=f->glGetUniformLocation(shaderProgram, "LightPosition2");
    GPUlightColor=f->glGetUniformLocation(shaderProgram, "lightColor");

    f->glUseProgram(shaderProgramBackground);
    GPUtopcolor=f->glGetUniformLocation(shaderProgramBackground, "top_color");
    GPUbotcolor=f->glGetUniformLocation(shaderProgramBackground, "bot_color");


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



    f->glDisable(GL_DEPTH_TEST);
    f->glUseProgram(shaderProgramBackground);
    //draw background
    f->glUniform4f(GPUtopcolor,BACKGROUND_GRADIENT_TOP[0],BACKGROUND_GRADIENT_TOP[1],BACKGROUND_GRADIENT_TOP[2],1);
    f->glUniform4f(GPUbotcolor,BACKGROUND_GRADIENT_BOT[0],BACKGROUND_GRADIENT_BOT[1],BACKGROUND_GRADIENT_BOT[2],1);
    f->glBindVertexArray( background_vao );
    f-> glDrawArrays(GL_TRIANGLES, 0, 3);
 //   f->glBindVertexArray(0);
    f->glEnable(GL_DEPTH_TEST);

    f->glUseProgram(shaderProgram);
    m_transform.setToIdentity();
    m_projection.setToIdentity();

    tempRotationMatrix = tempRotationMatrix * getRotationMatrixFromRotationLine(x1Line-(minMaxXYZ[1] + minMaxXYZ[0])/2.0f,
                                                                                y1Line-(minMaxXYZ[3] + minMaxXYZ[2])/2.0f,
                                                                                z1Line-(minMaxXYZ[4] + minMaxXYZ[5])/2.0f,
                                                                                x2Line-(minMaxXYZ[1] + minMaxXYZ[0])/2.0f,
                                                                                y2Line-(minMaxXYZ[3] + minMaxXYZ[2])/2.0f,
                                                                                z2Line-(minMaxXYZ[4] + minMaxXYZ[5])/2.0f,
                                                                                tempAngleYRotation);
    m_transform = m_transform * tempRotationMatrix;
    m_transform.rotate(rotation);
   // tempRotationMatrix = tempRotationMatrix * getRotationMatrixFromRotationLine(76.960,54.643,46.698,25.577,59.829,46.194,tempAngleYRotation);

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
    f->glUniform3f(GPUlightPosition2,light_position2[0],light_position2[1],light_position2[2]);
    //set color light in geometry shader (white)
    f->glUniform3f(GPUlightColor,1,1,1);

    f->glBindVertexArray(VAO);

    if(isDrawFaces)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw faces first mesh
        f->glUniform3f(GPUobjectColor, 0.5f, 0.5f, 0.5f);   //color of faces first mesh
        f->glDrawArrays(GL_TRIANGLES , 0, sizeDrawVertex/6);

        //        //draw faces second mesh
        //        f->glUniform3f(GPUobjectColor, 0.0f, 0.5f, 1.0f);   //color of faces second mesh
        //        f->glDrawArrays(GL_TRIANGLES , sizeDrawVertexFirstObject/6, sizeDrawVertex/6);
    }

    if(isDrawGrid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //draw grid first mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 0.0f, 0.7f);   //color of grid first mesh
        f->glDrawArrays(GL_TRIANGLES , 0, sizeDrawVertex/6);

        //        //draw grid second mesh
        //        f->glUniform3f(GPUobjectColor, 0.0f, 1.0f, 0.0f);//color of grid second mesh
        //        f->glDrawArrays(GL_TRIANGLES , sizeDrawVertexFirstObject/6, sizeDrawVertex/6);
    }

    f->glBindVertexArray(0);
}



void OpenGlViewer::mouseMoveEvent(QMouseEvent *e) {

//    if (e->buttons() & Qt::LeftButton) {
//        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
//        rotation = QQuaternion::fromAxisAndAngle((QVector3D(diff.y(), diff.x(), 0.0).normalized() * diff.length() / 100.0).normalized(), rotationSpeed) * rotation;

//        mousePressPosition=QVector2D(e->localPos());
//        // Request an update
//        update();
//    }
        if (e->buttons() & Qt::LeftButton) {

            float scale=5;
            tempAngleYRotation = (e->localPos().y() - mousePressPosition.y())/scale;

         mousePressPosition=QVector2D(e->localPos());
            qDebug()<<"tempAngle y ROtation ="<<tempAngleYRotation;

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
    minMaxXYZ[0]=minMaxXYZ[2]=minMaxXYZ[4]=100000000;
    minMaxXYZ[1]=minMaxXYZ[3]=minMaxXYZ[5]=-100000000;

    for(int i=0;i<meshes.size();++i)
        findMinMaxForStl(meshes[i]);

    double distanceX=abs((minMaxXYZ[1]-minMaxXYZ[0]));
    double distanceY=abs((minMaxXYZ[3]-minMaxXYZ[2]));
    double distanceZ=abs((minMaxXYZ[5]-minMaxXYZ[4]));

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

    f->glBufferData(GL_ARRAY_BUFFER,  sizeDrawVertex * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    sizeDrawVertex=0;
    for(int i=0;i<meshes.size();++i)
        sizeDrawVertex+=n*meshes[i]->face.size();

    drawVertex=new GLfloat[sizeDrawVertex];

    int currentSize=0;
    int currentPosition=0;

    for(int i=0;i<meshes.size();++i)
    {
        currentSize=meshes[i]->face.size();

        for(int j=0;j<currentSize;++j)
        {
            drawVertex[currentPosition+n*j   ]=meshes[i]->face[j].P0(0).X();
            drawVertex[currentPosition+n*j+ 1]=meshes[i]->face[j].P0(0).Y();
            drawVertex[currentPosition+n*j+ 2]=meshes[i]->face[j].P0(0).Z();

            drawVertex[currentPosition+n*j+ 3]=meshes[i]->face[j].N().X();
            drawVertex[currentPosition+n*j+ 4]=meshes[i]->face[j].N().Y();
            drawVertex[currentPosition+n*j+ 5]=meshes[i]->face[j].N().Z();

            drawVertex[currentPosition+n*j+ 6]=meshes[i]->face[j].P0(1).X();
            drawVertex[currentPosition+n*j+ 7]=meshes[i]->face[j].P0(1).Y();
            drawVertex[currentPosition+n*j+ 8]=meshes[i]->face[j].P0(1).Z();

            drawVertex[currentPosition+n*j+ 9]=meshes[i]->face[j].N().X();
            drawVertex[currentPosition+n*j+10]=meshes[i]->face[j].N().Y();
            drawVertex[currentPosition+n*j+11]=meshes[i]->face[j].N().Z();

            drawVertex[currentPosition+n*j+12]=meshes[i]->face[j].P0(2).X();
            drawVertex[currentPosition+n*j+13]=meshes[i]->face[j].P0(2).Y();
            drawVertex[currentPosition+n*j+14]=meshes[i]->face[j].P0(2).Z();

            drawVertex[currentPosition+n*j+15]=meshes[i]->face[j].N().X();
            drawVertex[currentPosition+n*j+16]=meshes[i]->face[j].N().Y();
            drawVertex[currentPosition+n*j+17]=meshes[i]->face[j].N().Z();
        }
        currentPosition+=n*currentSize;
    }


    //    sizeDrawVertexFirstObject=drawFirstObject->face.size();

    //    for(int i=0;i<sizeDrawVertexFirstObject;++i)
    //    {
    //        drawVertex[n*i  ]=(*drawFirstObject).face[i].P0(0).X();
    //        drawVertex[n*i+1]=(*drawFirstObject).face[i].P0(0).Y();
    //        drawVertex[n*i+2]=(*drawFirstObject).face[i].P0(0).Z();

    //        drawVertex[n*i+3]=(*drawFirstObject).face[i].N().X();
    //        drawVertex[n*i+4]=(*drawFirstObject).face[i].N().Y();
    //        drawVertex[n*i+5]=(*drawFirstObject).face[i].N().Z();

    //        drawVertex[n*i+6]=(*drawFirstObject).face[i].P0(1).X();
    //        drawVertex[n*i+7]=(*drawFirstObject).face[i].P0(1).Y();
    //        drawVertex[n*i+8]=(*drawFirstObject).face[i].P0(1).Z();

    //        drawVertex[n*i+9]=(*drawFirstObject).face[i].N().X();
    //        drawVertex[n*i+10]=(*drawFirstObject).face[i].N().Y();
    //        drawVertex[n*i+11]=(*drawFirstObject).face[i].N().Z();

    //        drawVertex[n*i+12]=(*drawFirstObject).face[i].P0(2).X();
    //        drawVertex[n*i+13]=(*drawFirstObject).face[i].P0(2).Y();
    //        drawVertex[n*i+14]=(*drawFirstObject).face[i].P0(2).Z();

    //        drawVertex[n*i+15]=(*drawFirstObject).face[i].N().X();
    //        drawVertex[n*i+16]=(*drawFirstObject).face[i].N().Y();
    //        drawVertex[n*i+17]=(*drawFirstObject).face[i].N().Z();
    //    }
    //    sizeDrawVertexFirstObject*=n;

    //    if(drawSecondObject!=nullptr)
    //    {
    //        int  size2=drawSecondObject->face.size();

    //        for(int i=0;i<size2;++i)
    //        {
    //            drawVertex[sizeDrawVertexFirstObject + n*i  ]=(*drawSecondObject).face[i].P0(0).X();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+1]=(*drawSecondObject).face[i].P0(0).Y();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+2]=(*drawSecondObject).face[i].P0(0).Z();

    //            drawVertex[sizeDrawVertexFirstObject +n*i+3]=(*drawSecondObject).face[i].N().X();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+4]=(*drawSecondObject).face[i].N().Y();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+5]=(*drawSecondObject).face[i].N().Z();

    //            drawVertex[sizeDrawVertexFirstObject +n*i+6]=(*drawSecondObject).face[i].P0(1).X();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+7]=(*drawSecondObject).face[i].P0(1).Y();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+8]=(*drawSecondObject).face[i].P0(1).Z();

    //            drawVertex[sizeDrawVertexFirstObject +n*i+9]=(*drawSecondObject).face[i].N().X();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+10]=(*drawSecondObject).face[i].N().Y();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+11]=(*drawSecondObject).face[i].N().Z();

    //            drawVertex[sizeDrawVertexFirstObject +n*i+12]=(*drawSecondObject).face[i].P0(2).X();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+13]=(*drawSecondObject).face[i].P0(2).Y();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+14]=(*drawSecondObject).face[i].P0(2).Z();

    //            drawVertex[sizeDrawVertexFirstObject +n*i+15]=(*drawSecondObject).face[i].N().X();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+16]=(*drawSecondObject).face[i].N().Y();
    //            drawVertex[sizeDrawVertexFirstObject +n*i+17]=(*drawSecondObject).face[i].N().Z();
    //        }

    //    }
    f->glBufferData(GL_ARRAY_BUFFER,  sizeDrawVertex * sizeof(GLfloat), drawVertex, GL_STATIC_DRAW);
    update();
}

void OpenGlViewer::clearMeshesVector()
{
    for(int i=0;i<meshes.size();++i)
        delete meshes[i];
    meshes.clear();
}

QMatrix4x4 OpenGlViewer::getRotationMatrixFromRotationLine(float x1, float y1, float z1, float x2, float y2, float z2, float angle)
{
    float u =x2-x1;
    float v =y2-y1;
    float w =z2-z1;

    float theta =  angle / 180 * 3.14 ; //deg in radians

    float l = sqrt(u*u + v*v+ w*w);
    if(!l)
    {
        qDebug()<<"Error l ==0";
        return QMatrix4x4();
    }

    float u2=u*u;
    float v2=v*v;
    float w2=w*w;

    float cosT = cos(theta);

    float oneMinusCosT = 1 - cosT;
    float sinT=sin(theta);

    float l2 = u2+v2+w2;


    return QMatrix4x4((u2 + (v2 + w2) * cosT)/l2,(u*v * oneMinusCosT - w*l*sinT)/l2,(u*w * oneMinusCosT + v*l*sinT)/l2,((x1*(v2 + w2) - u*(y1*v + z1*w)) * oneMinusCosT+ (y1*w - z1*v)*l*sinT)/l2,
                      (u*v * oneMinusCosT + w*l*sinT)/l2,(v2 + (u2 + w2) * cosT)/l2, (v*w * oneMinusCosT - u*l*sinT)/l2,((y1*(u2 + w2) - v*(x1*u + z1*w)) * oneMinusCosT        + (z1*u - x1*w)*l*sinT)/l2,
                      (u*w * oneMinusCosT - v*l*sinT)/l2,(v*w * oneMinusCosT + u*l*sinT)/l2,(w2 + (u2 + v2) * cosT)/l2,((z1*(u2 + v2) - w*(x1*u + y1*v)) * oneMinusCosT + (x1*v - y1*u)*l*sinT)/l2,
                      0,0,0,1);
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

void OpenGlViewer::setRotationLineAxis(float x1, float y1, float z1, float x2, float y2, float z2)
{
    x1Line = x1;
    y1Line = y1;
    z1Line = z1;

    x2Line = x2;
    y2Line = y2;
    z2Line = z2;
}



bool OpenGlViewer::addMesh(QString path, bool isNeedToDraw)
{

    //    if(drawFirstObject!=nullptr)
    //        delete drawFirstObject;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    meshes.push_back(new MyMesh());

    if(vcg::tri::io::Importer<MyMesh>::Open(*meshes.back(),path.toLocal8Bit())) { // all the importers return 0 in case of success
        printf("Error in reading %s: '%s'\n");
        // QMessageBox::warning(this,"Error", "Can't open file "+path);
        delete meshes[meshes.size()-1];
        meshes.pop_back();
        QApplication::restoreOverrideCursor();
        return false;
    }
    if(meshes.back()->face.size()==0)
    {
        delete meshes[meshes.size()-1];
        meshes.pop_back();
        QApplication::restoreOverrideCursor();
        return false;
    }

    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*meshes.back());

    if(isNeedToDraw)
    {
        InitMaxOrigin();
        updateDrawVertex();
    }
    QApplication::restoreOverrideCursor();
    return true;
}



void OpenGlViewer::setLight(bool value)
{
    isLight=value;
    update();
}



void OpenGlViewer::saveFirstMesh()
{
    if(meshes.size()==0)
    {
        QMessageBox::warning(this, "Warning","Please, add at least one mesh");
        return;
    }
    QString resultPath=QFileDialog::getSaveFileName(this,"Save object","C://",tr("STL (*.stl);;PLY (*.ply)"));
    QFileInfo file1(resultPath);
    if(resultPath.isEmpty())
        return;
    if(file1.completeSuffix()=="ply")
    {
        vcg::tri::io::ExporterPLY<MyMesh>::Save(*meshes[0] ,resultPath.toLocal8Bit());
        return;
    }
    if(file1.completeSuffix()=="stl")
    {
        vcg::tri::io::ExporterSTL<MyMesh>::Save(*meshes[0] ,resultPath.toLocal8Bit());
        return;
    }
    QMessageBox::warning(this, "Warning extension","Please select object with another format");
    return;

}

void OpenGlViewer::alignSecondMesh(MyMesh * firstMesh=nullptr, MyMesh * secondMesh=nullptr,vcg::Matrix44d * resultTransformMatrix=nullptr, bool * isVisible=nullptr)
{

    if(firstMesh==nullptr || secondMesh==nullptr)
    {
        if(meshes.size()<2)
        {
            QMessageBox::warning(this, "Error align SecondMesh","Please, add at least two meshes");
            (*isVisible)=false;
            return;
        }
        firstMesh=meshes[0];
        secondMesh=meshes[1];
        distanceInfo.clear();
    }

    if(firstMesh->face.size()==0 || secondMesh->face.size()==0)
    {
        QMessageBox::warning(this, "Warning","Please, choose two objects");
        (*isVisible)=false;
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

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

    //ap.MaxIterNum=1000;
    // ap.TrgDistAbs=0.005;
    //  ap.EndStepNum=10;
    // ap.MinMinDistPerc=0.005;
    ap.UseVertexOnly=true;



    //set parameter value
    ap.SampleNum=5000;

    //timer->restart();
    // 1) Convert fixed mesh and put it into the grid.
    aa.convertMesh<MyMesh>(*firstMesh,fix);

    if((*firstMesh).fn==0 || ap.UseVertexOnly) {
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

    int quantityIteration=0;
    for(uint i=0;i<COUNT_ALIGN_CYCLES;++i)
    {
        aa.convertVertex((*secondMesh).vert,tmpmv);
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
        vcg::tri::UpdatePosition<MyMesh>::Matrix(*secondMesh, result.Tr, true);
        vcg::tri::UpdateBounding<MyMesh>::Box(*secondMesh);

        distance=result.computeAvgErr();

        if( isnan(distance.second) || isnan(distance.first))
        {
            if(isVisible!=nullptr)
                (*isVisible)=false;
            distanceInfo.append("Iteration="+QString::number(i+1)+" distance is Nan.[Bad mesh][Not included]\n");
            emit setDistanceInLabel(distanceInfo);
            QApplication::restoreOverrideCursor();
            return;
        }

        //        if(distance.first<previousError)
        //        {
        previousResult=result;
        previousError=distance.first;
        if(resultTransformMatrix!=nullptr)
            (*resultTransformMatrix)=(*resultTransformMatrix)*result.Tr;
        quantityIteration=i;
        if(distance.first<ERROR_ALIGN)
            break;
        //  }
        //        else{
        //            vcg::tri::UpdatePosition<MyMesh>::Matrix(*secondMesh,vcg::Inverse(result.Tr), true);
        //            vcg::tri::UpdatePosition<MyMesh>::Matrix(*secondMesh, previousResult.Tr, true);
        //            vcg::tri::UpdateBounding<MyMesh>::Box(*secondMesh);
        //            distance=previousResult.computeAvgErr();
        //            quantityIteration=i;
        //            break;
        //        }

    }
    // QString time= QString::number(timer->elapsed());

    if(distance.first>ERROR_ALIGN)
    {
        if(isVisible!=nullptr)
            (*isVisible)=true;
        distanceInfo.append("Iteration="+QString::number(quantityIteration+1)+" Distance="+QString::number(distance.first)+"[Bad mesh][Included]\n"); //Time="+time +"\n");
        emit setDistanceInLabel(distanceInfo);
    }
    else
    {
        if(isVisible!=nullptr)
            (*isVisible)=true;

        distanceInfo.append("Iteration="+QString::number(quantityIteration+1)+" Distance="+QString::number(distance.first)+"[Good mesh][Included]\n"); //Time="+time +"\n");
        emit setDistanceInLabel(distanceInfo);
    }
    updateDrawVertex();
    QApplication::restoreOverrideCursor();
}

void OpenGlViewer::appendSecondMeshToFirst(MyMesh * firstMesh, MyMesh * secondMesh)
{
    if(firstMesh==nullptr || secondMesh==nullptr)
    {
        if(meshes.size()<2)
        {
            QMessageBox::warning(this, "Error align SecondMesh","Please, add at least two meshes");
            return;
        }
        firstMesh=meshes[0];
        secondMesh=meshes[1];
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    vcg::tri::Append<MyMesh, MyMesh>::Mesh(*firstMesh,*secondMesh);

    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*firstMesh);

    delete meshes[1];   //delete object that meshes[1] pointed to
    meshes.erase(meshes.begin()+1);         //erase pointer from array
    //  emit setDistanceInLabel(QString(""));
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

void OpenGlViewer::addedMeshesToAlign(QStringList meshesList, QString path)
{
    if(path.isEmpty())
        return;
    vectorContentMLP.clear();
    QString exportPath=path.split(".")[0]+".mlp";
    QFileInfo fileInfo(path);
    //get path to dir where file with meshes located
    const QString pathToDir=path.mid(0,path.size()-fileInfo.fileName().size());

    QStringList meshesNeedToAlign;  //will contain all meshes that need to align without duplicate
    bool isNeedToAdd=true;
    for(int i=0;i<meshesList.size();++i)
    {
        isNeedToAdd=true;
        for(int j=0;j<WRvectorFileNames.size();++j)
            if(meshesList[i]==WRvectorFileNames[j])
            {
                isNeedToAdd=false;
                break;
            }
        if(isNeedToAdd)
            meshesNeedToAlign.append(meshesList[i]);
    }


    if(meshesNeedToAlign.isEmpty())
        return;

    vcg::Matrix44d tempMatrix;
    tempMatrix.SetIdentity();
    bool isVisible=false;

    int currentIndex=0;
    if(WRvectorFileNames.empty())
    {
        for(int i=0;i<meshesNeedToAlign.size();++i)
            if(addMesh(pathToDir+meshesNeedToAlign[i],false))
            {
                currentIndex=i+1;
                WRvectorFileNames.push_back(meshesNeedToAlign[i]);
                WRvectorMatrix.push_back(tempMatrix);
                WRvectorVisible.push_back(true);
                break;
            }
    }
    if(meshesNeedToAlign.size()>currentIndex)
    {
        for(int i=currentIndex;i<meshesNeedToAlign.size();++i)
        {

            if(meshesNeedToAlign[i].isEmpty() || meshesNeedToAlign[i]=="\r\n" || meshesNeedToAlign[i]=="\n")
                continue;

            if(!QFileInfo(pathToDir+meshesNeedToAlign[i]).exists() || !addMesh(pathToDir+meshesNeedToAlign[i],false))
                continue;

            alignSecondMesh(meshes[meshes.size()-1],meshes[meshes.size()-2],&tempMatrix,&isVisible);

            if(isVisible)
            {
                for(int i=0;i<WRvectorMatrix.size();++i)
                {
                    if(WRvectorVisible[i])
                        WRvectorMatrix[i]*=tempMatrix;
                }
                if(meshes.size()>2)
                {
                    for(int i=0;i<meshes.size()-2;++i)
                    {
                        vcg::tri::UpdatePosition<MyMesh>::Matrix(*meshes[i], tempMatrix, true);
                        vcg::tri::UpdateBounding<MyMesh>::Box(*meshes[i]);
                    }
                }
                WRvectorFileNames.push_back(meshesNeedToAlign[i]);

                tempMatrix.SetIdentity();
                WRvectorMatrix.push_back(tempMatrix);
                WRvectorVisible.push_back(true);
                // vectorContentMLP.push_back({fileName,vcgMatrixToString(tempMatrix),"1"});  //if visible push actual matrix data with visible ==1
                //appendSecondMeshToFirst();
                //                if(drawFirstObject!=nullptr)
                //                    delete drawFirstObject;
                //                drawFirstObject=drawSecondObject;
                //                drawSecondObject=nullptr;
            }
            else
            {
                WRvectorFileNames.push_back(meshesNeedToAlign[i]);

                tempMatrix.SetIdentity();
                WRvectorMatrix.push_back(tempMatrix);
                WRvectorVisible.push_back(false);
                // vectorContentMLP.push_back({fileName,identityMatrix,"0"}); //if non visible, - push Matrix Identity with visible ==0
            }
        }
    }
//    else
//    {
//        InitMaxOrigin();
//        updateDrawVertex();
//        return;
//    }

    if(WRvectorMatrix.size()!=WRvectorVisible.size() || WRvectorMatrix.size()!=WRvectorFileNames.size())
    {
        QMessageBox::warning(this,"Error size incorrect", "Send it to developer error in code");
        return;
    }

    for(int i=0;i<WRvectorMatrix.size();++i)
        vectorContentMLP.push_back({WRvectorFileNames[i],vcgMatrixToString(WRvectorMatrix[i]),WRvectorVisible[i]?"1":"0"});

    exportAsMLP(exportPath);
    InitMaxOrigin();
    updateDrawVertex();
}

void OpenGlViewer::clearMeshes()
{
    clearMeshesVector();
    distanceInfo.clear();
    vectorContentMLP.clear();
    WRvectorFileNames.clear();
    WRvectorMatrix.clear();
    WRvectorVisible.clear();
    //    if(drawFirstObject!=nullptr)
    //        delete drawFirstObject;
    //    if(drawSecondObject!=nullptr)
    //        delete drawSecondObject;
    emit setDistanceInLabel(" ");
    updateDrawVertex();
}

void OpenGlViewer::openAlignFile()
{

    clearMeshesVector();
    distanceInfo.clear();
    vectorContentMLP.clear();
    //    std::vector<QString> vectorFileNames;
    //    std::vector<vcg::Matrix44d> vectorMatrix;
    //    std::vector<bool> vectorVisible;
    //path to file with meshes that need to align
    QString pathAlignMeshes=QFileDialog::getOpenFileName(this,"Open file with collect of meshes" );
    //QString exportPath=pathAlignMeshes.split(".")[0]+".mlp";
    QStringList stringList;
    if(!pathAlignMeshes.isEmpty())
    {
        QFile file(pathAlignMeshes);
        QByteArray data;
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd())
                stringList.append(QString::fromStdString(file.readLine().toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0]);
        }
        file.close();

        //emit infoDisplay("MessageParse start");
        // for(auto &i:stringList)
        //   emit infoDisplay(i);
        //emit infoDisplay("MessageParse end");
        //   qDebug()<<data;
        addedMeshesToAlign(stringList,pathAlignMeshes);
        // qDebug()<<"Emit signal append mesh 2";
    }
    // addedMeshesToAlign(QStringList(),pathAlignMeshes);

    //    QFileInfo fileInfo(pathAlignMeshes);

    //    //get path to dir where file with meshes located
    //    const QString pathToDir=pathAlignMeshes.mid(0,pathAlignMeshes.size()-fileInfo.fileName().size());

    //    if(pathAlignMeshes.isEmpty())
    //        return;

    //    QFile fileWithAlignMeshes(pathAlignMeshes);
    //    if(fileWithAlignMeshes.open(QIODevice::ReadOnly))
    //    {
    //        QString fileName;
    //        QByteArray isEmpty;
    //        do{
    //            isEmpty=fileWithAlignMeshes.readLine();
    //            if(isEmpty.isEmpty() || isEmpty=="\r\n" || isEmpty=="\n")
    //                continue;

    //            fileName=QString::fromStdString(isEmpty.toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0];

    //            if(!QFileInfo(pathToDir+fileName).exists())
    //                continue;
    //        }while(!addMesh(pathToDir+fileName,false) && !fileWithAlignMeshes.atEnd());

    //        // vectorContentMLP.push_back({fileName,identityMatrix,"1"}); //add First mesh to MPL vector with Identity matrix

    //        //read all meshes from file and import it
    //        vcg::Matrix44d tempMatrix;
    //        tempMatrix.SetIdentity();

    //        bool isVisible=false;
    //        vectorFileNames.push_back(fileName);
    //        vectorMatrix.push_back(tempMatrix);
    //        vectorVisible.push_back(true);

    //        while(!fileWithAlignMeshes.atEnd())
    //        {
    //            isEmpty=fileWithAlignMeshes.readLine();
    //            if(isEmpty.isEmpty() || isEmpty=="\r\n" || isEmpty=="\n")
    //                continue;

    //            fileName=QString::fromStdString(isEmpty.toStdString()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)[0];

    //            if(!QFileInfo(pathToDir+fileName).exists() || !addMesh(pathToDir+fileName,false))
    //                continue;

    //            alignSecondMesh(meshes[meshes.size()-1],meshes[meshes.size()-2],&tempMatrix,&isVisible);

    //            if(isVisible)
    //            {
    //                for(int i=0;i<vectorMatrix.size();++i)
    //                {
    //                    if(vectorVisible[i])
    //                        vectorMatrix[i]*=tempMatrix;
    //                }
    //                vectorFileNames.push_back(fileName);

    //                tempMatrix.SetIdentity();
    //                vectorMatrix.push_back(tempMatrix);
    //                vectorVisible.push_back(true);
    //                // vectorContentMLP.push_back({fileName,vcgMatrixToString(tempMatrix),"1"});  //if visible push actual matrix data with visible ==1
    //                //appendSecondMeshToFirst();
    //                //                if(drawFirstObject!=nullptr)
    //                //                    delete drawFirstObject;
    //                //                drawFirstObject=drawSecondObject;
    //                //                drawSecondObject=nullptr;
    //            }
    //            else
    //            {
    //                vectorFileNames.push_back(fileName);

    //                tempMatrix.SetIdentity();
    //                vectorMatrix.push_back(tempMatrix);
    //                vectorVisible.push_back(false);
    //                // vectorContentMLP.push_back({fileName,identityMatrix,"0"}); //if non visible, - push Matrix Identity with visible ==0
    //            }
    //        }
    //    }
    //    if(vectorMatrix.size()!=vectorVisible.size() || vectorMatrix.size()!=vectorFileNames.size())
    //    {
    //        QMessageBox::warning(this,"Error size incorrect", "Send it to developer error in code");
    //        return;
    //    }

    //    for(int i=0;i<vectorMatrix.size();++i)
    //        vectorContentMLP.push_back({vectorFileNames[i],vcgMatrixToString(vectorMatrix[i]),vectorVisible[i]?"1":"0"});

    //    fileWithAlignMeshes.close();
    //    exportAsMLP(exportPath);

    //    InitMaxOrigin();
    //    updateDrawVertex();
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


//void OpenGlViewer::drawFirstMesh()
//{
//    uint size=drawFirstObject->face.size();
//    glBegin(GL_TRIANGLES);  // START TRIANGLES DRAWING
//    for (uint i = 0; i < size; ++i) {
//        glNormal3f((*drawFirstObject).face[i].N().X(),(*drawFirstObject).face[i].N().Y(),(*drawFirstObject).face[i].N().Z());
//        glVertex3f((*drawFirstObject).face[i].P0(0).X(),(*drawFirstObject).face[i].P0(0).Y(),(*drawFirstObject).face[i].P0(0).Z());
//        glVertex3f((*drawFirstObject).face[i].P0(1).X(),(*drawFirstObject).face[i].P0(1).Y(),(*drawFirstObject).face[i].P0(1).Z());
//        glVertex3f((*drawFirstObject).face[i].P0(2).X(),(*drawFirstObject).face[i].P0(2).Y(),(*drawFirstObject).face[i].P0(2).Z());
//    }
//    glEnd();  // END TRIANGLES DRAWING
//}

//void OpenGlViewer::drawSecondMesh()
//{
//    uint size=drawSecondObject->face.size();
//    glBegin(GL_TRIANGLES);  // START TRIANGLES DRAWING
//    for (uint i = 0; i < size; ++i) {
//        glNormal3f((*drawSecondObject).face[i].N().X(),(*drawSecondObject).face[i].N().Y(),(*drawSecondObject).face[i].N().Z());
//        glVertex3f((*drawSecondObject).face[i].P0(0).X(),(*drawSecondObject).face[i].P0(0).Y(),(*drawSecondObject).face[i].P0(0).Z());
//        glVertex3f((*drawSecondObject).face[i].P0(1).X(),(*drawSecondObject).face[i].P0(1).Y(),(*drawSecondObject).face[i].P0(1).Z());
//        glVertex3f((*drawSecondObject).face[i].P0(2).X(),(*drawSecondObject).face[i].P0(2).Y(),(*drawSecondObject).face[i].P0(2).Z());
//    }
//    glEnd();  // END TRIANGLES DRAWING
//}
//bool OpenGlViewer::setSecondMesh(QString path,bool isNeedToDraw)
//{

//    if(drawFirstObject==nullptr)
//    {
//        QMessageBox::warning(this, "Warning","Please, choose first object");
//        return false;
//    }
//    //    if(drawSecondObject!=nullptr)
//    //        delete drawSecondObject;
//    QApplication::setOverrideCursor(Qt::WaitCursor);

//    drawSecondObject=new MyMesh();

//    if(vcg::tri::io::Importer<MyMesh>::Open(*drawSecondObject,path.toLocal8Bit())) { // all the importers return 0 in case of success
//        drawSecondObject->Clear();
//        printf("Error in reading %s: '%s'\n");
//        // QMessageBox::warning(this,"Error", "Can't open file "+path);
//        QApplication::restoreOverrideCursor();
//        return false;
//    }
//    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*drawSecondObject);

//    if(isNeedToDraw)
//    {
//        InitMaxOrigin();
//        updateDrawVertex();
//    }
//    QApplication::restoreOverrideCursor();
//    return true;

//}
//void OpenGlViewer::saveSecondMesh()
//{
//    if(drawSecondObject==nullptr)
//    {
//        QMessageBox::warning(this, "Error","Second object ==nullptr");
//        return;
//        return;
//    }
//    if(drawSecondObject->fn==0)
//    {
//        QMessageBox::warning(this, "Warning","Please, choose second object");
//        return;
//    }
//    QString resultPath=QFileDialog::getSaveFileName(this,"Save object","C://",tr("STL (*.stl);;PLY (*.ply)"));
//    QFileInfo file1(resultPath);
//    if(resultPath.isEmpty())
//        return;
//    if(file1.completeSuffix()=="ply")
//    {
//        vcg::tri::io::ExporterPLY<MyMesh>::Save(*drawSecondObject ,resultPath.toLocal8Bit());
//        return;
//    }
//    if(file1.completeSuffix()=="stl")
//    {
//        vcg::tri::io::ExporterSTL<MyMesh>::Save(*drawSecondObject ,resultPath.toLocal8Bit());
//        return;
//    }
//    QMessageBox::warning(this, "Warning extension","Please select object with another format");
//    return;

//}
