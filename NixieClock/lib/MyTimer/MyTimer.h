#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <Arduino.h>

class MyTimer {

    private:	
		uint16_t dlay;
        uint32_t tprec;		
        void (*fc) (void);
      
    public:
        MyTimer(uint16_t dl);
        void init();
        void run();
        void cback(void (*fp) (void));
};

#endif