#define RED 44
#define GREEN 42
#define BLUE 46
#define PHOTOIN A0

#define LED 32 // green
#define LED 34 // red
#define LED 36 // blue

unsigned long previousMillis = 0;
const long interval = 100; // interval at which to blink (milliseconds)

unsigned long discrimPreviousMillis = 0;
const long discrimInterval = 1000;

int redResponseTime = 350;
int greenResponseTime = 330;
int blueResponseTime = 390;

/*
void discrim(int r, int g, int b){
  unsigned long currentMillis = millis();
  //paper is white
  if( r > 350 && g > 350 && b > 350 ){
    Serial.print("paper is white");
    digitalWrite(32, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(36, HIGH);
  }else if( r < 200 && b < 200 && g < 200){ //paper is black
    Serial.print("paper is black");
    digitalWrite(34, LOW);
    digitalWrite(36, LOW);
    digitalWrite(32, LOW);
    //no leds on
  }else if(b >= 300){ //blue paper
    Serial.print("paper is blue");
    digitalWrite(36, HIGH);
    digitalWrite(32, LOW);
    digitalWrite(34, LOW);
  }else if(g >= 260) {//green paper
    Serial.print("paper is green");
    digitalWrite(32, HIGH);
    digitalWrite(36, LOW);
    digitalWrite(34, LOW);
  }else if(r >= 420){ //red paper
    Serial.print("paper is red");
    digitalWrite(34, HIGH);
    digitalWrite(32, LOW);
    digitalWrite(36, LOW);
  }

  if (currentMillis - discrimPreviousMillis >= discrimInterval) {
    discrimPreviousMillis = currentMillis;
    digitalWrite(34, LOW);
    digitalWrite(36, LOW);
    digitalWrite(32, LOW);
  }

  Serial.println("\n");
}
*/

int COLOR_SAMPLE[][3] = { //RGB
   {503, 399, 499},       //white
   {163, 165, 177},       //black
   {423, 174, 199},         //red
   {199, 265, 243},       //green
   {173, 204, 309}         //blue
}

int getColorDis(int red, int green, int blue, int redSample, int greenSample, int blueSample){
   return sqrt( pow(red - redSample, 2) + pow(green - greenSample, 2) + pow(blue - blueSample, 2) );
}

char* discrim(int R, int G, int B){
   int colorDis;               //color dis
   int best_colDis = 3000;            //what it says
   char* possible_color;       //least dis


   for(int i=0; i< 5; i++){
       colorDis = getColorDis(R, G, B, COLOR_SAMPLE[i][0], COLOR_SAMPLE[i][1], COLOR_SAMPLE[i][2])
       if(colorDis < 50 && colorDis < best_colDis){
           best_colDis = colorDis;
           //leastDis = COLOR_SAMPLE[i][3];
       }
   }
}


void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(PHOTOIN, INPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  pinMode(32, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(34, OUTPUT);
}
int counter = 0;
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int r = takeReading(RED, redResponseTime);
    int g = takeReading(GREEN, greenResponseTime);
    int b = takeReading(BLUE, blueResponseTime);

    //discrimination function
    discrim(r, g, b);

    // Print the values to the Serial Monitor
    Serial.print("R: ");
    Serial.print(r);
    Serial.print("\tG: ");
    Serial.print(g);
    Serial.print("\tB: ");
    Serial.println(b);

    //incremate counter
    counter++;
  }
}


int takeReading(int ledPin, int response_time) {
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(response_time); // wait for the LED to reach 90% of its brightness
  int reading = 0;
  for (int i = 0; i < 4; i++) {
    reading += analogRead(PHOTOIN); // summing up 4 readings
    delay(1); // wait a little bit between readings to give accurate results
  }
  digitalWrite(ledPin, LOW);
  delay(1); // ensure the LED is off for at least 1ms
  return reading / 4; // return the average reading
}
