#include "lcdnumber.h"

LCDNumber::LCDNumber(QWidget *parent) : QLCDNumber(parent) {

}

void LCDNumber::clearValue() {
    count = 0;
    display(count);
}

void LCDNumber::setValue() {
    count++;
    display(count);
}
