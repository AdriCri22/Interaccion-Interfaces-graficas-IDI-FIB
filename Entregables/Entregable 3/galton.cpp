#include "galton.h"
#include "ui_galton.h"

Galton::Galton(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Galton)
{
    ui->setupUi(this);
}

Galton::~Galton()
{
    delete ui;
}

void Galton::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() == Qt::ALT) && (event->key() == Qt::Key_B))
    {
        emit throwBall();
    }

    if ((event->modifiers() == Qt::ALT) && (event->key() == Qt::Key_R))
    {
        emit clearAll();
    }
}
