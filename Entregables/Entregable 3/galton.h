#ifndef GALTON_H
#define GALTON_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Galton; }
QT_END_NAMESPACE

class Galton : public QMainWindow
{
    Q_OBJECT

public:
    Galton(QWidget *parent = nullptr);
    ~Galton();

public slots:

signals:
    void throwBall();
    void clearAll();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Galton *ui;

};
#endif // GALTON_H
