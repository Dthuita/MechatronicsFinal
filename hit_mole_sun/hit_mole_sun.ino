#include <math.h>
#include <TimerOne.h>

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
#define LEFTENCODEA 19
#define LEFTENCODEB 27
#define RIGHTENCODEA 20
#define RIGHTENCODEB 29

int ori = 1; // robot starts facing forward, 0 means backward and 1 means forward
int w = 190; // width of wheel base
int testState = 0;
char curr = 'w';
char next = 'b';

int countsPerRotation = 932;
volatile int counterL = 0;
volatile int counterR = 0;
volatile int lastCounterL = 0;
volatile int lastCounterR = 0;

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

  Serial.begin(9600);

  turnMoleReverse(-60, 3.81, 38.1);
}

void cmForward(int x) {
  int y = x/2;
  cm(y,0);
}

void cmReverse(int x) {
  int y = x/2;
  cm(y,1);
}

/*
void cmPivotLeft(int x) {
  cm(x,2);
}

void cmPivotRight(int x) {
  cm(x,3);
}
*/

void cmPivotLeft(int angle) {
  // Convert angle to distance using the wheelbase width
  double distance = (angle / 360.0) * (M_PI * 9); // 'w' is the wheelbase width in mm
  cm(distance, 2); // 2 represents pivoting left
}

void cmPivotRight(int angle) {
  // Convert angle to distance using the wheelbase width
  double distance = (angle / 360.0) * (M_PI * 9); // 'w' is the wheelbase width in mm
  cm(distance, 3); // 3 represents pivoting right
}

/*
void cm(int x, int dir) {
  double distPerCount = (17.6 * 3.1415926) / countsPerRotation;
  //double distPerCount = 60 * 3.1415926 / countsPerRotation
  //double distPerCount = 60 * 3.1415926 / countsPerRotation / 2;
  bool complete = false;
  double distL = 0.0;
  double distR = 0.0;
  counterL = 0;
  counterR = 0;
  // Start moving the motors
  if(dir == 0)
    forward();
  else if (dir == 1)
    reverse();
  else if (dir == 2)
    pivotLeft();
  else if (dir == 3)
    pivotRight();

  // Track distance and keep going until we've traveled the correct distance
  while(!complete) {
    // Update left distance
    if(counterL != lastCounterL) {
      lastCounterL = counterL;
      distL = distPerCount * counterL / 10; // Removed the incorrect division by 10
    }
    // Update right distance
    if(counterR != lastCounterR) {
      lastCounterR = counterR;
      distR = distPerCount * counterR / 10; // Removed the incorrect division by 10
    }
    // Check if we've traveled the correct distance
    if((dir && (distL >= x || distR >= x)) || (!dir && (distL <= -x || distR <= -x))) {
      complete = true;
    }
  }
  // Stop the robot
  brake();
}
*/

void cm(int x, int dir) {
  double distPerCount = (17.6 * 3.1415926) / countsPerRotation; // Ensure this is correct based on your wheel's actual circumference
  bool complete = false;
  double distL = 0.0;
  double distR = 0.0;
  counterL = 0;
  counterR = 0;

  // Start moving the motors based on direction
  if(dir == 0) { // Forward
    forward();
  } else if (dir == 1) { // Reverse
    reverse();
  } else if (dir == 2) { // Pivot Left
    pivotLeft();
  } else if (dir == 3) { // Pivot Right
    pivotRight();
  }

  // Track distance and keep going until we've traveled the correct distance
  while(!complete) {
    // Update left and right distances based on counters
    if(counterL != lastCounterL) {
      lastCounterL = counterL;
      distL += distPerCount / 10; // Simply add because we're tracking total movement
    }
    if(counterR != lastCounterR) {
      lastCounterR = counterR;
      distR += distPerCount / 10; // Simply add for the same reason
    }

    // For pivoting, you might choose the larger of the two distances
    // since one wheel might move slightly more than the other
    double maxDist = max(distL, distR);

    // Check if we've traveled the correct distance
    // For pivoting, we compare against the larger of the two distances
    if (dir == 0 || dir == 1) { // Forward or Reverse
      if(distL >= x || distR >= x) {
        complete = true;
      }
    } else if (dir == 2 || dir == 3) { // Pivot Left or Right
      if(maxDist >= x) {
        complete = true;
      }
    }
  }

  // Stop the robot
  brake();
}


void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

void reverse() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

void brake() {
  // Turn off all motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
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

void pivotLeft() {
  //double distance = (angle / 360.0) * M_PI * w;
  //long time = distance / 0.0762;

  // Set left motor to reverse and right motor to forward for pivot
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

void pivotRight() {
  //double distance = (angle / 360.0) * M_PI * w;
  //long time = distance / 0.0762;

  // Set left motor to forward and right motor to reverse for pivot
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255); // Full speed
  analogWrite(enB, 255); // Full speed
}

//Turning the mole to drive forward aka starting backwards
void turnMoleForward(float angle, double distance1, double distance2) {
  cmForward(distance1);
  delay(500);

  if(angle < 0){
    cmPivotLeft(-1 * angle);
  }
  else{
    cmPivotRight(angle);
  }
  
  delay(500);
  cmForward(distance2);
  delay(500);

  if(angle < 0){
    cmPivotLeft(-1 * angle);
  }
  else{
    cmPivotRight(angle);
  }
  
  delay(500);
  cmForward(distance1);
}

//Turning the mole to drive in reverse aka starting forwards
void turnMoleReverse(float angle, double distance1, double distance2) {
  cmReverse(distance1);
  delay(500);
  
  if(angle < 0){
    cmPivotLeft(-1 * angle);
  }
  else{
    cmPivotRight(angle);
  }

  delay(500);
  cmReverse(distance2);
  delay(500);
  
  if(angle < 0){
    cmPivotLeft(-1 * angle);
  }
  else{
    cmPivotRight(angle);
  }

  delay(500);
  cmReverse(distance1);
}

void loop() {
  /*
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
    case 7:
      brake();
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
      turnMoleForward(-75, 3.81, 19.7104);
    else
      turnMoleReverse(-75, 3.81, 19.7104);
    ori = !ori;    
  }

  else if ((curr == 'b' && next == 'g')
    || (curr == 'w' && next == 'b')
    || (curr == 'p' && next == 'r')
    || (curr == 'y' && next == 'p'))
  {
    if (ori == 1)
      turnMoleForward(75, 3.81, 19.7104);
    else
      turnMoleReverse(75, 3.81, 19.7104);
    ori = !ori;
  }

  else if ((curr == 'g' && next == 'w')
    || (curr == 'w' && next == 'r')
    || (curr == 'r' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-60, 3.81, 38.1);
    else
      turnMoleReverse(-60, 3.81, 38.1);
    ori = !ori;
  }

  else if ((curr == 'w' && next == 'g')
    || (curr == 'r' && next == 'w')
    || (curr == 'y' && next == 'r'))
  {
    if (ori == 1)
      turnMoleForward(60, 3.81, 38.1);
    else
      turnMoleReverse(60, 3.81, 38.1);
    ori = !ori;    
  }

  else if ((curr == 'g' && next == 'r')
    || (curr == 'b' && next == 'p')
    || (curr == 'w' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-30, 3.81, 65.9892);
    else
      turnMoleReverse(-30, 3.81, 65.9892);
    ori = !ori;    
  }  

  else if ((curr == 'r' && next == 'g')
    || (curr == 'p' && next == 'b')
    || (curr == 'y' && next == 'w'))
  {
    if (ori == 1)
      turnMoleForward(30, 3.81, 65.9892);
    else
      turnMoleReverse(30, 3.81, 65.9892);
    ori = !ori;    
  }  

  else if ((curr == 'g' && next == 'p')
    || (curr == 'b' && next == 'y'))
  {
    if (ori == 1)
      turnMoleForward(-15, 3.81, 73.6092);
    else
      turnMoleReverse(-15, 3.81, 73.6092);
    ori = !ori;    
  }  

  else if ((curr == 'p' && next == 'g')
    || (curr == 'y' && next == 'b'))
  {
    if (ori == 1)
      turnMoleForward(15, 3.81, 73.6092);
    else
      turnMoleReverse(15, 3.81, 73.6092);
    ori = !ori;    
  }  

  else if ((curr == 'b' && next == 'r')
    || (curr == 'w' && next == 'p'))
  {
    if (ori == 1)
      turnMoleForward(-45, 3.81, 53.8734);
    else
      turnMoleReverse(-45, 3.81, 53.8734);
    ori = !ori;    
  }  

  else if ((curr == 'r' && next == 'b')
    || (curr == 'p' && next == 'w'))
  {
    if (ori == 1)
      turnMoleForward(45, 3.81, 53.8734);
    else
      turnMoleReverse(45, 3.81, 53.8734);
    ori = !ori;    
  }  

  else if ((curr == 'g' && next == 'y')
    || (curr == 'y' && next == 'g'))
  {
    if (ori == 1)
      cmReverse(8.3);
    else
      cmForward(8.3);
    ori = !ori;    
  }  

  testState++;
  //delay(20000);*/
}
