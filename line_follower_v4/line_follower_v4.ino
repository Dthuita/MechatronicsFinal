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

// Motor pins
#define LEFTENCODEA 26
#define LEFTENCODEB 27
#define RIGHTENCODEA 28
#define RIGHTENCODEB 29

/*************************************************************************
*  PID control system variables 
*************************************************************************/
float Kp = 0.07;             
float Ki = 0.0008;         
float Kd = 0.6;              

int P = 0;
int I = 0;
int D = 0;

int int_error = 0;
int der_error = 0;
int lastError = 0;
int prev_lastError = 0;

int motor_cmd = 0;

int looptime = 250; // Time interval for loop execution

/*************************************************************************
*  Motor speed variables
*************************************************************************/
const uint8_t maxspeeda = 150;
const uint8_t maxspeedb = 150;
const uint8_t basespeeda = 100;
const uint8_t basespeedb = 100;

/*************************************************************************
*  QTR Sensor vars
*************************************************************************/
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
const int LED_PINS[] = {41, 40, 39, 38, 37, 36, 35, 34};

void setup() {
  // Configure H-bridge pins 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){13, 12, 11, 10, 9, 8, 7, 6}, SensorCount);
  qtr.setEmitterPin(5);

  delay(500);

  // Configure the LEDs
  for (int i = 0; i < SensorCount; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Calibrate sensors
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }

  // Print calibration values
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void PID_control(uint16_t position) {
  int error = 3500 - position;

  int temp_error = error;
  temp_error = constrain(temp_error, 0, 7000);

  if(maxspeeda > motor_cmd && motor_cmd > 0) {
    int_error += temp_error * looptime;
  }

  der_error = (error - prev_lastError) / (2 * looptime);

  P = Kp * error;
  I = Ki * int_error;
  D = Kd * der_error;

  prev_lastError = lastError;
  lastError = error;
  motor_cmd = P + I + D;

  int motorspeeda = constrain(basespeeda + motor_cmd, 0, maxspeeda);
  int motorspeedb = constrain(basespeedb - motor_cmd, 0, maxspeedb);

  Serial.print("motorspeeda: ");
  Serial.println(motorspeeda);
  Serial.print("motorspeedb: ");
  Serial.println(motorspeedb);

  forward_brake(motorspeeda, motorspeedb);
}

void loop() {
  // Read calibrated sensor values and obtain a measure of the line position
  uint16_t position = qtr.readLineBlack(sensorValues);
  Serial.print("position: ");
  Serial.println(position);

  // Update LEDs based on position
  // (Your existing LED code should be here, adjusted as necessary.)

  PID_control(position);

  //delay(looptime); // Uncomment this if you need to slow down the loop execution
}

void forward_brake(int a, int b) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, a);
  analogWrite(enB, b);
}

void turnAngle(float angle) {
  // placeholder values
  unsigned long turnTime = 0;
  if (angle == 30) turnTime = 500; // Adjust these times for your robot
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
