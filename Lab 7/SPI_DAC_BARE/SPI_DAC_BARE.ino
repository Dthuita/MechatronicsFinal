#include <SPI.h>

#define COPI 51
#define SCK 52
#define CS 24


/* dacval must be between 0 and 1023 or there will be an error */
/* Vref is set at 4.096V */

void DACout(unsigned int dacval) {
  dacval = dacval*4 + 0x1000;
  digitalWrite(CS, LOW);
  SPI.transfer16(dacval); 
  digitalWrite(CS, HIGH);             // 
}



void setup() {
  SPI.begin();   // initiate SPI as controller
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)) ;
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
}

void loop() {
  DACout(498);
}





