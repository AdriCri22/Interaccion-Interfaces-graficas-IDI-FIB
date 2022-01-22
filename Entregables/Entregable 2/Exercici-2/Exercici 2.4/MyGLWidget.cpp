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

MyGLWidget::~MyGLWidget()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  LL2GLWidget::carregaShaders();
  LL2GLWidget::creaBuffersHomer();
  LL2GLWidget::creaBuffersTerra();
	LL2GLWidget::iniEscena();
  iniCamera();
}

void MyGLWidget::iniCamera(){

  obs = glm::vec3(0, 15, 15);
  vrp = glm::vec3(0, 0, 0.5);
  up = glm::vec3(0, 1, 0);
  fov = float(M_PI)/4.0f;
  ra  = 1.0;
  znear =  0.001;
  zfar  = 30;

  viewTransform();
  projectTransform();
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Homer
	for (int i = 0; i < n_homers; i++) {
		float alpha = i * 2*M_PI/n_homers;
		if ((0 <= alpha and alpha < M_PI/2) or (M_PI <= alpha and alpha < 3*M_PI/2))
			homerTransform(alpha + r, glm::vec3(-h * sin(alpha) , 0, -h * cos(alpha)));

		else 
			homerTransform(alpha + r, glm::vec3(-h * cos(alpha - M_PI/2) , 0, h * sin(alpha - M_PI/2)));
			
		glBindVertexArray (VAO_Homer);
		glDrawArrays(GL_TRIANGLES, 0, modelHomer.faces().size()*3);
		glBindVertexArray (0);
	}

  // Terra
  terraTransform();
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray (0);
}

void MyGLWidget::homerTransform (float alpha, glm::vec3 pos)
{
  // Matriu de transformació de Homer
  glm::mat4 TG(1.0f);
	TG = glm::translate(TG, pos);
	TG = glm::rotate(TG, alpha, glm::vec3(0, 1, 0));
	TG = glm::scale(TG, glm::vec3(1.83/midaModelHomer));
  TG = glm::translate(TG, -baseHomer);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::terraTransform ()
{
  // Matriu de transformació de l'escac
  glm::mat4 TG(1.0f);
	TG = glm::translate(TG, glm::vec3(-midaTerra/2, 0, -midaTerra/2));
  TG = glm::scale(TG, glm::vec3(midaTerra, 1.0, midaTerra));
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
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
	View = glm::translate(View, glm::vec3(0, 0, -20));
	View = glm::rotate(View, theta, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -psi, glm::vec3(0, 1, 0));
	View = glm::translate(View, -vrp);
	//View = glm::lookAt (obs, vrp, up);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Plus :
      if (n_homers < 15) n_homers += 1;
      break;
    case Qt::Key_Minus :
      if (1 < n_homers) n_homers -= 1;
      break;
    case Qt::Key_Up :
      if (h < 5) h += 0.5;
      break;
    case Qt::Key_Down :
      if (1.5 < h) h -= 0.5;
      break;
    case Qt::Key_Right :
      r -= M_PI/36;
      break;
    case Qt::Key_Left :
      r += M_PI/36;
      break;
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

  psi += (e->x() - xClick) * (M_PI/180);
  theta -= (e->y() - yClick) * (M_PI/180);

  xClick = e->x();
  yClick = e->y();

  viewTransform();
  update ();
}
