#include <QTRSensors.h>

#define SensorCount 8
#define BUTTON 3

QTRSensors qtr;
uint16_t sensorValues[SensorCount];
const uint8_t QTRPins[SensorCount] = {13, 12, 11, 10, 9, 8, 7, 6};

// PID variables
float Kp = 0.05; // Proportional coefficient
float Ki = 0.00001; // Integral coefficient
float Kd = 0.8; // Derivative coefficient
int lastError = 0;
int integral = 0;

// LED and Motor control pins
const uint8_t LEDPins[SensorCount] = {30, 31, 32, 33, 34, 35, 36, 37};
#define LEFTPWM 5
#define LEFTA 42
#define LEFTB 43
#define RIGHTPWM 6
#define RIGHTA 46
#define RIGHTB 47

void setup() {
    Serial.begin(9600);
    qtr.setTypeRC();
    qtr.setSensorPins(QTRPins, SensorCount);
    qtr.setEmitterPin(5);

    // Initialize LED and motor control pins
    for (uint8_t i = 0; i < SensorCount; i++) {
        pinMode(LEDPins[i], OUTPUT);
        digitalWrite(LEDPins[i], LOW);
    }
    pinMode(LEFTPWM, OUTPUT);
    pinMode(LEFTA, OUTPUT);
    pinMode(LEFTB, OUTPUT);
    pinMode(RIGHTPWM, OUTPUT);
    pinMode(RIGHTA, OUTPUT);
    pinMode(RIGHTB, OUTPUT);

    pinMode(BUTTON, INPUT_PULLUP);

    // Calibrate sensors
    for (uint16_t i = 0; i < 400; i++) {
        qtr.calibrate();
        for (uint8_t j = 0; j < SensorCount; j++) {
            digitalWrite(LEDPins[j], HIGH);
        }
        delay(25);
    }
    for (uint8_t j = 0; j < SensorCount; j++) {
        digitalWrite(LEDPins[j], LOW);
    }

    Serial.println("Press the button to start");
    while(digitalRead(BUTTON) == HIGH);
    Serial.println("Starting...");
}

void loop() {
    uint16_t position = qtr.readLineBlack(sensorValues);
    updateLEDs(position); // Update LED status based on line position
    PID_control(position); // Adjust motors based on PID calculation
    delay(250);
}

void updateLEDs(unsigned int position) {
    // Set all LEDs high
    for (uint8_t i = 0; i < SensorCount; i++) {
        digitalWrite(LEDPins[i], HIGH);
    }
    // Turn off LEDs based on position
    if (position < 1000) { digitalWrite(30, LOW); }
    if (position > 0 && position < 2000) { digitalWrite(31, LOW); }
    if (position > 1000 && position < 3000) { digitalWrite(32, LOW); }
    if (position > 2000 && position < 4000) { digitalWrite(33, LOW); }
    if (position > 3000 && position < 5000) { digitalWrite(34, LOW); }
    if (position > 4000 && position < 6000) { digitalWrite(35, LOW); }
    if (position > 5000 && position < 7000) { digitalWrite(36, LOW); }
    if (position > 6000) { digitalWrite(37, LOW); }
}

void PID_control(unsigned int position) {
    int error = 3500 - position; // Center is 3500 for 0-7000 scale
    int P = error;
    integral = integral + error;
    int D = error - lastError;
    lastError = error;
    int motorSpeedChange = P * Kp + integral * Ki + D * Kd;
    int motorSpeedA = 100 + motorSpeedChange; // base speed + change
    int motorSpeedB = 100 - motorSpeedChange;

    motorSpeedA = constrain(motorSpeedA, -75, 125); // Constrain to max/min speeds
    motorSpeedB = constrain(motorSpeedB, -75, 125);

    forward_movement(motorSpeedA, motorSpeedB); // Adjust motors
}

void forward_movement(int speedA, int speedB) {
    if (speedA < 0) {
        speedA = -speedA; // Make speed positive
        digitalWrite(LEFTA, LOW);
    } else {
        digitalWrite(LEFTA, HIGH);
    }
    if (speedB < 0) {
        speedB = -speedB; //Make speed positive for reverse movement, and adjust digital pins accordingly to change motor phase. The `analogWrite` functions then set the corrected speeds to control forward and backward movement. This method provides dynamic adjustment based on line position, enabling precise steering for the line-following task.
    }
}