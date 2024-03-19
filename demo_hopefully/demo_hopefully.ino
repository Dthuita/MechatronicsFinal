//MOTOR AND HBRIDGE PINS------------------------------------------------------------------------------------------------------------
  //LEFT MOTOR
    #define enA 3
    #define in1 22
    #define in2 24
    
  //RIGHT MOTOR 
    #define enB 2
    #define in3 23
    #define in4 25
    
  //MOTOR ENCODES
    #define LEFTENCODEA 18
    #define LEFTENCODEB 27
    #define RIGHTENCODEA 19
    #define RIGHTENCODEB 29

//COlOR SENSOR: SDA 20, SCL 21
//SERVO MOTOR: PowerPin 32, ServoPin 33
//DISTANCE SENSOR: A0
    
//MAIN (demo_hopefully) PINS------------------------------------------------------------------------------------------------------------
  #define startButtonPin 37
  
  #define colorSensorDetectedLed 40
  #define calibrationDoneLed 41
  #define lineFollowingStartedLed 42
  #define coinSlotStartedLed 43
  #define redButtonPressStartedLed 44
  
  //moles mode activated -- flash starting with outermost so we recognize that its different and something isn't wrong
    #define moveMoleActiveLed 47
    #define lineDistActiveLed 46
    #define hitMoleActiveLed 45

//MAIN (demo_hopefully) VARIABLES------------------------------------------------------------------------------------------------------------
  int flag = 0;
  int whack = 0;

//moveMole VARIABLES------------------------------------------------------------------------------------------------------------
  // Color memory
    char curr = 'w';
    char next = 'w';
    
    



//SETUP FUNCTION------------------------------------------------------------------------------------------------------------
  void setup(){
    Serial.begin(9600);
    
    //CONFIGURE MOTOR AND HBRIDGE PINS 
      pinMode(enA, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(enB, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);

    //CONFIGURE LED PINS
      pinMode(colorSensorDetectedLed, OUTPUT);
      pinMode(calibrationDoneLed, OUTPUT);
      pinMode(lineFollowingStartedLed, OUTPUT);
      pinMode(coinSlotStartedLed, OUTPUT);
      pinMode(redButtonPressStartedLed, OUTPUT);
      pinMode(moveMoleActiveLed, OUTPUT);
      pinMode(lineDistActiveLed, OUTPUT);
      pinMode(hitMoleActiveLed, OUTPUT);

      digitalWrite(colorSensorDetectedLed, LOW);
      digitalWrite(calibrationDoneLed, LOW);
      digitalWrite(lineFollowingStartedLed, LOW);
      digitalWrite(coinSlotStartedLed, LOW);
      digitalWrite(redButtonPressStartedLed, LOW);
      digitalWrite(moveMoleActiveLed, LOW);
      digitalWrite(lineDistActiveLed, LOW);
      digitalWrite(hitMoleActiveLed, LOW);

      Serial.println("LEDs Written");
      
    //CONFIGURE START BUTTON PIN
      pinMode(startButtonPin, INPUT_PULLUP);

    //SETUP EACH FILE 
      setupCoinSlot();
      setupButton();
      
      setupColorSensor();   //find color sensor
      digitalWrite(colorSensorDetectedLed, HIGH);
      setupHitMole();       //attach servo
      setupMoveMole();      //attach isr's
  
      setupLineDist();      //calibrate line follower (dist sensor done after pin recognition)
      digitalWrite(calibrationDoneLed, HIGH);
      
      Serial.println("done with setup");
  }



//LOOP FUNCTION------------------------------------------------------------------------------------------------------------
  void loop(void){
    
    //code that is running after setup but before button press
    if(flag == 0){
      if(digitalRead(startButtonPin) == HIGH){ 
        flag = 1; //once start button is pressed, ready to move onto next phase aka start
        Serial.println("button pressed");
      }
    } 

    //after button press, before distance detected
    //lineFollowDistanceSensor
    else if(flag == 1){
      digitalWrite(lineFollowingStartedLed, HIGH);
      loopLineDist(0);
    }

    //after distance detected, before coinSlot marks phase as complete
    else if(flag == 2){
      digitalWrite(coinSlotStartedLed, HIGH);
      loopCoinSlot();
    }

    //after coinSlot marks phase as complete, before pressRedButton marks phase as complete
    else if(flag == 3){
      digitalWrite(redButtonPressStartedLed, HIGH);
      loopPressButton();
    }

    //time to whack!
    //triggered by pressRedButton phase being done
    else if(flag == 4){
      if(whack == 0){         //should happen 3 times total, so after third call, set flag to move on
        colorReading();
        colorReading();
        colorReading();
        
        whack = 1;
      }

      else if(whack == 1){    //should happen 1 time, update where we are then set flag to move on
        digitalWrite(hitMoleActiveLed, LOW);
        digitalWrite(moveMoleActiveLed, HIGH);
        moveMoles();
        
        curr = next;
        whack = 2;
      }

      else if(whack == 2){    //should happen until distance limit detected
        digitalWrite(moveMoleActiveLed, LOW);
        digitalWrite(lineDistActiveLed, HIGH);
        loopLineDist(1);
      }

      else if(whack == 3){    //should happen 1 time
        digitalWrite(lineDistActiveLed, LOW);
        digitalWrite(hitMoleActiveLed, HIGH);
        loopHitMole();
        whack = 0;
      }
    }
  
  }
