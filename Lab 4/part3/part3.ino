// pin definitions
#define LEFTPWM 5
#define LEFTA 16
#define LEFTB 17
#define RIGHTPWM 6
#define RIGHTA 14
#define RIGHTB 15
#define LEFTENCODEA 2
#define LEFTENCODEB 3
#define RIGHTENCODEA 20
#define RIGHTENCODEB 21
#define DIP1 30
#define DIP2 31
#include <TimerOne.h>

int countsPerRotation = 932;
volatile int counterL = 0;
volatile int counterR = 0;
volatile int lastCounterL = 0;
volatile int lastCounterR = 0;

void setup() {
  // Encoders as input pullups
  pinMode(LEFTENCODEA, INPUT_PULLUP);
  pinMode(LEFTENCODEB, INPUT_PULLUP);
  pinMode(RIGHTENCODEA, INPUT_PULLUP);
  pinMode(RIGHTENCODEB, INPUT_PULLUP);
  // switches as inputs
  pinMode(DIP1, INPUT);
  pinMode(DIP2, INPUT);
  // Motor pins as outputs
  pinMode(LEFTPWM, OUTPUT);
  pinMode(LEFTA, OUTPUT);
  pinMode(LEFTB, OUTPUT);
  pinMode(RIGHTPWM, OUTPUT);
  pinMode(RIGHTA, OUTPUT);
  pinMode(RIGHTB, OUTPUT);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(LEFTENCODEA), leftIsr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHTENCODEA), rightIsr, CHANGE);

  // Read the two switches and convert it into a speed mode
  int speedMode = digitalRead(DIP1) * 2 + digitalRead(DIP2);
  Serial.println("---------------");
  double speed = 0.0;

  if(speedMode == 0) {
    speed = 0.25;
  }
  else if(speedMode == 1) {
    speed = 0.4; }
  else if(speedMode == 2) {
    speed = 0.7;
  }
  else {
    speed = 1.0; 
  }

  Serial.print("Selected Speed: ");
  Serial.print(speed * 100);
  Serial.println("%");

  // Move forward at the selected speed
  forwardPwm((int)(speed * 255));
  delay(1000);
  resetEncoders();
  delay(2000);

  // After 2 seconds, compute the average distance traveled by the motors to find speed
  double distPerCount = 60 * 3.1415926 / countsPerRotation / 2;
  double distL = counterL * distPerCount / 10;
  double distR = counterR * distPerCount / 10;
  double dist = (distL + distR) / 2;
  double cmPerS = dist / 2.0;

  // Print the speed
  Serial.print("Average speed: ");
  Serial.print(cmPerS);
  Serial.println("cm/s");
  brake();

}

void loop() {
}

void resetEncoders() {
  counterL = 0;
  counterR = 0;
}

void forwardPwm(int val) {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // move forward with the specificed PWM value (0-255)
  analogWrite(LEFTPWM, val);
  analogWrite(RIGHTPWM, val);
}

void brake() {
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, LOW);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void leftIsr() {
  int b = digitalRead(LEFTENCODEB);
  int a = digitalRead(LEFTENCODEA);
  //Moving counterclockwise
  if(b == a) {
    counterL--;
  }
  //Moving clockwise
  else {
    counterL++;
  }
}

void rightIsr()
{
  int b = digitalRead(RIGHTENCODEB);
  int a = digitalRead(RIGHTENCODEA);
  //Moving counterclockwise
  if(b == a) {
    counterR++;
  }
  //Moving clockwise
  else {
    counterR--;
  }
}