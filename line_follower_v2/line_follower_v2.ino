#include <QTRSensors.h>

/*************************************************************************
*  MOTOR and HBRIDGE PINS
*************************************************************************/
//h-bridge pins
#define LEFTPWM 2 //ENABLE_A
#define LEFTA 22
#define LEFTB 24
#define RIGHTPWM 1 //ENABLE_B
#define RIGHTA 23
#define RIGHTB 25 

//motor pins
#define LEFTENCODEA 26
#define LEFTENCODEB 27
#define RIGHTENCODEA 28
#define RIGHTENCODEB 29

//LED testing
#define LEFTLED 15
#define RIGHTLED 16
/*************************************************************************
*  PID control system variables 
*************************************************************************/
float  Kp = 1; //related to the proportional control term;  //change the  value by trial-and-error (ex: 0.07).            
float Ki = 0.0008; //related to the integral control term; //change the value by trial-and-error (ex: 0.0008).          
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

int left_led_state = 0;
int right_led_state = 0;
int curr_time = 0;
int prev_time = 0;

/*************************************************************************
*  Motor speed variables (choose between 0 - no speed, and 255 - maximum speed)
*************************************************************************/
const  uint8_t maxspeeda = 100;
const uint8_t maxspeedb = 100;
const uint8_t basespeeda = 50;
const uint8_t basespeedb = 50;

/*************************************************************************
*  QTR Sensor vars
*************************************************************************/
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
const int LED_PINS[] = {41, 40, 39, 38, 37, 36, 35, 34};



void setup(){
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){13, 12, 11, 10, 9, 8, 7, 6}, SensorCount);
  qtr.setEmitterPin(5);


  //SETUP LEDS
  pinMode(LEFTLED, OUTPUT);
  pinMode(RIGHTLED, OUTPUT);

  left_led_state = !left_led_state;
  right_led_state = !right_led_state;

  digitalWrite(LEFTLED, left_led_state);
  digitalWrite(RIGHTLED, right_led_state);


  delay(500);


  //set the pin modes (we're a little silly)
  pinMode(LEFTENCODEA, INPUT_PULLUP);
  pinMode(LEFTENCODEB, INPUT_PULLUP);
  pinMode(RIGHTENCODEA, INPUT_PULLUP);
  pinMode(RIGHTENCODEB, INPUT_PULLUP);
  
  pinMode(LEFTPWM, OUTPUT);
  pinMode(LEFTA, OUTPUT);
  pinMode(LEFTB, OUTPUT);
  pinMode(RIGHTPWM, OUTPUT);
  pinMode(RIGHTA, OUTPUT);
  pinMode(RIGHTB, OUTPUT);  
  

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

  left_led_state = !left_led_state;
  right_led_state = !right_led_state;

  digitalWrite(LEFTLED, left_led_state);
  digitalWrite(RIGHTLED, right_led_state);
}

void  PID_control(uint16_t position) {
  curr_time = millis();

  //if(curr_time - prev_time >= looptime){
      left_led_state = 0;
      right_led_state = 0;

      digitalWrite(LEFTLED, left_led_state);
      digitalWrite(RIGHTLED, right_led_state);
        
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
    
      int motorspeeda = basespeeda - motor_cmd;
      int motorspeedb = basespeedb + motor_cmd;
    
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
    
      if(motorspeeda < motorspeedb){
        right_led_state = 1;
        digitalWrite(RIGHTLED, right_led_state);
      } else if(motorspeedb < motorspeeda){
        left_led_state = 1;
        digitalWrite(LEFTLED, left_led_state); 
      } else{
        left_led_state = 1;
        right_led_state = 1;
        
        digitalWrite(RIGHTLED, right_led_state);
        digitalWrite(LEFTLED, left_led_state);
      }
      
      forward_brake(motorspeeda, motorspeedb);

      prev_time = curr_time;
  //}
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


  //SET LED PINS HIGH
  // Serial.print("\nLed Pos: ");
  // Serial.print(ledPos[0]);
  // Serial.println(ledPos[1]);     

  // Serial.print("On Pins: ");
  for(int i=0; i < SensorCount; i++){
    if(i != ledPos[0] && i != ledPos[1]){
      digitalWrite(LED_PINS[i], HIGH);
      // Serial.print(i);
      // Serial.print(", ");
    }
  }
  // Serial.println();



  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  /*for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);*/

  PID_control(position);

  //delay(250);
}

//pwnVal ranges 0 - 255
void  forward_brake(int posa, int posb) {
  //set the appropriate values for aphase and bphase so that the robot goes straight
  //should enable motors to move
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, HIGH);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, HIGH);

  analogWrite(LEFTPWM, posb);
  analogWrite(RIGHTPWM, posa);
}

// void forward(int pwnVal) {
//   digitalWrite(LEFTA, HIGH);
//   digitalWrite(LEFTB, LOW);
//   digitalWrite(RIGHTA, HIGH);
//   digitalWrite(RIGHTB, LOW);
//   // Set both motors to full speed
//   analogWrite(LEFTPWM, pwnVal);
//   analogWrite(RIGHTPWM, pwnVal);
// }

// void turnRight(int pwnVal) {
//   digitalWrite(LEFTA, HIGH);
//   digitalWrite(LEFTB, LOW);
//   digitalWrite(RIGHTA, HIGH);
//   digitalWrite(RIGHTB, LOW);
//   // Spin the left motor, causing a right turn
//   analogWrite(LEFTPWM, pwnVal);
//   analogWrite(RIGHTPWM, 0);
// }
// void turnLeft(int pwnVal) {
//   digitalWrite(LEFTA, HIGH);
//   digitalWrite(LEFTB, LOW);
//   digitalWrite(RIGHTA, HIGH);
//   digitalWrite(RIGHTB, LOW);
//   // Spin the right motor, causing a left turn
//   analogWrite(LEFTPWM, 0);
//   analogWrite(RIGHTPWM, pwnVal);
// }
