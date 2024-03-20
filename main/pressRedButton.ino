//pressing the red button code

void setupButton(){
  //setup function here
}

void loopPressButton(){
  cmForward(30);
  
  //turn 90 degrees
  cmPivotRight(90);
  
  //line follow until 1.5 inch = roughly 5 cm
  cmForward(5);
  
  //move from button to white
  turnMoleRightToLeft(105, 6, 20);  
  
  //line follow until 1.5 inch = roughly 5 cm
  int x, y, START;
  START = millis();
  while( (x = calcDist()) > 5.0 && (y= millis()-START) < 10000){

    Serial.print("dis: ");Serial.println(x);
    Serial.print("time: ");Serial.println(y);

    //linefolow
    follow_line();
    Serial.println("line following!");
  }
  
  //update
  Serial.println("loopPressButton() done");
}

//post coins, assume that the robot is centered, or that we can get him centered
