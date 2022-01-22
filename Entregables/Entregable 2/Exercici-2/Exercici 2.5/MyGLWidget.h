#include "LL2GLWidget.h"
#include <math.h>
#include "model.h"


class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
    virtual void initializeGL ();
    virtual void iniCamera();
    virtual void paintGL ();
    virtual void homerTransform (float alpha, glm::vec3 pos);
    virtual void terraTransform ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *e);
    virtual void viewTransform ();
    virtual void projectTransform ();

    float theta = float (M_PI/4);
    float psi = 0;
    float h = 4;    // radi
    float n_homers = 10;  // Número de Homers
    float r = 0;  // rotate
    bool ortho = false;
};
