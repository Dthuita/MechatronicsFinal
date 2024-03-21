
#include "RobotFunctions.h"

/************** PINS **************/
#define servoPin 4 //Note: this pin must have PWM

/************** VARs **************/

Servo myservo;  // create servo object to control a servo
int pos = 0; 

/************** FUNCs **************/

void init_Servo() {
  myservo.attach(servoPin);  // (pin, min, max)
  
  //init servo postion down
  myservo.write(0);

  
}

void swing_hammer() {
  Serial.println("First Swing");
  myservo.write(90);
  delay(250); //may cause issues
  myservo.write(180);
  
  Serial.println("Second Swing");
  myservo.write(90);
  delay(250); //may cause issues
  myservo.write(180);
}

void servo_test(){
  //nothing rn!
}
