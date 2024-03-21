
#include "RobotFunctions.h"

/************** PINS **************/
#define BtnPin 30

/************** VARIABLES **************/
const unsigned long PART_ONE = 30000; //how long part one, getting to wall will take
const unsigned long PART_TWO = 10000; //how long recentering on the line will take as fail safe

char curr = 'u';
char next = 'r';

void setup() {
  
  //init sensors and motors; NOTE: dist sensor doesn't need init
  init_ColorSensor();
  init_LineSensor();
  init_Servo();
  init_wheelMotors();
  
  init_moveMole();

  //init btn
  //pinMode(BtnPin, INPUT_PULLUP);

  //calibrate line sensor
  calibrate_lineSensor();
   Serial.print("Setup co");
}

void loop() {
  /*
  //On btn push START
  if( 1 == 1){ 
    int START = millis();
    //////////// run line following for some time or until wall detected ////////////
    int x, y;
    while( (x = calcDist()) > 5.0 && (y= millis()-START) < PART_ONE){

      Serial.print("dis: ");Serial.println(x);
      Serial.print("time: ");Serial.println(y);

      //linefolow
      follow_line();
      Serial.println("line following!");
    }

    //////////// wall detected so turn to knock coins ////////////
    Serial.println("Coin knock!");
    //Coin_Knocker();
    coin2();

    // MEGAN ADDED CODE STARTS HERE//
    
    /////////// hit button ///////////
    //loopPressButton();

    /////////// begin mole whacking ////////////
    while(1){
      next = get_color();
      next = get_color();
      next = get_color();
      
      moveMoles();

      curr = next;

      START = millis();
  
      int x, y;
      while( (x = calcDist()) > 5.0 && (y= millis()-START) < PART_TWO){
  
        Serial.print("dis: ");Serial.println(x);
        Serial.print("time: ");Serial.println(y);
  
        //linefolow
        follow_line();
        Serial.println("line following!");
      }
      
      swing_hammer();
    }
  }

  /*
  //on btn push test
  if(digitalRead(BtnPin)){
    //motor_test();
    //line_follower_test();
    //color_sensor_test();
    '';

    
    //servo_motor_test();
    //dist_test();
  }
  */

  

}

void Coin_Knocker(){
//pivot turn and knock first pair of coins
  pivotLeft();
  delay(1000);
  reverse();
  delay(2000);
  
  //slight turn while foward driving then back up at angle and knock next pair of coins
  Serial.println("Move  foward left angled");
  forward_brake(100, 250);
  delay(2100);

  Serial.println("pivot to angle");
  forward_brake(250, 0);
  delay(900);

  Serial.println("Move backward right angled");
  reverse_brake(250, 250);
  delay(2500);
}

void coin2(){
  cmForward(3);
  cmPivotLeft(100);
  cmReverse(10);

  cmForward(10);
  cmPivotLeft(100);
  cmForward(20);

  cmPivotRight(100);
  cmReverse(10);
  
  cmForward(10);
  cmPivotRight(100);
  cmForward(12);
  
  cmPivotLeft(110);
  cmForward(75);

  for (int i = 0; i < 5; i++)
    follow_line();
}
