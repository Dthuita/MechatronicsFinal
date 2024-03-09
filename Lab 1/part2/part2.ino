#define SQUARE 12
#include <avr/interrupt.h>
#include <stdlib.h>

void setup() {
  pinMode(SQUARE, OUTPUT);
  
  // Interupt configuration
  TCCR1A = 0;
  TCCR1B = bit(WGM12) | bit(CS10);
  OCR1A =  7999; // 16Mhz/2 (for when signal is low vs when it is high)/1000(Hz) - 1 (since it is zero indexed).
  TIMSK1 = bit (OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
  //Flip the value of the pin output, called every 500 microseconds (1/2000 sec)
  digitalWrite(SQUARE, digitalRead(SQUARE) ^ 1);
}

void loop() {
}