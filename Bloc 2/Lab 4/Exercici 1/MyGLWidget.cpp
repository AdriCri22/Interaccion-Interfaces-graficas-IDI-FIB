// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
    if (program != NULL)
        delete program;
}

void MyGLWidget::initializeGL ()
{
    // Cal inicialitzar l'ús de les funcions d'OpenGL
    initializeOpenGLFunctions();  
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
    carregaShaders();
    creaBuffers();
    iniCamera();
    escala = 1.0f;
}

void MyGLWidget::iniCamera(){
    obs = glm::vec3(0, 6, 6);
    vrp = glm::vec3(0, 0, 0.5);
    up = glm::vec3(0, 1, 0);
    fov = float(M_PI)/4.0f;
    ra  = 1.0;
    znear =  0.001;
    zfar  = 15;

    viewTransform();
    projectTransform();
}

void MyGLWidget::paintGL() {
    
    // Borramos el frame-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    modelTransform();
    glBindVertexArray (VAO);
    glDrawArrays(GL_TRIANGLES, 0, model.faces().size()*3);
    glBindVertexArray (0);
}

void MyGLWidget::modelTransform ()
{
  // Matriz de transformación
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(2.0 * 1.0/midaModel));
  TG = glm::translate(TG, -baseModel);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriz de posición y orientación del observador
  glm::mat4 View(1.0f);
  View = glm::lookAt (obs, vrp, up);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv (GL_VIEWPORT, vp);
    ample = vp[2];
    alt = vp[3];
#else
    ample = w;
    alt = h;
#endif
    ra = float(ample)/float(alt);
    projectTransform();
}

void MyGLWidget::creaBuffers() {
    
    // Carga el modelo
    model.load("../../Model/legoman.obj");
    
    calculaCapsaModel();
    
    // Creación de VAOs i VBOs para pintar
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    GLuint VBO[2];
    glGenBuffers(2, VBO);
    
    // Geometria
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3,
           model.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);
    
    // Color
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model.faces().size() 
            * 3 * 3, model.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    
    glBindVertexArray(0);
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    
    // Identificadores de los uniform locations (El TG ya está en BL2GLWidget)
    projLoc  = glGetUniformLocation (program->programId(), "Proj");
    viewLoc  = glGetUniformLocation (program->programId(), "View");
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = model.vertices()[0];
  miny = maxy = model.vertices()[1];
  minz = maxz = model.vertices()[2];
  for (unsigned int i = 3; i < model.vertices().size(); i+=3)
  {
    if (model.vertices()[i+0] < minx)
      minx = model.vertices()[i+0];
    if (model.vertices()[i+0] > maxx)
      maxx = model.vertices()[i+0];
    if (model.vertices()[i+1] < miny)
      miny = model.vertices()[i+1];
    if (model.vertices()[i+1] > maxy)
      maxy = model.vertices()[i+1];
    if (model.vertices()[i+2] < minz)
      minz = model.vertices()[i+2];
    if (model.vertices()[i+2] > maxz)
      maxz = model.vertices()[i+2];
  }
  midaModel = maxy-miny;
  baseModel[0]   = (maxx+minx)/2.0;
  baseModel[1]   = miny;
  baseModel[2]   = (maxz+minz)/2.0;
}
