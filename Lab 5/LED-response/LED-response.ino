#define RED 44
#define BLUE 46
#define GREEN 42
#define TIMEINTERVAL 10
#define PHOTOIN A0

int i;

int reading[100];



void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  digitalWrite(BLUE, LOW);
  analogReference(DEFAULT);

  pinMode(PHOTOIN, INPUT);
  reading[0] = analogRead(PHOTOIN);
/*
  // Test for Blue LED
  for (i = 0; i < 100; i++) {
    digitalWrite(BLUE, HIGH);
    delayMicroseconds((unsigned long)(i * TIMEINTERVAL));
    reading[i] = analogRead(PHOTOIN);
    digitalWrite(BLUE, LOW);
    delay(100);
  }
  for (i = 0; i < 100; i++) {
    Serial.print("Blue\t");
    Serial.print(i);
    Serial.print("\t");
    Serial.println(reading[i]);
  }*/

  // Test for Red LED
  for (i = 0; i < 100; i++) {
    digitalWrite(RED, HIGH);
    delayMicroseconds((unsigned long)(i * TIMEINTERVAL));
    reading[i] = analogRead(PHOTOIN);
    digitalWrite(RED, LOW);
    delay(100);
  }
  for (i = 0; i < 100; i++) {
    Serial.print("Red\t");
    Serial.print(i);
    Serial.print("\t");
    Serial.println(reading[i]);
  }
/*
  // Test for Green LED
  for (i = 0; i < 100; i++) {
    digitalWrite(GREEN, HIGH);
    delayMicroseconds((unsigned long)(i * TIMEINTERVAL));
    reading[i] = analogRead(PHOTOIN);
    digitalWrite(GREEN, LOW);
    delay(100);
  }
  for (i = 0; i < 100; i++) {
    Serial.print("Green\t");
    Serial.print(i);
    Serial.print("\t");
    Serial.println(reading[i]);
  }*/
}

void loop() {

}
