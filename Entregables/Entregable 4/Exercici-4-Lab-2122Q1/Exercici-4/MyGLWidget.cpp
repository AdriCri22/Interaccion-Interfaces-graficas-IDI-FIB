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

void MyGLWidget::initializeGL () {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();

  glm::vec3 posFocus1 = glm::vec3(0, 0, 0);
  glUniform3fv(posFocus1Loc, 1, &posFocus1[0]);
  glm::vec3 posFocus2 = glm::vec3(8.0, 3.0, 2.0);
  glUniform3fv(posFocus2Loc, 1, &posFocus2[0]);
  glm::vec3 posFocus3 = glm::vec3(dimCapsaPatr[0]/2, dimCapsaPatr[1]/2, dimCapsaPatr[2]/2);
  glUniform3fv(posFocus3Loc, 1, &posFocus3[0]);

  glUniform3fv(colorFocus1Loc, 1, &colorFocus1[0]);
  glUniform3fv(colorFocus2Loc, 1, &colorFocus2[0]);
  glUniform3fv(colorFocus3Loc, 1, &colorFocus3[0]);

  iniEscena();
  iniCamera();

  glm::vec3 a = glm::vec3(1, 1, 0);
  glm::vec3 b = glm::vec3(1, 1, 1);
  glm::vec3 v = a * b;
  std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;

  v = glm::cross(a, b);
  std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;

  float v2 = glm::dot(a, b);
  std::cout << v2 << std::endl;
}

void MyGLWidget::actualitzaFocus() {
  /*glm::mat3 NormalMatrix = glm::inverse(glm::transpose(glm::mat3(View * patricioTG)));
  glUniformMatrix3fv (NormalMatrixLoc, 1, GL_FALSE, &NormalMatrix[0][0]);
  glm::vec4 posFocusSCO = glm::vec4(0.0, 0.0, 0.0, 1.0);
  glUniform4fv (posFocusLoc, 1, &posFocusSCO[0]);*/
}

void MyGLWidget::paintGL () {
  LL4GLWidget::paintGL ();
  actualitzaFocus();
}

glm::mat4 MyGLWidget::calculaPatricioTG()
{
  glm::mat4 tg = glm::translate(glm::mat4(1.f), glm::vec3(x,0,z));
  tg = glm::rotate(tg, rot, glm::vec3(0, 1, 0));
  tg = glm::scale(tg, glm::vec3(escala, escala, escala));
  tg = glm::translate(tg, -centreBasePatr);
  return tg;
}

void MyGLWidget::modelTransformPatricio ()
{
  patricioTG = calculaPatricioTG();  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &patricioTG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {

  case Qt::Key_Right:
    if (x < 9) {
      x += 1;
      modelTransformPatricio();
      glm::vec3 posFocus3 = glm::vec3(dimCapsaPatr[0]/2+x-5, dimCapsaPatr[1]/2, dimCapsaPatr[2]/2+z-2);
      glUniform3fv(posFocus3Loc, 1, &posFocus3[0]);
    }
    break;

  case Qt::Key_Left:
    if (x > 1) {
      x -= 1;
      modelTransformPatricio();
      glm::vec3 posFocus3 = glm::vec3(dimCapsaPatr[0]/2+x-5, dimCapsaPatr[1]/2, dimCapsaPatr[2]/2+z-2);
      glUniform3fv(posFocus3Loc, 1, &posFocus3[0]);
    }
    break;

  case Qt::Key_Up:
    if (z > 1) {
      z -= 1;
      modelTransformPatricio();
      glm::vec3 posFocus3 = glm::vec3(dimCapsaPatr[0]/2+x-5, dimCapsaPatr[1]/2, dimCapsaPatr[2]/2+z-2);
      glUniform3fv(posFocus3Loc, 1, &posFocus3[0]);
    }    
    break;

  case Qt::Key_Down:
    if (z < 9) {
      z += 1;
      modelTransformPatricio();
      glm::vec3 posFocus3 = glm::vec3(dimCapsaPatr[0]/2+x-5, dimCapsaPatr[1]/2, dimCapsaPatr[2]/2+z-2);
      glUniform3fv(posFocus3Loc, 1, &posFocus3[0]);
    } 
    break;

  case Qt::Key_R:
      rot += 45*M_PI/180;
      modelTransformPatricio();
    break;

  case Qt::Key_C:
    if (colorFocus1[0] == 0.0)
      colorFocus1 = glm::vec3(0.9, 0.2, 0.2);
    else
      colorFocus1 = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(colorFocus1Loc, 1, &colorFocus1[0]);
    break;

  case Qt::Key_E:
    if (colorFocus2[0] == 0.0)
      colorFocus2 = glm::vec3(0.2, 0.9, 0.2);
    else
      colorFocus2 = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(colorFocus2Loc, 1, &colorFocus2[0]);
    break;

  case Qt::Key_P:
    if (colorFocus3[0] == 0.0)
      colorFocus3 = glm::vec3(0.2, 0.2, 0.9);
    else
      colorFocus3 = glm::vec3(0.0, 0.0, 0.0);
    glUniform3fv(colorFocus3Loc, 1, &colorFocus3[0]);
    break;

  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::carregaShaders()
{
  LL4GLWidget::carregaShaders();
  posFocus1Loc = glGetUniformLocation (program->programId(), "posFocus1");
  posFocus2Loc = glGetUniformLocation (program->programId(), "posFocus2");
  posFocus3Loc = glGetUniformLocation (program->programId(), "posFocus3");
  colorFocus1Loc = glGetUniformLocation (program->programId(), "colorFocus1");
  colorFocus2Loc = glGetUniformLocation (program->programId(), "colorFocus2");
  colorFocus3Loc = glGetUniformLocation (program->programId(), "colorFocus3");
}
