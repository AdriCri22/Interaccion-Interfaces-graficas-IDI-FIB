#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {

}

void MyLabel::clearValue() {
    setText("-");
}

void MyLabel::setValue() {
    emit clearAll();
    setText("o");
    if ((rand() & 1) == 0) emit Left();
    else emit Right();
}

void MyLabel::clearAllLabels() {
    setText("-");
    emit clearAll();
}
