/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

// Define pins
#define SENSOR A3
#define BUTTON 7

Servo myservo;  // create servo object to control a servo


int input = 0;
int buttonState = 0;
int hasBeenPressed = 0;

int potpin = A0;  // analog pin used to connect the potentiometer
int val;          // variable to read the value from the analog pin


// Measured voltages
double voltages[] = {
  3.126, 2.29, 1.598, 1.24, 1.014, 0.869, 0.755, 0.678
};


// Distance points used for measurements
double distances[] = {
  5, 10, 15, 20, 25, 30, 35, 40
};


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  buttonState = digitalRead(BUTTON);
  // Check if button has been pressed
  if(buttonState == HIGH) {

  // Check if button has been processed
  if(!hasBeenPressed) {

  // Make sure logic does not run again
  hasBeenPressed = 1;


  // Read sensor input
  input = analogRead(SENSOR);

  // Compute voltage from the sensor value
  double voltage = input * 5.0 / 1023.0;


  int i = 0;

  // Determine which two voltages the measurement falls between
  while(voltages[i] > voltage){
    i++;
    // Avoid out of bounds error
    if(i != 0 && i<8) {
      // Compute the slope = change in distance over change in voltage
      double slope = (distances[i] - distances[i-1]) / (voltages[i] - voltages[i-1]);
    
    
      // Use slope to compute distance
      double distance = distances[i-1] + slope * (voltage - voltages[i-1]);
      
      
      // Print to the serial output
      Serial.print("---------------------");
      Serial.print("COMPUTED DISTANCE: ");
      Serial.print(distance);
      Serial.println("cm");
      }
    }
  }
  else {
  // Allow button press to trigger the logic to run again
  hasBeenPressed = 0;
  }


  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 30, 120);    // scale it for use with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there

}
