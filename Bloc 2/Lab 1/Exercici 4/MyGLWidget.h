// MyGLWidget.h
#include "BL2GLWidget.h"
#include "../Model/model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    virtual void initializeGL ( );
    
        virtual void paintGL ( );
    
    virtual void carregaShaders();
    
    virtual void creaBuffers ();
    
    virtual void projectTransform();
    
    virtual void viewTransform();
    
    virtual void keyPressEvent (QKeyEvent *event);
    
    Model m; // un únic model
    
    GLuint VAO_Homer;
    
    GLuint projLoc, viewLoc;
};
