#ifndef __PULSANTE_H_
#define __PULSANTE_H_

#include <Arduino.h>

#define debounceDelay 150

class Pulsante{

    private:
        unsigned long tPressed;
        unsigned long tReleased;
        unsigned long secPress;
        boolean pulPressed;
        boolean pulReleased;
        uint8_t pinPulsante;
        boolean preState;
        boolean curState;

    public:
       Pulsante(uint8_t pinPulsante, unsigned long secPress);
       bool pressed();
       uint8_t released();
};

#endif
