#include <QTRSensors.h>

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
#define LEFTENCODEA 26
#define LEFTENCODEB 27
#define RIGHTENCODEA 28
#define RIGHTENCODEB 29

/*************************************************************************
*  PID control system variables 
*************************************************************************/
float  Kp = 0.07; //related to the proportional control term;  //change the  value by trial-and-error (ex: 0.07).            
float Ki = 0.0008; //related to the integral  control term; //change the value by trial-and-error (ex: 0.0008).          
float  Kd = 0.6; //related to the derivative control term; //change the  value by trial-and-error (ex: 0.6).
              
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
const  uint8_t maxspeeda = 150;
const uint8_t maxspeedb = 150;
const uint8_t basespeeda  = 100;
const uint8_t basespeedb = 100;

/*************************************************************************
*  QTR Sensor vars
*************************************************************************/
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
const int LED_PINS[] = {41, 40, 39, 38, 37, 36, 35, 34};



void setup(){
  //config h bridge pins 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){13, 12, 11, 10, 9, 8, 7, 6}, SensorCount);
  qtr.setEmitterPin(5);

  delay(500);

  //configure the LEDs
  for(int i=0; i < SensorCount; i++){
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

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

  Serial.print("motorspeeda: ");
  Serial.println(motorspeeda);
  Serial.print("motorspeedb: ");
  Serial.println(motorspeedb);
 
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

  Serial.print("motorspeeda: ");
  Serial.println(motorspeeda);
  Serial.print("motorspeedb: ");
  Serial.println(motorspeedb);
  
  forward_brake(motorspeeda, motorspeedb);
}

void loop(){
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

  Serial.print("position: ");
  Serial.println(position);

  int ledPos[2];
  int p=0;

  if(position >= 0 && position < 1000){
    digitalWrite(LED_PINS[0], LOW);
    ledPos[p] = 0;
    p++;
  }
  
  if(position > 0 && position < 2000){
    digitalWrite(LED_PINS[1], LOW);
    ledPos[p] = 1;
    p++;
  }
  
  if(position > 1000 && position < 3000){
    digitalWrite(LED_PINS[2], LOW);
    ledPos[p] = 2;
    p++;
  }

  if(position > 2000 && position < 4000){
    digitalWrite(LED_PINS[3], LOW);
    ledPos[p] = 3;
    p++;
  }

  if(position > 3000 && position < 5000){
    digitalWrite(LED_PINS[4], LOW);
    ledPos[p] = 4;
    p++;
  }

  if(position > 4000 && position < 6000){
    digitalWrite(LED_PINS[5], LOW);
    ledPos[p] = 5;
    p++;    
  }

  if(position > 5000 && position < 7000){
    digitalWrite(LED_PINS[6], LOW);
    ledPos[p] = 6;
    p++;
  }

  if(position > 6000){
    digitalWrite(LED_PINS[7], LOW);
    ledPos[p] = 7;
    p++;
  }


  // Serial.print("On Pins: ");
  for(int i=0; i < SensorCount; i++){
    if(i != ledPos[0] && i != ledPos[1]){
      digitalWrite(LED_PINS[i], HIGH);
      // Serial.print(i);
      // Serial.print(", ");
    }
  }
  // Serial.println();


  PID_control(position);

  //delay(250);
}

//pwnVal ranges 0 - 255
void forward_brake(int a, int b){
  //technically the reverse direction but works for foward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, a);
  analogWrite(enB, b);
}
