#ifndef ROBOT_FUNCS_H
#define ROBOT_FUNCS_H

#include <Arduino.h>
#include <QTRSensors.h>
#include <Servo.h>
#include <stdint.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <TimerOne.h>

/***************** COLOR SENSOR *****************/
  //init
void init_ColorSensor(void);
//funcs
int getColorDis(int red, int green, int blue, int lux, int cTemp, int SAMPLE[5]);
char discrim(int R, int G, int B, int LUX, int CTEMP);
char get_color(void);
//tests
void color_test();  

/***************** DIST SENSOR *****************/
double calcDist(); //func

/***************** SERVO MOTOR *****************/
void init_Servo(); //init
void swing_hammer(); //funcs
void servo_test(); //test

/***************** WHEEL MOTORS *****************/
//init
void init_wheelMotors(); 
//funcs
void forward_brake(int a, int b);
void reverse_brake(int a, int b);
void turnLeft();
void pivotLeft();
void reverse();
void PID_control(uint16_t position);
//test
void test_motors();

/***************** LINE SENSORS *****************/
//init
void init_LineSensor(); 
//funcs
void calibrate_lineSensor();
void follow_line();
//test
void test_lineSensor();

/***************** MEGAN'S CODE *****************/
void cmForward(int x);

#endif
