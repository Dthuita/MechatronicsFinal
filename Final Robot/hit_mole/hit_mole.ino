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
int testState = 0;
char curr = 'w';
char next = 'b';

unsigned long startTime = 0; // for non-blocking delay
bool isActionInProgress = false; // flag to check if action is in progress
long actionDuration = 0; // stores duration for the current action

void setup() {
  // Configure H-bridge pins 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(LEFTENCODEA), leftIsr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHTENCODEA), rightIsr, CHANGE);
}

/*
void forward(double distance) {
  long time = distance / 0.0762; // Time = Distance / Speed

  // Set the motor direction to forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  delay(3000);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void reverse(double distance) {
  long time = distance / 0.0762; // Time = Distance / Speed

  // Set the motor direction to reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  delay(3000);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}
*/

void cmForward(int x) {
  cm(x,true);
}

void cmReverse(int x) {
  cm(x,false);
}

void cm(int x, bool dir) {
  double distPerCount = 60 * 3.1415926 / countsPerRotation / 2;
  bool complete = false;
  double distL = 0.0;
  double distR = 0.0;
  counterL = 0;
  counterR = 0;
  //Start moving the motors
  if(dir) {
    forward(); }
  else {
    reverse();
  }

  //Track distance and keep going until we've traveled the correct distance
  while(!complete) {
    // Update left distance
    if(counterL != lastCounterL) {
      lastCounterL = counterL;
      distL = distPerCount * counterL / 10;
    }
    // Update right distance
    if(counterR != lastCounterR) {
      lastCounterR = counterR;
      distR = distPerCount * counterR / 10;
    }
    // Check if we've traveled the correct distance
    if((dir && (distL > x || distR > x)) || (!dir && (distL < -x || distR < -x))) {
      complete = true;
    }
  }
  // Stop the robot
  brake();
}

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

void leftIsr() {
  int b = digitalRead(LEFTENCODEB);
  int a = digitalRead(LEFTENCODEA);
  //Moving counterclockwise
  if(b == a) {
    counterL--;
  }
  //Moving clockwise
  else {
    counterL++;
  }
}

void rightIsr()
{
  int b = digitalRead(RIGHTENCODEB);
  int a = digitalRead(RIGHTENCODEA);
  //Moving counterclockwise
  if(b == a) {
    counterR++;
  }
  
  //Moving clockwise
  else {
    counterR--;
  }
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
  delay(1800);
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
  delay(1800);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void turnMoleForward(float angle, double distance1, double distance2) {
  forward(distance1);
  pivotLeft(-1 * angle);
  forward(distance2);
  pivotLeft(-1 * angle);
  forward(distance1);
}

void turnMoleReverse(float angle, double distance1, double distance2) {
  reverse(distance1);
  pivotRight(angle);
  reverse(distance2);
  pivotRight(angle);
  reverse(distance1);
}

int dist = 0;
void loop() {
  if(dist != 0) {
    Serial.println("Enter a distance to move (enter negative number to move in the reverse direction:");
  }

  while (Serial.available()==0){}
  // Tell the robot to travel the specific distance entered by the user
  dist = Serial.parseInt();

  if(dist > 0) {
    cmForward(dist);
  }
  else {
    cmReverse(-dist);
  }


  // Test runs based on the state
  switch (testState) {
    case 0:
      curr = 'w'; next = 'b';
      break;
    case 1:
      curr = 'b'; next = 'p';
      break;
    case 2:
      curr = 'p'; next = 'g';
      break;
    case 3:
      curr = 'g'; next = 'r';
      break;
    case 4:
      curr = 'r'; next = 'w';
      break;
    case 5:
      curr = 'w'; next = 'y';
      break;
    case 6:
      curr = 'y'; next = 'g';
      break;
    default:
      return;
  }

  // put your main code here, to run repeatedly:
  if ((curr == 'g' && next == 'b')
    || (curr == 'b' && next == 'w')
    || (curr == 'r' && next == 'p')
    || (curr == 'p' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-75, 38.1, 197.104);
    else
      turnMoleReverse(-75, 38.1, 197.104);
    ori = !ori;    
  }

  else if ((curr == 'b' && next == 'g')
    || (curr == 'w' && next == 'b')
    || (curr == 'p' && next == 'r')
    || (curr == 'y' && next == 'p'))
  {
    if (ori == 1)
      turnMoleForward(75, 38.1, 197.104);
    else
      turnMoleReverse(75, 38.1, 197.104);
    ori = !ori;
  }

  else if ((curr == 'g' && next == 'w')
    || (curr == 'w' && next == 'r')
    || (curr == 'r' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-60, 38.1, 381);
    else
      turnMoleReverse(-60, 38.1, 381);
    ori = !ori;
  }

  else if ((curr == 'w' && next == 'g')
    || (curr == 'r' && next == 'w')
    || (curr == 'y' && next == 'r'))
  {
    if (ori == 1)
      turnMoleForward(60, 38.1, 381);
    else
      turnMoleReverse(60, 38.1, 381);
    ori = !ori;    
  }

  else if ((curr == 'g' && next == 'r')
    || (curr == 'b' && next == 'p')
    || (curr == 'w' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-30, 38.1, 659.892);
    else
      turnMoleReverse(-30, 38.1, 659.892);
    ori = !ori;    
  }  

  else if ((curr == 'r' && next == 'g')
    || (curr == 'p' && next == 'b')
    || (curr == 'y' && next == 'w'))
  {
    if (ori == 1)
      turnMoleForward(30, 38.1, 659.892);
    else
      turnMoleReverse(30, 38.1, 659.892);
    ori = !ori;    
  }  

  else if ((curr == 'g' && next == 'p')
    || (curr == 'b' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-15, 38.1, 736.092);
    else
      turnMoleReverse(-15, 38.1, 736.092);
    ori = !ori;    
  }  

  else if ((curr == 'p' && next == 'g')
    || (curr == 'y' && next == 'b'))
  {
    if (ori == 1)
      turnMoleForward(15, 38.1, 736.092);
    else
      turnMoleReverse(15, 38.1, 736.092);
    ori = !ori;    
  }  

  else if ((curr == 'b' && next == 'r')
    || (curr == 'w' && next == 'p'))
  {
    if (ori == 1)
      turnMoleForward(-45, 38.1, 538.734);
    else
      turnMoleReverse(-45, 38.1, 538.734);
    ori = !ori;    
  }  

  else if ((curr == 'r' && next == 'b')
    || (curr == 'p' && next == 'w'))
  {
    if (ori == 1)
      turnMoleForward(45, 38.1, 538.734);
    else
      turnMoleReverse(45, 38.1, 538.734);
    ori = !ori;    
  }  

  else if ((curr == 'g' && next == 'y')
    || (curr == 'y' && next == 'g'))
  {
    if (ori == 1)
      reverse(838.2);
    else
      forward(838.2);
    ori = !ori;    
  }  

  testState++;
  delay(3000);
}