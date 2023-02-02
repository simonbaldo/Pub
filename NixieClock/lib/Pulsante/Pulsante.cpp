#include "Pulsante.h"

Pulsante::Pulsante(uint8_t pinpul, unsigned long secP){
    secPress=(unsigned long)secP * 1000;
    pinPulsante=pinpul;
    pinMode(pinPulsante, INPUT);
    pulPressed=false;
    preState=LOW;
    curState=LOW;
};

bool Pulsante::pressed() {
   if (pulPressed) {
       if ((millis() - tPressed) > debounceDelay) {
          pulPressed=false;
          return true;
       }
   }
   else {
       if  (digitalRead(pinPulsante)) {
            pulPressed=true;
            tPressed=millis();            
        }
   }
   return false;             
};

uint8_t Pulsante::released() {
   curState=digitalRead(pinPulsante);
   if (pulPressed) {
       if ((millis() - tPressed) > debounceDelay) {
          pulPressed=false;          
       }
   }
   else
   if (pulReleased) {
       if ((millis() - tReleased) > 200) {
          pulReleased=false;          
          
          if ((tReleased - tPressed) > secPress)
            return 2;
          else 
            return 1;
       }
   }
   else {
       if  (curState==HIGH && preState==LOW) {
            pulPressed=true;
            preState=HIGH;
            tPressed=millis();            
        }
        if (curState==LOW && preState==HIGH) {
           pulReleased=true;
           preState=LOW;
           tReleased=millis();
        }
   }

   return 0;             
};
