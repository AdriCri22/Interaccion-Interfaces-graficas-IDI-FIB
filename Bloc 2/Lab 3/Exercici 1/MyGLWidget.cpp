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
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  
  m.load ("../../Model/Patricio.obj");
  
  creaBuffers();
  escala = 1.0f;
  
  glm::vec3 centre, base;
  calcula_base_radi(m, centre, base);
  
  projectTransform();
  viewTransform();
}

void MyGLWidget::paintGL () 
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform ();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO_Patricio);

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size () * 3);

  glBindVertexArray (0);
}

void MyGLWidget::creaBuffers () 
{
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Patricio);
  glBindVertexArray(VAO_Patricio);

  GLuint VBO_Patricio[2];
  glGenBuffers(2, VBO_Patricio);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size () * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW); // posició

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_matdiff (), GL_STATIC_DRAW); // color

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::projectTransform () {
    float FOV = float(M_PI) / 3.0f;
    float ra = 1.0f;
    float znear = 0.4f;
    float zfar = 5.0f;
    
    glm::mat4 Proj = glm::perspective (FOV, ra, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
    glm::vec3 OBS (0, 0, 3);
    glm::vec3 VRP (0, 0, 0);
    glm::vec3 UP (0, 1, 0);
    
    glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calcula_capsa(Model &m, glm::vec3 &min, glm::vec3 &max) {
    min = glm::vec3(FLT_MAX);
    max = glm::vec3(-FLT_MAX);
    for (auto it = m.vertices().begin(); it < m.vertices().end(); it++) {
        if (max.x < *it) max.x = *it;
        else if (min.x > *it) min.x = *it;
        it++;
        if (max.y < *it) max.y = *it;
        else if (min.y > *it) min.y = *it;
        it++;
        if (max.z < *it) max.z = *it;
        else if (min.z > *it) min.z = *it;
    }
}

void MyGLWidget::calcula_base_radi(Model &m, glm::vec3 &base, glm::vec3 &centre) {
    glm::vec3 min, max;
    calcula_capsa(m, min, max);
    base = glm::vec3((max.x - min.x)/2, min.y, (max.z - min.z)/2);
    centre = glm::vec3((max.x - min.x)/2, (max.y - min.y)/2, (max.z - min.z)/2);
    std::cout << centre.x << " " << centre.y << " " << centre.z << std::endl;
}


void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  BL2GLWidget::keyPressEvent(event);
}
