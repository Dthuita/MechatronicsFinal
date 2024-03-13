#include <math.h>

/*************************************************************************
*  MOTOR and HBRIDGE PINS
*************************************************************************/
// Left Motor
#define enA 3
#define in1 22
#define in2 24

// Right Motor
#define enB 2
#define in3 23
#define in4 25

// Motor pins
#define LEFTENCODEA 26
#define LEFTENCODEB 27
#define RIGHTENCODEA 28
#define RIGHTENCODEB 29

int ori = 1; // robot starts facing forward, 0 means backward and 1 means forward
int w = 190; // width of wheel base

void setup() {
  // Configure H-bridge pins 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
/*
void turnAngle(float angle) {
  // placeholder values
  unsigned long turnTime = 0;
  if (angle == 30) turnTime = 500;
  else if (angle == 45) turnTime = 750;
  else if (angle == 60) turnTime = 1000;
  else if (angle == 90) turnTime = 1500;
  else if (angle == 120) turnTime = 2000;
  else if (angle == 180) turnTime = 3000;

  unsigned long startTime = millis();
  while (millis() - startTime < turnTime) {
    analogWrite(enA, basespeeda + 50);
    analogWrite(enB, basespeedb - 50);
  }
  forward_brake(0, 0);
}
*/

void forward(double distance) {
  // Calculate the time to run the motors in milliseconds
  long time = distance / 0.0762; // Time = Distance / Speed

  // Set the motor direction to forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // Turn on the motors
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed

  // Wait for the time calculated
  delay(time);

  // Stop the motors
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void reverse(double distance) {
  // Calculate the time to run the motors in milliseconds
  long time = distance / 0.0762; // Time = Distance / Speed

  // Set the motor direction to reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  // Turn on the motors
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed

  // Wait for the time calculated
  delay(time);

  // Stop the motors
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void pivotLeft(float angle) {
  double distance = (angle / 360.0) * M_PI * w;
  long time = distance / 0.0762;

  // Set left motor to reverse and right motor to forward for pivot
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  delay(time);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void pivotRight(float angle) {
  double distance = (angle / 360.0) * M_PI * w;
  long time = distance / 0.0762;

  // Set left motor to forward and right motor to reverse for pivot
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed
  delay(time);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void turnMoleForward(float angle, double distance1, double distance2) {
  forward(distance1);
  pivotLeft(angle);
  forward(distance2);
  pivotLeft(angle);
  forward(distance1);
}

void turnMoleReverse(float angle, double distance1, double distance2) {
  reverse(distance1);
  pivotRight(angle);
  reverse(distance2);
  pivotRight(angle);
  reverse(distance1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((curr = "blue" && next = "green" && ori = 1)
    || (curr = "white" && next = "blue" && ori = 1)
    || (curr = "purple" && next = "red" && ori = 1)
    || (curr = "yellow" && next = "purple" && ori = 1)
  {
    if (ori = 1)
      turnMoleForward(75, 38.1, 197.1);
    else
      turnMoleReverse(75, 38.1, 197.1);
    ori = !ori;
  }

  else if ((curr = "green" && next = "white")
    || (curr = "white" && next = "red")
    || (curr = "red" && next = "yellow")
  {
    if (ori = 1)
      turnMoleForward(-60, 38.1, 381);
    else
      turnMoleReverse(-60, 38.1, 381);
    ori = !ori;
  }

  else if ((curr = "white" && next = "green")
    || (curr = "red" && next = "white")
    || (curr = "yellow" && next = "red")
  {
    if (ori = 1)
      turnMoleForward(60, 38.1, 381);
    else
      turnMoleReverse(60, 38.1, 381);
    ori = !ori;    
  }

  else if ((curr = "green" && next = "red")
    || (curr = "blue" && next = "purple")
    || (curr = "white" && next = "yellow")
  {
    if (ori = 1)
      turnMoleForward(-30, 38.1, 659.892);
    else
      turnMoleReverse(-30, 38.1, 659.892);
    ori = !ori;    
  }  

  else if ((curr = "red" && next = "green")
    || (curr = "purple" && next = "blue")
    || (curr = "yellow" && next = "white")
  {
    if (ori = 1)
      turnMoleForward(30, 38.1, 659.892);
    else
      turnMoleReverse(30, 38.1, 659.892);
    ori = !ori;    
  }  

  else if ((curr = "green" && next = "purple")
    || (curr = "blue" && next = "yellow")
  {
    if (ori = 1)
      turnMoleForward(-15, 38.1, 736.092);
    else
      turnMoleReverse(-15, 38.1, 736.092);
    ori = !ori;    
  }  

  else if ((curr = "purple" && next = "green")
    || (curr = "yellow" && next = "blue")
  {
    if (ori = 1)
      turnMoleForward(15, 38.1, 736.092);
    else
      turnMoleReverse(15, 38.1, 736.092);
    ori = !ori;    
  }  

  else if ((curr = "blue" && next = "red")
    || (curr = "white" && next = "purple")
  {
    if (ori = 1)
      turnMoleForward(-45, 38.1, 538.734);
    else
      turnMoleReverse(-45, 38.1, 538.734);
    ori = !ori;    
  }  

  else if ((curr = "red" && next = "blue")
    || (curr = "purple" && next = "white")
  {
    if (ori = 1)
      turnMoleForward(45, 38.1, 538.734);
    else
      turnMoleReverse(45, 38.1, 538.734);
    ori = !ori;    
  }  

  else if ((curr = "green" && next = "yellow")
    || (curr = "yellow" && next = "green")
  {
    if (ori = 1)
      reverse(838.2);
    else
      forward(838.2);
    ori = !ori;    
  }  
}
/*
void forward() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Set both motors to full speed
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void reverse() {
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, HIGH);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, HIGH);
  // Set both motors to full speed
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void brake() {
  // Turn off all motors
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, LOW);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void coast() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Set motors to 0% but keep them coasting
  analogWrite(LEFTPWM, 0);
  analogWrite(RIGHTPWM, 0);
}

void turnRight() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Spin the left motor, causing a right turn
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 0);
}

void turnLeft() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Spin the right motor, causing a left turn
  analogWrite(LEFTPWM, 0);
  analogWrite(RIGHTPWM, 255);
}

void pivotLeft() {
  //Left motor goes backwards, right motor goes forwards
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, HIGH);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void pivotRight() {
  //Right motor goes backwards, left motor goes forwards
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, HIGH);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}
*/