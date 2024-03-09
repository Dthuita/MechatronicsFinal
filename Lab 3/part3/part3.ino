#define BUTTON 2
#include <TimerOne.h>

int enabled = 1;
int lastCounter = 0;
double elapsedTime = 0.0;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  // Create a timer that gets called every 100ms

  Timer1.initialize(100000);
  Timer1.attachInterrupt(timerIsr);

  // Create an interupt for the pin
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonIsr, RISING);
  Serial.begin(9600);
}
void loop() {
  //Check if we should be printing the elapsed time
  if(enabled) {
    Serial.print("Elapsed time: ");
    Serial.print(elapsedTime);
    Serial.println("s");
    delay(1000);
  } 
}

// Toggle whether we print the elapsed time
void buttonIsr() {
  enabled = !enabled;
}

// Increment the time counter
void timerIsr() {
  elapsedTime += 0.1;
}