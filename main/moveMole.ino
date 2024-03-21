//INCLUDE LIBRARIES------------------------------------------------------------------------------------------------------------
#include <math.h>
#include <TimerOne.h>

//VARIABLE SETUP------------------------------------------------------------------------------------------------------------

    // Left Motor
    #define enA 3
    #define in1 22
    #define in2 24
    
    // Right Motor
    #define enB 2
    #define in3 23
    #define in4 25
    
    // Motor pins
    #define LEFTENCODEA 18
    #define LEFTENCODEB 27
    #define RIGHTENCODEA 19
    #define RIGHTENCODEB 29
    
    //Distance variables
    int countsPerRotation = 932;
    volatile int counterL = 0;
    volatile int counterR = 0;
    volatile int lastCounterL = 0;
    volatile int lastCounterR = 0;


void init_moveMole(){
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
}


//CONVERT INPUTS INTO DISTANCE------------------------------------------------------------------------------------------------------------

    void cmForward(int x) {
      int y = x/2;
      cm(y,0);
    }

    void cmSpeed(int x){
      int y = x/2;
      cm(y,4);
    }
    
    void cmReverse(int x) {
      int y = x/2;
      cm(y,1);
    }
    
    void cmPivotLeft(int angle) {
      // Convert angle to distance using the wheelbase width
      double distance = (angle / 360.0) * (M_PI * 9); //(degrees / 360.0) * (pi * 1/2 * wheelbase); 
      cm(distance, 2); // 2 represents pivoting left
    }
    
    void cmPivotRight(int angle) {
      // Convert angle to distance using the wheelbase width
      double distance = (angle / 360.0) * (M_PI * 9); // 'w' is the wheelbase width in mm
      cm(distance, 3); // 3 represents pivoting right
    }


//MOVE A SPECIFIED DISTANCE------------------------------------------------------------------------------------------------------------

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
      } else if (dir == 4) { // Speed
        fastForward();
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


//CORE MOVEMENT FUNCTIONS------------------------------------------------------------------------------------------------------------

    void forward() {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enA, 150);
      analogWrite(enB, 150);
    }

    void fastForward(){
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
      analogWrite(enA, 150);
      analogWrite(enB, 150);
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


//INSTRUCTIONS TO MOVE FROM ONE MOLE TO ANOTHER------------------------------------------------------------------------------------------------------------

    //Move from left to right
    //EX: green -> blue
    void turnMoleLeftToRight(float angle, double distance1, double distance2) {
      cmReverse(distance1);
      cmPivotRight(angle);
      cmSpeed(distance2);
      cmPivotLeft(180-angle);
    }
    
    //Move from right to left
    //EX: blue -> green
    void turnMoleRightToLeft(float angle, double distance1, double distance2) {
      cmReverse(distance1);
      cmPivotLeft(angle);
      cmForward(distance2);
      cmPivotRight(180-angle);
    }


//MOVE BETWEEN MOLES------------------------------------------------------------------------------------------------------------

    //green to blue aka one - equation is ready for line following/distance integration
    //turnMoleReverse(105, 6, 20);
  
    //green to white aka two - equation is ready for line following/distance integration
    //turnMoleReverse(130, 6, 40);
   
    //green to red aka three - equation is ready for line following/distance integration
    //turnMoleReverse(160, 6, 70);
    
    //green to purple aka four - equation is ready for line following/distance integration
    //turnMoleReverse(180, 6, 60);

    //blue to red aka five - equation is ready for line following/distance integration
    //turnMoleReverse(150, 6, 50);
    
    //green to yellow aka six - equation is ready for line following/distance integration
    //turnMoleReverse(200, 6, 70);
    
    void moveMoles(){
      if ((curr == 'g' && next == 'b')
        || (curr == 'b' && next == 'w')
        || (curr =='u' && next == 'r')
        || (curr == 'r' && next == 'p')
        || (curr == 'p' && next == 'y'))
      {
        turnMoleLeftToRight(105, 6, 20);    
      }
    
      else if ((curr == 'b' && next == 'g')
        || (curr == 'w' && next == 'b')
        || (curr == 'p' && next == 'r')
        || (curr == 'y' && next == 'p'))
      {
        turnMoleRightToLeft(105, 6, 20);
      }
    
      else if ((curr == 'g' && next == 'w')
        || (curr == 'w' && next == 'r')
        || (curr == 'r' && next == 'y'))
      {
        turnMoleLeftToRight(130, 6, 40);
      }
    
      else if ((curr == 'w' && next == 'g')
        || (curr == 'r' && next == 'w')
        || (curr == 'y' && next == 'r'))
      {
        turnMoleRightToLeft(130, 6, 40);    
      }
    
      else if ((curr == 'g' && next == 'r')
        || (curr == 'b' && next == 'p')
        || (curr == 'w' && next == 'y'))
      {
        turnMoleLeftToRight(160, 6, 70);   
      }  
    
      else if ((curr == 'r' && next == 'g')
        || (curr == 'p' && next == 'b')
        || (curr == 'y' && next == 'w'))
      {
        turnMoleRightToLeft(160, 6, 60);    
      }  
    
      else if ((curr == 'g' && next == 'p')
        || (curr == 'b' && next == 'y'))
      {
        turnMoleLeftToRight(180, 6, 60);    
      }  
    
      else if ((curr == 'p' && next == 'g')
        || (curr == 'y' && next == 'b'))
      {
        turnMoleRightToLeft(180, 6, 60);    
      }  
    
      else if ((curr == 'b' && next == 'r')
        || (curr == 'w' && next == 'p'))
      {
        turnMoleLeftToRight(150, 6, 50);    
      }  
    
      else if ((curr == 'r' && next == 'b')
        || (curr == 'p' && next == 'w'))
      {
        turnMoleRightToLeft(150, 6, 50);    
      }  
    
      else if ((curr == 'g' && next == 'y') 
        || (curr == 'y' && next == 'g')){
        turnMoleLeftToRight(200, 6, 70);
      }
    }
