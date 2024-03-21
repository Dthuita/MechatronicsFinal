
#include "RobotFunctions.h"

/************** PINS **************/
#define BtnPin 30

#define led1 36
#define led2 38
#define led3 40
#define led4 42
#define led5 44
#define led6 46
#define led7 48
#define led8 50

/************** VARIABLES **************/
const unsigned long PART_ONE = 30000; //how long part one, getting to wall will take
const unsigned long PART_TWO = 5000; //how long recentering on the line will take as fail safe

char curr = 'u';
char next = 'r';

void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);

  //init sensors and motors; NOTE: dist sensor doesn't need init
  init_ColorSensor();
  init_LineSensor();
  init_Servo();
  init_wheelMotors();
  
  init_moveMole();

  //init btn
  pinMode(BtnPin, INPUT_PULLUP);

  //calibrate line sensor
  calibrate_lineSensor();
  digitalWrite(led1, HIGH);
}

void loop() {
  
  //On btn push START
  if( digitalRead(BtnPin) == LOW){ 
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
    
    // MEGAN ADDED CODE STARTS HERE//
    coin2();
    digitalWrite(led2, HIGH);

    /////////// begin mole whacking ////////////
    while(1){
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      
      next = get_color();
      next = get_color();
      next = get_color();

      digitalWrite(led3, HIGH);
      
      moveMoles();

      digitalWrite(led4, HIGH);

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

      digitalWrite(led5, HIGH);
      
      swing_hammer();

      digitalWrite(led6, HIGH);
      delay(500);
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
  }*/
  
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
  cmForward(4);
  cmPivotLeft(80);
  cmReverse(20);

  cmForward(20);
  cmPivotLeft(80);
  cmForward(20);

  cmPivotRight(80);
  cmReverse(25);
  
  cmForward(25);
  cmPivotRight(80);
  cmForward(12);
  
  cmPivotLeft(85);
  cmForward(60);

  int START = millis();
  //////////// run line following for some time or until wall detected ////////////
  int x, y;
  while( (x = calcDist()) > 5.0 && (y= millis()-START) < 5000){

    Serial.print("dis: ");Serial.println(x);
    Serial.print("time: ");Serial.println(y);

    //linefolow
    follow_line();
    Serial.println("line following!");
  }

  fastForward();
  delay(1000);
}
