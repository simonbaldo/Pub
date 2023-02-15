#include "MyClock.h"

MyClock::MyClock(uint16_t dl) {
	dlay=dl;
    init();
}

void MyClock::init() {
    ora = 0;
    min = 0;
    sec = 0;
    tprec = 0;
    offset=0;
}

void MyClock::setOra(uint8_t oo) {
    ora=oo;
}

void MyClock::setMin(uint8_t mm) {
    min=mm;
}

void MyClock::setSec(uint8_t ss) {
    sec=ss;
}

uint8_t MyClock::getOra() {
    return ora;
}

uint8_t MyClock::getMin() {
    return min;
}

void MyClock::setOffset(uint8_t off) {
    offset=off;
}

void MyClock::setTime(uint8_t oo, uint8_t mm, uint8_t ss) {
	ora=oo;
	min=mm;
	sec=ss;
}

void MyClock::run() {

    if ((millis() - tprec) > dlay) {
        sec++;
        if (sec==60) {
            min++;
            setSec(0);
            if (min==60) {
                ora++;
                setMin(0);
                if (ora==24) {
                    setOra(0);
                }
            }
        }
        //alle 5:50:00 aggiusto l'ora 
        if (ora==05 && min==50 && sec==00) {
            sec=offset;
        }
        
        tprec = millis();
        fcc();
    }
}

void MyClock::addOra() {
    ora++;
    if (ora==24)
        setOra(0);
}

void MyClock::addMin() {
    min++;
    if (min==60)
        setMin(0);
}

void MyClock::mostraOra() {
    sprintf(str, "%02d:%02d:%02d", ora, min, sec);
  //mostro ora corrente sempre sulla stessa riga nel serial monitor
    Serial.print("\r");
    Serial.print(str);
}

char * MyClock::getHMS() {
    sprintf(str, "%02d:%02d:%02d", ora, min, sec);
    return str;
}

void MyClock::cback(void (*fpp) (void)){
    fcc = fpp;
}
