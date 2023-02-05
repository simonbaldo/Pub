#include <Arduino.h>
#include "Pulsante.h"
#include "MyClock.h"
#include "MyTimer.h"

Pulsante p1(18, 3); //pin pulsante A4, secondi per attivare il flag longPress
//Pulsante p2(19, 3);

//MyClock cl(1000;); //definizione dell'orologio ad 1 secondo
MyClock cl(999);

MyTimer tm(11000);   //imposto un timer ogni 11 secondi per la visualizzazione
MyTimer ti(900);     //imposto un timer per la visualizzazione delle cifre

boolean longPress=false;
boolean visCifra=false;
boolean visualizzaOra=false;
boolean dispAcceso=true;
boolean acqOra=true;
boolean mostraOraSpot=false;
//boolean initDisplay=true;

uint32_t tpinitD=0;   
uint8_t timeDiff1;  //variabile per pulsante 1
uint8_t timeDiff2;  //variabile per pulsante 2
uint8_t modImpo;
uint8_t oraC1;      //prima cifra ora
uint8_t oraC2;      //seconda cifra ora
uint8_t minC1;      //prima cifra minuti
uint8_t minC2;      //seconda cifra minuti
uint8_t oraCurr;    //ora corrente
uint8_t minCurr;    //minuti correnti
uint8_t oraset;    
uint8_t minset;
uint8_t catodoPrec;
uint8_t cifraDisp;
uint8_t spegniNotte=0; //valori possibili 0, 1. Se 1 il nixie viene spento dalle 11.00 alle 5:59
//uint16_t delayInitD=400;

//uint8_t catodi[10]={3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; //array contente le uscite per i catodi per le cifre da 0 a 9
uint8_t catodi[10]={10, 7, 6, 5, 4, 3, 8, 9, 12, 11}; //array contente le uscite per i catodi per le cifre da 0 a 9
char str[15];

//signature function
void testPressedButton();
void display();
void displayImpostazioni();
void displayOra();
void displayOraSpot(); //visualizza ora alla pressione del tasto se sono in modalità nottuna
void scomponiOra();
//void cifreInitDisplay();
void visualizzaCifra();
void dispOra(uint8_t);
void visOra();


//verifica della pressione dei pulsanti per impostazione dell'ora
void testPressedButton() {
	
	//la funzione released() restituisce al rilascio del pulsante i valori: 
	//0 - tasto non premuto
	//1 - tasto premuto
	//2 - pressione prolungata del tasto
	
    timeDiff1=p1.released();
    //timeDiff2=p2.released();

    //se premo il tasto mostro l'ora se sono in spegnimento notturno e nella fascia 23:00 - 5:59
    if (!longPress && timeDiff1 == 1 and spegniNotte == 1  ) {
		if  (cl.getOra() >=23 || cl.getOra()<=6) {
		     mostraOraSpot=true;
			 spegniNotte=0; //disattivo spegnimento
			 acqOra=true;
			 cifraDisp=0;
			 dispAcceso=true;
			 return;
		}	 
	}

    if (timeDiff1 == 2) {
   	   if (!longPress) {
		  longPress=true;
		  modImpo=1;
		  oraset=0;
		  minset=0;
		//oraC1=0;
		//oraC2=0;
		//minC1=0;
		//minC2=0;
		  scomponiOra();
		  spegniNotte=0;
		  return;
	  }
    }    

    //if (longPress && timeDiff2 > 0 ) {
	if (longPress && timeDiff1 == 1 ) {	
       if (modImpo==1) {
	      oraC1++;
	      if (oraC1 == 3) {
		     oraC1=0; 
	      }
          return;
       }
       else
       if (modImpo==2) {
	      oraC2++; 
	      if (oraC2 == 10) {
		     oraC2=0; 
	      }		  
          return;
       }
       else
       if (modImpo==3) {
	      minC1++; 
	      if (minC1 == 6) {
		     minC1=0; 
	      }		  
          return;
       }
       if (modImpo==4) {
	      minC2++; 
	      if (minC2 == 10) {
		     minC2=0; 
	      }		  
          return;
       }
       if (modImpo==5) {
	      spegniNotte++; 
	      if (spegniNotte == 2) {
		     spegniNotte=0; 
	      }		  
          return;
       }

    }   

    //if (longPress && (timeDiff1 >0)) {
	if (longPress && (timeDiff1 == 2)) {	
          modImpo++;
          if  (modImpo==6) {
              longPress=false;
              modImpo=0;
			  oraset=(uint8_t)(oraC1*10 + oraC2);
			  minset=(uint8_t)(minC1*10 + minC2);
              cl.setTime(oraset, minset, 0);
			  ti.init();
			  dispAcceso=true;
          }
          return;
    }	
}

//display della singola cifra dell'orologio
void dispOra(uint8_t cc) {

   //Serial.println("Display");	
	 //spegnimento dei catodi
	// for (int i=0;i<10;i++)
	// 	 digitalWrite(catodi[i], LOW);
 
     //spegnimento del catodo precedente	 
	 if  (catodoPrec!=99)
		 digitalWrite(catodi[catodoPrec], LOW);
	
	  if (cc==99) 
		  strcpy(str, "          ");
		 //str[0]=" ";
	  else {       
		  sprintf(str, "%01d %s", cc, cl.getHMS());
    }  
	
     Serial.print("\r");	
     Serial.print(str);

     //spegniNotte = 0 accende il display, mentre spegniNotte=1 spegne il display dalle 23 alle 5:59
	 if (cc!=99 && spegniNotte == 0 ) 
		 digitalWrite(catodi[cc], HIGH);
	 else	 
	 if (cc!=99 && spegniNotte == 1 &&  oraCurr<23 && oraCurr>5)
		 digitalWrite(catodi[cc], HIGH);
     else		 
	 //in modalità impostazioni visualizzo sempre la cifra 
     if (cc!=99 && modImpo>0)
	    digitalWrite(catodi[cc], HIGH);
     	 
	 catodoPrec=cc;
}


//display delle impostazioni
void displayImpostazioni() {
	
	ti.run();
	
	if (visCifra) {		
		if (dispAcceso) {
			if (modImpo==1) 
			   dispOra(oraC1);
			else
			if (modImpo==2) 
			   dispOra(oraC2);			    
			else
			if (modImpo==3) 
			   dispOra(minC1);			    
			else
			if (modImpo==4) 
			   dispOra(minC2);			    
			else
			if (modImpo==5) 
			   dispOra(spegniNotte);			    
		}	
		else {
		      dispOra(99);
		} 
	  
		dispAcceso=!dispAcceso;
		visCifra=false;			
   }		
	
}


//scomposizione dell'ora nelle singole cifre
void scomponiOra() {
	
	oraCurr=(uint8_t)cl.getOra();
	oraC1=(uint8_t)(oraCurr / 10);
	oraC2=(uint8_t)(oraCurr % 10);
	
	minCurr=(uint8_t)cl.getMin();
	minC1=(uint8_t)(minCurr / 10);
	minC2=(uint8_t)(minCurr % 10);	
}

/*
//esegue uno display delle cifre da 0 a 9 prima di mostrare l'ora
void cifreInitDisplay() {
	
	dispOra(cifraDisp);
  if (millis() - tpinitD > delayInitD) {
    tpinitD=millis();
    cifraDisp++;
  }
	
	if (cifraDisp==10) {
		cifraDisp=0;
		initDisplay=false;
		dispOra(99);
	}
}
*/
//display delle cifre dell'ora
void displayOraSpot() {
	
	if (acqOra) {
		scomponiOra();
		acqOra=false;
	}
	   		   
	ti.run();

	//visCifra viene attivata dalla cback di ti.run();	
	if (visCifra) {
		cifraDisp++;				   
		
		if (dispAcceso) {
			if (cifraDisp==1) 
				dispOra(oraC1);
			else
			if (cifraDisp==3) 
				dispOra(oraC2);			    
			else
			if (cifraDisp==5) 
				dispOra(minC1);			    
			else
			if (cifraDisp==7) 
				dispOra(minC2);			    			   
		}
		else {
			dispOra(99);
		} 
		
		visCifra=false;
		dispAcceso=!dispAcceso;
		if  (cifraDisp==8) {
			mostraOraSpot=false;
			spegniNotte=1;
		}
	}	   
	
}

//display delle cifre dell'ora
void displayOra() {

    //visualizzazione ora a seguito della pressione del tasto in modlaità spegnimento notturno
    if (mostraOraSpot) {
		displayOraSpot();
		return;
	}
	
   //visualizzaOra viene attivata dalla cback di tm.run()
   if  (visualizzaOra) {
	   
	   //visualizza una sequenza di cifre da 0 a 9
	  // if  (initDisplay) {
	  //	   cifreInitDisplay();
	  //	   return;
	  // }
	   
	   if (acqOra) {
		   scomponiOra();
		   acqOra=false;
	   }
	   	
	   
	   ti.run();

	   //visCifra viene attivata dalla cback di ti.run();	
	   if (visCifra) {
		   cifraDisp++;				   
		   
		   if (dispAcceso) {
				if (cifraDisp==1) 
				   dispOra(oraC1);
				else
				if (cifraDisp==3) 
				   dispOra(oraC2);			    
				else
				if (cifraDisp==5) 
				   dispOra(minC1);			    
				else
				if (cifraDisp==7) 
				   dispOra(minC2);			    			   
		   }
		   else {
		      dispOra(99);
		   } 
		   
		   visCifra=false;
		   dispAcceso=!dispAcceso;
		   if  (cifraDisp==8) {
			   visualizzaOra=false;
		   }
	   }	   
   }
	
}


//visualizzazione delle cifre dell'orario o delle impostazioni
void display() {
	
	//verifica se sono in modalità impostazioni
	if (modImpo>0) {
		displayImpostazioni();
	}
	else {
	   displayOra();
	}
}


//callback di visualizzazione della cifra
void visualizzaCifra() {	

  visCifra=true;   
}


//callback di visualizzazione dell'ora
void visOra() {

  //se sono in visualizzazione dell'orario in fase notturna non aggiorno i flag ed i contatori delle cifre
  //poichè ci pensa la function di verifica del pulsante a valorizzarli
  if (mostraOraSpot)
	return;
  	
  visualizzaOra=true; 
  dispAcceso=true;  
  acqOra=true;
  //initDisplay=true;
  
  cifraDisp=0;    
}


//dummy function per callback del clock
void dummyFunc() {

}


//setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    
  //impostazione variabile per impostazione orario
  modImpo=0;
  catodoPrec=99;  
  
  //inizializzazione dell'orologio
  cl.init();
  //definizione della callback - Verrà chiamata la funzione ad ogni intervallo prestabilito
  //chiamo una dummy function perchè per il momento non serve. E' necessario definirla anche se vuota
  //per non causare comportamenti aleatori
  cl.cback(dummyFunc);
  
  //imposto timer per visualizzazione delle cifre
  ti.init();
  //impostazione callback a scatto del timer per visualizzazione delle cifre
  ti.cback(visualizzaCifra);
  
  //imposto timer per visualizzazione delle cifre dell'ora
  tm.init();
  //impostazione callback a scatto del timer  per visualizzazione dell'ora
  tm.cback(visOra);  

  delay(2000);
  //impostazione pinout delle uscite dei catodi e spegimento del nixie
  for (int i=0;i<10;i++) {
	  pinMode(catodi[i], OUTPUT);
	  dispOra(i);
	  delay(500);
  }
  //spegnimento dell'ultima cifra
  dispOra(99);	  

}


//main loop
void loop() {

//avvio timer per visualizzazione cifre orologio
  tm.run();
	
//avvio orologio
  cl.run();
	
//verifica pressione dei pulsanti per impostazione ora
  testPressedButton();	
	
//mostro ora oppure impostazioni 
  display();
}

