#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

public:
    MyLabel(QWidget *parent);

public slots:
    void setValue();
    void clearValue();
    void clearAllLabels();

signals:
    void Left();
    void Right();
    void clearAll();

};

#endif // MYLABEL_H
