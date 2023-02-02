#include "MyTimer.h"

MyTimer::MyTimer(uint16_t dl) {
	dlay=dl;
}

void MyTimer::run() {

    if ((millis() - tprec) > dlay) {
        tprec = millis();
        fc();
    }
}

void MyTimer::cback(void (*fp) (void)) {
    fc = fp;
}

void MyTimer::init() {
    tprec = millis();
}