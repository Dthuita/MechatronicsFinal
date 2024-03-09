/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// Define pins
#define SENSOR_PIN A1  // Analog pin used to connect the distance sensor
#define SERVO_PIN 9    // Servo control pin

// Distance measurement arrays
double voltages[] = {3.126, 2.29, 1.598, 1.24, 1.014, 0.869, 0.755, 0.678}; // Measured voltages for specific distances
double distances[] = {5, 10, 15, 20, 25, 30, 35, 40}; // Distances in cm

// Function to calculate distance based on sensor voltage
double calculateDistance(double voltage) {
  for (int i = 0; i < sizeof(voltages)/sizeof(voltages[0]) - 1; i++) {
    if (voltage >= voltages[i+1] && voltage <= voltages[i]) {
      // Linear interpolation
      return distances[i] + (distances[i+1] - distances[i]) * (voltage - voltages[i]) / (voltages[i+1] - voltages[i]);
    }
  }
  return 0; // Return 0 if no matching range is found
}

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 bps
  //pinMode(SENSOR_PIN, OUTPUT);
  myservo.attach(SERVO_PIN); // Attaches the servo on pin 9 to the servo object
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN); // Read the value from the distance sensor
  double voltage = sensorValue * (5.0 / 1023.0); // Convert sensor value to voltage
  double distance = calculateDistance(voltage); // Calculate distance based on sensor voltage

  // Map distance to servo position (adjust range as needed)
  int servoPos = map(distance, 5, 40, 180, 0);

  myservo.write(servoPos); // Move servo to position based on distance

  // Debugging output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Servo Position: ");
  Serial.println(servoPos);

  delay(100); // Delay a little bit to make the servo's movement smoother
}

