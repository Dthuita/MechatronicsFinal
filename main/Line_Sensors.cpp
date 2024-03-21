#include "RobotFunctions.h"

/************** PINS **************/
/* 
  NEED: line sensor pins -> arduino pins
  (1-8)sensor pins -> (6-13) arduino pins; ctrl pin -> (5) arduino; VCC -> 5v?; GND -> GND;
  NOTE: may need to change sensor pins
*/

/************** VARs **************/

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

/************** FUNCs **************/

void init_LineSensor(){
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){6, 7, 8, 9, 10, 11, 12, 13}, SensorCount);
  qtr.setEmitterPin(5);
}

void calibrate_lineSensor(){
  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 100; i++)
  {
    qtr.calibrate();
  }
}

void follow_line(){
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);

  Serial.print("position: ");
  Serial.println(position);

  PID_control(position);

}

void test_lineSensor(){
  //???
}
