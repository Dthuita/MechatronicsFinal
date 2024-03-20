//coin slot code

//theoretically muba is writing

void setupCoinSlot(){
  //setup function here
  Serial.println("setupCoinSlot() done");
}

void loopCoinSlot(){
  Coin_Knocker();
  
  flag = 3;
  Serial.println("loopCoinSlot() done");

  delay(500);
}

void Coin_Knocker(){
//pivot turn and knock first pair of coins
  cmPivotLeft(90);
  cmReverse(10);
  
  //slight turn while foward driving then back up at angle and knock next pair of coins
  Serial.println("Move  foward left angled");
  cmForward(3);
  cmPivotLeft(45);
  cmForward(7);

  Serial.println("pivot to angle");
  cmPivotRight(45);
  
  Serial.println("Move backward right angled");
  cmReverse(10);
}
