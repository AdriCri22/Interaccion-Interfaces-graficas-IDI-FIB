// MyGLWidget.h
#include "BL2GLWidget.h"
#include <limits>
#include "../..//Model/model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
    // const float FLT_MAX = std::numeric_limits<float>::min();
    
    virtual void initializeGL ( );
    
        virtual void paintGL ( );
    
    virtual void carregaShaders();
    
    virtual void creaBuffers ();
    
    virtual void projectTransform();
    
    virtual void viewTransform();
    
    virtual void keyPressEvent (QKeyEvent *event);
    
    void calcula_capsa(Model &m, glm::vec3 &min, glm::vec3 &max);
    void calcula_base_radi(Model &m, glm::vec3 &base, glm::vec3 &centre);
    
    Model m; // un únic model
    
    GLuint VAO_Patricio;
    
    GLuint projLoc, viewLoc;
};
