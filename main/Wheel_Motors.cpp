
#include "RobotFunctions.h"

/************** PINS **************/
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

//motor pins
#define LEFTENCODEA 18
#define LEFTENCODEB 27
#define RIGHTENCODEA 19
#define RIGHTENCODEB 29

/************** VARs **************/

/*************************************************************************
*  PID control system variables 
*************************************************************************/
float  Kp = 0.07; //related to the proportional control term;  //change the  value by trial-and-error (ex: 0.07).            
float Ki = 0.0008; //related to the integral  control term; //change the value by trial-and-error (ex: 0.0008).          
float  Kd = 0.5; //related to the derivative control term; //change the  value by trial-and-error (ex: 0.6).
              
int P = 0;
int I = 0;
int D = 0;

int  int_error = 0;
int  der_error = 0;
int  lastError = 0;
int  prev_lastError = 0;

int motor_cmd = 0;

int looptime = 250; //??? I made this up

/*************************************************************************
*  Motor speed variables (choose between 0 - no speed, and 255 - maximum speed)
*************************************************************************/
const  uint8_t maxspeeda = 75;
const uint8_t maxspeedb = 75;
const uint8_t basespeeda  = 50;
const uint8_t basespeedb = 50;


/************** FUNCs **************/

void init_wheelMotors(){
  //config h bridge pins 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

//pwnVal ranges 0 - 255
void forward_brake(int a, int b){
  //technically the reverse direction but works for foward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, a);
  analogWrite(enB, b);
}
void reverse_brake(int a, int b){ //go backwards
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, a);
  analogWrite(enB, b);
}
void turnLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // Spin the right motor, causing a left turn
  analogWrite(enA, 0);
  analogWrite(enB, 255);
}

/*void pivotLeft() {
  //Left motor goes backwards, right motor goes forwards
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
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
  // Set both motors to full speed
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}*/


void  PID_control(uint16_t position) {
  int error = 3500 - position; //3500 is the ideal position  (the centre)

  //Rate-limiting: cap the integrand? Is this even necessary?
  int temp_error = 0;

  if(error > 7000){
    temp_error = 7000;
  } else if(error < 0){
    temp_error = 0;
  } else{
    temp_error = error;
  }

  //will this prevent a motor from starting again?
  if(maxspeeda > motor_cmd && 0 < motor_cmd){ //Latching: if at max or min, don't let grow
     int_error = int_error + temp_error * looptime;
  }
  
  der_error = ((error - prev_lastError) / 2) / looptime; //filter sensor noise

  P = Kp*error; //ORIGINAL CODE: P = error;
  I = Ki*int_error; //ORIGINAL CODE: I = I + error;
  D = Kd*der_error; //ORIGINAL CODE: D = error - lastError;

  prev_lastError = lastError; //filter sensor noise
  lastError = error; //filter sensor noise
  motor_cmd = P + I + D;
  
  //int motorspeed = P*Kp + I*Ki + D*Kd; //calculate the correction //needed to be applied to the speed
                                       
  //int motorspeeda = basespeeda + motorspeed;
  //int motorspeedb = basespeedb -  motorspeed;

  int motorspeeda = basespeeda + motor_cmd;
  int motorspeedb = basespeedb - motor_cmd;

  // Serial.print("motorspeeda: ");
  // Serial.println(motorspeeda);
  // Serial.print("motorspeedb: ");
  // Serial.println(motorspeedb);
 
  if (motorspeeda > maxspeeda) {
    motorspeeda = maxspeeda;
  }
  if (motorspeedb > maxspeedb) {
    motorspeedb = maxspeedb;
  }
  if (motorspeeda < 0) {
    motorspeeda = 0;
  }
  if (motorspeedb < 0)  {
    motorspeedb = 0;
  } 

  // Serial.print("motorspeeda: ");
  // Serial.println(motorspeeda);
  // Serial.print("motorspeedb: ");
  // Serial.println(motorspeedb);
  
  forward_brake(motorspeeda, motorspeedb);
}

void test_motors(){
  Serial.println("Right Motor moving...");
  forward_brake(0, 255);
  delay(5000);

  Serial.println("Left Motor moving...");
  forward_brake(255, 0);
  delay(5000);
}
