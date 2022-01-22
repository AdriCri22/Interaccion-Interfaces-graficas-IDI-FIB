// MyGLWidget.h
#include "BL2GLWidget.h"
#include "../../Model/model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    
    virtual void iniCamera ();
    virtual void creaBuffers ();
    virtual void carregaShaders ();
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void modelTransform();
    virtual void calculaCapsaModel ();
    
    // uniform locations
    GLuint viewLoc, projLoc;
    
    // VAO i VBO names
    GLuint VAO;
    
    glm::vec3 baseModel;
    float midaModel;
    Model model;
    
    // Internal vars
    float fov, ra, znear, zfar;
    glm::vec3 obs, vrp, up;
};
