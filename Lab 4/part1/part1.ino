// pin definitions
/*#define LEFTPWM 5
#define LEFTA 16
#define LEFTB 17
#define RIGHTPWM 6
#define RIGHTA 14
#define RIGHTB 15
*/

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

void setup() {
  Serial.begin(9600);

  // Set all motor pins to be outputs
  pinMode(LEFTPWM, OUTPUT);
  pinMode(LEFTA, OUTPUT);
  pinMode(LEFTB, OUTPUT);
  pinMode(RIGHTPWM, OUTPUT);
  pinMode(RIGHTA, OUTPUT);
  pinMode(RIGHTB, OUTPUT);

  pinMode(LEFTENCODEA, INPUT_PULLUP);
  pinMode(LEFTENCODEB, INPUT_PULLUP);
  pinMode(RIGHTENCODEA, INPUT_PULLUP);
  pinMode(RIGHTENCODEB, INPUT_PULLUP);

  // Print instructions
  Serial.println("1: Forward");
  Serial.println("2: Reverse");
  Serial.println("3: Brake");
  Serial.println("4: Coast");
  Serial.println("5: Turn Left");
  Serial.println("6: Turn Right");
  Serial.println("7: Pivot Left");
  Serial.println("8: Pivot Right");
  Serial.println("Enter an option (1-8):");
}

void loop() {
  while (Serial.available()==0){}
  int mode = Serial.parseInt();

  // Convert an integer to a command
  switch(mode) {
    case 1:
      forward();
      break;
    case 2:
      reverse();
      break;
    case 3:
      brake();
      break;
    case 4:
      coast();
      break;
    case 5:
      turnLeft();
      break;
    case 6:
      turnRight();
      break;
    case 7:
      pivotLeft();
      break;
    case 8:
      pivotRight();
      break;
    case 0:
      break;
    default:
      Serial.println("Invalid option");
      break; 
    }
}

void forward() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Set both motors to full speed
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void reverse() {
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, HIGH);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, HIGH);
  // Set both motors to full speed
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void brake() {
  // Turn off all motors
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, LOW);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void coast() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Set motors to 0% but keep them coasting
  analogWrite(LEFTPWM, 0);
  analogWrite(RIGHTPWM, 0);
}

void turnRight() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Spin the left motor, causing a right turn
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 0);
}

void turnLeft() {
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  // Spin the right motor, causing a left turn
  analogWrite(LEFTPWM, 0);
  analogWrite(RIGHTPWM, 255);
}

void pivotLeft() {
  //Left motor goes backwards, right motor goes forwards
  digitalWrite(LEFTA, LOW);
  digitalWrite(LEFTB, HIGH);
  digitalWrite(RIGHTA, HIGH);
  digitalWrite(RIGHTB, LOW);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}

void pivotRight() {
  //Right motor goes backwards, left motor goes forwards
  digitalWrite(LEFTA, HIGH);
  digitalWrite(LEFTB, LOW);
  digitalWrite(RIGHTA, LOW);
  digitalWrite(RIGHTB, HIGH);
  analogWrite(LEFTPWM, 255);
  analogWrite(RIGHTPWM, 255);
}