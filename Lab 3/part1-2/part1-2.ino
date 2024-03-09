#define BUTTON 2
int counter = 0;
int lastCounter = 0;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  //Create an interupt for the pin
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonIsr, RISING);
  Serial.begin(9600);
}

void loop() {
  //Check if the counter changed, and if it did, print the new value
  if(counter != lastCounter) {
    Serial.print("Button has been pressed ");
    Serial.print(counter);
    Serial.println(" times.");
    lastCounter = counter;
  } 
}

//Increment the counter on button press
void buttonIsr() {
  counter = counter + 1;
}