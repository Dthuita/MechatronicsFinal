
#include "RobotFunctions.h"

/************** PINS **************/
#define BtnPin 30

/************** PINS **************/
const unsigned long PART_ONE = 30000; //how long part one, getting to wall will take

void setup() {
  
  //init sensors and motors; NOTE: dist sensor doesn't need init
  init_ColorSensor();
  init_LineSensor();
  init_Servo();
  init_wheelMotors();

  //init btn
  pinMode(BtnPin, INPUT_PULLUP);

  //calibrate line sensor
  calibrate_lineSensor();
  // Serial.print("Setup co");
}

void loop() {
  
  int START = millis();

  //On btn push START
  if( 1 == 1){ 
    //////////// run line following for some time or until wall detected ////////////
    int x, y;
    while( (x = calcDist()) > 5.0 && (y= millis()-START) < PART_ONE){

      Serial.print("dis: ");Serial.println(x);
      Serial.print("time: ");Serial.println(y);

      //linefolow
      follow_line();
      Serial.println("line following!");
    }

    ////////////wall detected so turn to knock coins////////////
    Serial.println("Coin knock!");
    Coin_Knocker();
    //go foward in 45deg angle till btn hit then back up x cm

    //sense color?

    //go to color

    //wack mole

  }

  /*
  //on btn push test
  if(digitalRead(BtnPin)){
    //motor_test();
    //line_follower_test();
    //color_sensor_test();
    //servo_motor_test();
    //dist_test();
  }
  */

}

void Coin_Knocker(){
  //pivot turn and knock first pair of coins

  pivotLeft();
  delay(2000);
  reverse();
  delay(2000);
  
  //slight turn while foward driving then back up at angle and knock next pair of coins

  forward_brake(200, 255); //move foward with slight lean to left
  delay(2000);
  reverse_brake(255, 180); //move foward with slight lean to left
  delay(2000);
}
