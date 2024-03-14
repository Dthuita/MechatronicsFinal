#include <math.h>

/*************************************************************************
*  MOTOR and HBRIDGE PINS
*************************************************************************/
// Left Motor
#define LEFTPWM 3 //enA
#define LEFTA 22  //in1
#define LEFTB 24  //in2

// Right Motor
#define RIGHTPWM 2 //enB
#define RIGHTA 23  //in3
#define RIGHTB 25  //in4

// Motor pins
#define LEFTENCODEA 26
#define LEFTENCODEB 27
#define RIGHTENCODEA 28
#define RIGHTENCODEB 29

char curr = 'g';
char next = 'b';

int curr_time = 0;
int last_time = 0;

int time_check[5] = {0, 0, 0, 0, 0};
bool turn_right = 0;

int ori = 1; // robot starts facing forward, 0 means backward and 1 means forward
int w = 190; // width of wheel base

void setup() {
  // Configure H-bridge pins 
  pinMode(LEFTPWM, OUTPUT);
  pinMode(LEFTA, OUTPUT);
  pinMode(LEFTB, OUTPUT);
  pinMode(RIGHTPWM, OUTPUT);
  pinMode(RIGHTA, OUTPUT);
  pinMode(RIGHTB, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  curr_time = millis();

  //DETERMINE WHERE YOU'RE GOING
  if(curr_time - last_time <= time_check[4]){
      
      if(!ori){
        delay(1);
      }

      //1
      if ((curr == 'g' && next == 'b')
        || (curr == 'b' && next == 'w')
        || (curr == 'r' && next == 'p')
        || (curr == 'p' && next == 'y')){

        //note: mov 3.81 mm, rot 75 deg, mov 197.104 mm
        //time: 50, 750, 1200, 750, 50
        time_check[0] = 50;
        time_check[1] = 800;
        time_check[2] = 2000;
        time_check[3] = 2750;
        time_check[4] = 2800;
        last_time = curr_time;
        turn_right = 1;

        ori = !ori;
      }

      //1.5
      else if ((curr == 'b' && next == 'g')
        || (curr == 'w' && next == 'b')
        || (curr == 'p' && next == 'r')
        || (curr == 'y' && next == 'p')){

        //note: mov 3.81 mm, rot 75 deg, mov 197.104 mm
        //time: 50, 750, 2587, 750, 50
        time_check[0] = 50;
        time_check[1] = 800;
        time_check[2] = 3387;
        time_check[3] = 4137;
        time_check[4] = 4187;
        last_time = curr_time;
        turn_right = 1;
      }

      //2
      else if ((curr == 'g' && next == 'w')
        || (curr == 'w' && next == 'r')
        || (curr == 'r' && next == 'y')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 0;
      }

      //2.5
      else if ((curr == 'w' && next == 'g')
        || (curr == 'r' && next == 'w')
        || (curr == 'y' && next == 'r')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 1;
      }

      //3
      else if ((curr == 'g' && next == 'r')
        || (curr == 'b' && next == 'p')
        || (curr == 'w' && next == 'y')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 0;
      }

      //3.5
      else if ((curr == 'r' && next == 'g')
        || (curr == 'p' && next == 'b')
        || (curr == 'y' && next == 'w')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 1;
      }

      //4
      else if ((curr == 'g' && next == 'p')
        || (curr == 'b' && next == 'y')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 0;
      }

      //4.5
      else if ((curr == 'p' && next == 'g')
        || (curr == 'y' && next == 'b')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 1;
      }

      //5
      else if ((curr == 'b' && next == 'r')
        || (curr == 'w' && next == 'p')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 0;
      }

      //5.5
      else if ((curr == 'r' && next == 'b')
        || (curr == 'p' && next == 'w')){

        time_check[0] = 1;
        time_check[1] = 2;
        time_check[2] = 3;
        time_check[3] = 4;
        time_check[4] = 5;
        last_time = curr_time;
        turn_right = 1;
      }

      //6
      else if ((curr == 'g' && next == 'y')
        || (curr == 'y' && next == 'g')){

        time_check[0] = 1;
        time_check[1] = 0;
        time_check[2] = 0;
        time_check[3] = 0;
        time_check[4] = 1;
        last_time = curr_time;
        turn_right = 0;
      }
  }


  //LOGIC FOR INSTR
  if(curr_time - last_time == 0){
    if(ori){
      reverse();
    }
    else{
      forward();
    }
  }
  else if(curr_time - last_time == time_check[0]){
    if(turn_right){
      pivotRight();
    }
    else{
      pivotLeft();
    }
  }
  else if(curr_time - last_time == time_check[1]){
    if(ori){
      reverse();
    }
    else{
      forward();
    }
  }
  else if(curr_time - last_time == time_check[2]){
    if(turn_right){
      pivotRight();
    }
    else{
      pivotLeft();
    }  
  }
  else if(curr_time - last_time == time_check[3]){
    if(ori){
      reverse();
    }
    else{
      forward();
    }
  }
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
