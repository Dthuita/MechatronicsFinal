#include <Servo.h>

#define powerPin 11

Servo myservo;  // create servo object to control a servo
int pos = 0; 

void setup() {
  myservo.attach(6);  // (pin, min, max)

  pinMode(powerPin, OUTPUT);  
  digitalWrite(powerPin, HIGH);
  Serial.begin(9600);
}

void loop() {
  // digitalWrite(powerPin, HIGH);

  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(1);                       // waits 15ms for the servo to reach the position 
  } 
  Serial.println("moved to max angle!");
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }
  Serial.println(pos);
  Serial.println("moved to min angle!");

}