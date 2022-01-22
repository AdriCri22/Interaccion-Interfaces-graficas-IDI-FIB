// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL ( );
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintGL ( );
    virtual void modelTransformPatricio ();
    virtual glm::mat4 calculaPatricioTG();

    void carregaShaders ();
    void actualitzaFocus();

    GLuint posFocus1Loc, posFocus2Loc, posFocus3Loc;
    GLuint colorFocus1Loc, colorFocus2Loc, colorFocus3Loc;
    float x = 5, z = 2, rot = 0;
    glm::vec3 colorFocus1 = glm::vec3(0.9, 0.2, 0.2);
    glm::vec3 colorFocus2 = glm::vec3(0.2, 0.9, 0.2);
    glm::vec3 colorFocus3 = glm::vec3(0.2, 0.2, 0.9);

  private:
    int printOglError(const char file[], int line, const char func[]);

};
