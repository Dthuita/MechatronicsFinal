//INCLUDE STATEMENTS------------------------------------------------------------------------------------------------------------
    #include <QTRSensors.h>

//PIN SETUP------------------------------------------------------------------------------------------------------------
    //DISTANCE SENSOR PINS------------------------------------------------------------------------------------------------------------
        #define SENSOR A0 // IR Proximity Sensor

//VARIABLE SETUP------------------------------------------------------------------------------------------------------------
    //DISTANCE SENSOR VARIABLES------------------------------------------------------------------------------------------------------------
      int distance;
      int input = 0;

      // Measured voltages
        double voltages[] = {
          3.126, 2.29, 1.598, 1.24, 1.014, 0.869, 0.755, 0.678
        };
      
      // Distance points used for measurements
        double distances[] = {
          5, 10, 15, 20, 25, 30, 35, 40
        };

    //LINE FOLLOWING VARIABLES------------------------------------------------------------------------------------------------------------
        //PID VARIABLES------------------------------------------------------------------------------------------------------------
            float  Kp = 0.095; // Proportional control term
            float Ki = 0.0008; // Integral control term
            float  Kd = 0.6;   // Derivative control term
            
            int P = 0;
            int I = 0;
            int D = 0;
            
            int  int_error = 0;
            int  der_error = 0;
            int  lastError = 0;
            int  prev_lastError = 0;
            
            int motor_cmd = 0;
            
            int looptime = 250;

        //MOTOR SPEED VARIABLES------------------------------------------------------------------------------------------------------------
            const  uint8_t maxspeeda = 255;
            const uint8_t maxspeedb = 255;
            const uint8_t basespeeda  = 255;
            const uint8_t basespeedb = 255;

        //LINE FOLLOWING SENSOR VARIABLES------------------------------------------------------------------------------------------------------------
            QTRSensors qtr;
            const uint8_t SensorCount = 8;
            uint16_t sensorValues[SensorCount];
            //const int LED_PINS[] = {41, 40, 39, 38, 37, 36, 35, 34};


//FUNCTIONITIZED SETUP------------------------------------------------------------------------------------------------------------
    void setupLineDist(){
      // Configure the sensors
      qtr.setTypeRC();
      qtr.setSensorPins((const uint8_t[]){6, 7, 8, 9, 10, 11, 12, 13}, SensorCount);
      qtr.setEmitterPin(5);
    
      delay(500);
    
//      // Configure the LEDs
//      for(int i=0; i < SensorCount; i++){
//        pinMode(LED_PINS[i], OUTPUT);
//        digitalWrite(LED_PINS[i], LOW);
//      }
    
      // Calibrate QTR sensors
      for (uint16_t i = 0; i < 400; i++) {
        qtr.calibrate();
      }
    
      // Print the calibration values
      for (uint8_t i = 0; i < SensorCount; i++) {
        Serial.print(qtr.calibrationOn.minimum[i]);
        Serial.print(' ');
      }
      Serial.println();
      for (uint8_t i = 0; i < SensorCount; i++) {
        Serial.print(qtr.calibrationOn.maximum[i]);
        Serial.print(' ');
      }
      Serial.println();

      Serial.println("about to delay");
      delay(1000);

      Serial.println("calibration done");
    }

//DISTANCE FUNCTIONS------------------------------------------------------------------------------------------------------------
    void measureDistance() {
      input = analogRead(SENSOR);
      
      // Compute voltage from the sensor value
      double voltage = input * 5.0 / 1023.0;
      int i = 0;
      
      // Determine which two voltages the measurement falls between
      while(voltages[i] > voltage){
        i++;
      }
        
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


//LINE FOLLOWING FUNCTIONS------------------------------------------------------------------------------------------------------------
    void PID_control(uint16_t position) {
      int error = 3500 - position;
    
      int temp_error = 0;
      if(error > 7000){
        temp_error = 7000;
      } else if(error < 0){
        temp_error = 0;
      } else{
        temp_error = error;
      }
    
      if(maxspeeda > motor_cmd && 0 < motor_cmd){
         int_error = int_error + temp_error * looptime;
      }
      
      der_error = ((error - prev_lastError) / 2) / looptime;
    
      P = Kp*error;
      I = Ki*int_error;
      D = Kd*der_error;
    
      prev_lastError = lastError;
      lastError = error;
      motor_cmd = P + I + D;
      
      int motorspeeda = basespeeda + motor_cmd;
      int motorspeedb = basespeedb - motor_cmd;
    
      if (motorspeeda > maxspeeda) {
        motorspeeda = maxspeeda;
      }
      if (motorspeedb > maxspeedb) {
        motorspeedb = maxspeedb;
      }
      if (motorspeeda < 0) {
        motorspeeda = 0;
      }
      if (motorspeedb < 0) {
        motorspeedb = 0;
      } 
    
      forward_brake(motorspeeda, motorspeedb);
    }
    
    void forward_brake(int a, int b){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    
      analogWrite(enA, a);
      analogWrite(enB, b);
    }

//LOOP FUNCTIONITIZED------------------------------------------------------------------------------------------------------------
    void loopLineDist(int vers) {
      measureDistance(); // Measure distance to the nearest obstacle

      if(vers == 2){
        if (distance > 8) { // Proceed if the distance is greater than 8 cm
          uint16_t position = qtr.readLineBlack(sensorValues);
          Serial.print("Position: ");
          Serial.println(position);
      
          PID_control(position); // Execute PID control based on line position
      
        } else {
          forward_brake(0, 0); // Stop the motors if too close to an obstacle
          Serial.println("Too close to an obstacle, stopping. AKA done with loopLineDist vers 2.");
        }    
      }
      if(vers == 3){
        if (distance > 30) { // Proceed if the distance is greater than 8 cm
          uint16_t position = qtr.readLineBlack(sensorValues);
          Serial.print("Position: ");
          Serial.println(position);
      
          PID_control(position); // Execute PID control based on line position
      
        } else {
          forward_brake(0, 0); // Stop the motors if too close to an obstacle
          Serial.println("Too close to an obstacle, stopping. AKA done with loopLineDist vers 3.");
        }
      }
      else{
        if (distance > 8) { // Proceed if the distance is greater than 8 cm
          uint16_t position = qtr.readLineBlack(sensorValues);
          Serial.print("Position: ");
          Serial.println(position);
      
          PID_control(position); // Execute PID control based on line position
      
        } else {
          forward_brake(0, 0); // Stop the motors if too close to an obstacle
          Serial.println("Too close to an obstacle, stopping. AKA done with loopLineDist.");
          
          if(vers == 0){
            flag = 2;
            delay(500);
          }
          else if(vers == 1){
            whack = 3;
            delay(500);
          }
        }
      }
    }