#ifndef LCDNUMBER_H
#define LCDNUMBER_H

#include<QLCDNumber>

class LCDNumber : public QLCDNumber {
    Q_OBJECT

public:
    LCDNumber(QWidget *parent);
    int count = 0;

public slots:
    void setValue();
    void clearValue();

signals:

};

#endif // LCDNUMBER_H
