#define CHANNELA 2
#define CHANNELB 3
#include <TimerOne.h>

int countsPerRotation = 932;
int counter = 0;
int lastCounter = 0;
int ledCount = 0;

void setup() {
  pinMode(CHANNELA, INPUT_PULLUP);
  pinMode(CHANNELB, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  // Detect a rising or falling edge on Channel A (pin 2)
  attachInterrupt(digitalPinToInterrupt(CHANNELA), channelAIsr, CHANGE);

  // Timer to flash LED on pin 13
  Timer1.initialize(100000);
  Timer1.attachInterrupt(timerIsr);
  Serial.begin(9600);
}

void loop() {
  double distPerCount = 60 * 3.1415926 / countsPerRotation / 2;

  // If counter changed, print the new distance
  if(counter != lastCounter) {
    Serial.print("Distance: ");
    Serial.print(distPerCount * counter / 10);
    Serial.println("cm");
    lastCounter = counter;
  } 
}

void channelAIsr() {
  int b = digitalRead(CHANNELB);
  int a = digitalRead(CHANNELA);

  //Moving counterclockwise
  if(b == a) {
    counter--; 
  }

  //Moving clockwise
  else {
    counter++; 
  } 
}

void timerIsr() {
  ledCount = (ledCount + 1) % 5;
  //Turn LED on for 200ms

  if(ledCount == 0) {
    digitalWrite(13, HIGH);
  }
  else if(ledCount == 2) {
    digitalWrite(13, LOW);
  } 
}