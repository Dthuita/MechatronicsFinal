#include <TimerOne.h>

/*
 * #define LEFTPWM 5
 * #define LEFTA 16
 * #define LEFTB 17
 * 
 * #define RIGHTPWM 6
 * #define RIGHTA 14
 * #define RIGHTB 15
 * 
 * #define LEFTENCODEA 2
 * #define LEFTENCODEB 3
 * #define RIGHTENCODEA 20
 * #define RIGHTENCODEB 21
 */


/*************************************************************************
*  MOTOR and HBRIDGE PINS
*************************************************************************/
  // Left Motor
#define LEFTPWM 3   //enA
#define LEFTA 22    //in1
#define LEFTB 24    //in2

  // Right Motor
#define RIGHTPWM 2  //enB
#define RIGHTA 23   //in3
#define RIGHTB 25   //in4

//motor pins
#define LEFTENCODEA 19   //originally 26
#define LEFTENCODEB 27
#define RIGHTENCODEA 20  //originally 28
#define RIGHTENCODEB 29

int countsPerRotation = 932;

volatile int counterL = 0;
volatile int counterR = 0;

volatile int lastCounterL = 0;
volatile int lastCounterR = 0;

int dist = 0;

void setup() {
  // Read encoder inputs
  pinMode(LEFTENCODEA, INPUT_PULLUP);
  pinMode(LEFTENCODEB, INPUT_PULLUP);
  pinMode(RIGHTENCODEA, INPUT_PULLUP);
  pinMode(RIGHTENCODEB, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  // Set all motor pins to be outputs
  pinMode(LEFTPWM, OUTPUT);
  pinMode(LEFTA, OUTPUT);
  pinMode(LEFTB, OUTPUT);
  pinMode(RIGHTPWM, OUTPUT);
  pinMode(RIGHTA, OUTPUT);
  pinMode(RIGHTB, OUTPUT);
  
  Serial.println("Enter a distance to move (enter negative number to move in the reverse direction:");
  
  attachInterrupt(5, leftIsr, CHANGE);     //pin 26 maps to INT5, can try with just 5: digitalPinToInterrupt(LEFTENCODEA)
  attachInterrupt(6, rightIsr, CHANGE);    //pin 28 maps to INT6, can try with just 6: digitalPinToInterrupt(LEFTENCODEA)
}

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
}

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
      //Serial.print("distL: ");
      //Serial.println(distL);
    }
    
    // Update right distance
    if(counterR != lastCounterR) {
      lastCounterR = counterR;
      distR = distPerCount * counterR / 10;
      //Serial.print("distR: ");
      //Serial.println(distR);
    }
    // Check if we've traveled the correct distance
    if((dir && (distL > x || distR > x)) || (!dir && (distL < -x || distR < -x))) {
      complete = true;
    }

    //Serial.print("complete: ");
    //Serial.println(complete);
  }
  // Stop the robot
  brake();
  Serial.print("brake");
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
  Serial.println("rightISR");
  
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
