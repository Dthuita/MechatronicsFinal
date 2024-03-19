
#include "RobotFunctions.h"

/************** PINS **************/
#define SENSOR A1

/************** VARs **************/

// double voltages[] = {3.126, 2.29, 1.598, 1.24, 1.014, 0.869, 0.755, 0.678};
double distances[] = {5, 10, 15, 20, 25, 30, 35, 40};
double voltages[] = {3.08, 2.40, 1.65, 1.25, 1.03, 0.86, 0.75, 0.68};



/**************** FUNCs ****************/
double calcDist(){
  double input = analogRead(SENSOR);
  // Compute voltage from the sensor value
  double voltage = input * 5.0 / 1023.0;
  double distance;

  // Determine which two voltages the measurement falls between
  int i = 0;
  while(voltages[i] > voltage){ 
    i++;
    if(i != 0 && i<8) {
      double slope = (distances[i] - distances[i-1]) / (voltages[i] - voltages[i-1]); // Compute the slope = change in distance over change in voltage
      distance = distances[i-1] + slope * (voltage - voltages[i-1]); // Use slope to compute distance
    }
    // Print to the serial output
    // Serial.print("---------------------");
    // Serial.print("COMPUTED DISTANCE: ");
    // Serial.print(distance);
    // Serial.println("cm");
  }

  return distance;
} 

