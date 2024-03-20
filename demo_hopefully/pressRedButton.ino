//pressing the red button code

//theoretically muba is writing

void setupButton(){
  //setup function here
}

void loopPressButton(){
  //line follow until 30 cm
  loopLineDist(2);
  
  //turn 90 degrees
  cmPivotRight(90);
  
  //line follow until 1.5 inch = roughly 5 cm
  loopLineDist(3);
  
  //move from button to white
  turnMoleRightToLeft(105, 6, 20);  
  
  //line follow until 1.5 inch = roughly 5 cm
  loopLineDist(1);
  
  //update
  flag = 4;
  Serial.println("loopPressButton() done");
}

//post coins, assume that the robot is centered, or that we can get him centered
