#ifndef __MYCLOCK_H__
#define __MYCLOCK_H__

#include <Arduino.h>

class MyClock{

    private:	
	uint16_t dlay;
        uint8_t ora;
        uint8_t min;
        uint8_t sec;
        uint32_t tprec;
        void (*fcc) (void);
        char str[8];		

    public:
        MyClock(uint16_t dl);
        void init();
        void run();
        void setOra(uint8_t oo);
	uint8_t getOra();
        void setMin(uint8_t mm);
	uint8_t getMin();
        void setSec(uint8_t ss);
	void setTime(uint8_t oo, uint8_t mm, uint8_t ss);		
        char * getHMS();
        void cback(void (*fpp) (void));
        void mostraOra();
        void addOra();
        void addMin();
};

#endif
