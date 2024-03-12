
// Left Motor
#define enA 3
#define in1 22
#define in2 24

// Right Motor
#define enB 2
#define in3 23
#define in4 25

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println("Right Motor moving...");
  foward(0, 255);
  delay(5000);

  Serial.println("Left Motor moving...");
  foward(255, 0);
  delay(5000);
}
void foward(int a, int b){
  //technically the reverse direction but works for foward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, a);
  analogWrite(enB, b);
}
