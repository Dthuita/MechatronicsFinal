// Define pins
#define SENSOR A3
#define BUTTON 7

int input = 0;
int buttonState = 0;
int hasBeenPressed = 0;

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
      while(voltages[i] > voltage)
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
}