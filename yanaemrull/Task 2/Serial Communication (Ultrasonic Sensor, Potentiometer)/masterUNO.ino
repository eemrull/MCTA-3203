#include <Wire.h>
#define ULTRASONIC_TRIG 7
#define ULTRASONIC_ECHO 6
#define POTENTIOMETER A0

void setup() {
  Wire.begin();          // Initialize I2C as master
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  Serial.begin(9600);    // For debugging
}

void loop() {
  // Read ultrasonic sensor
  long duration, distance;
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  duration = pulseIn(ULTRASONIC_ECHO, HIGH);
  distance = duration * 0.034 / 2;

  // Read potentiometer
  int potValue = analogRead(POTENTIOMETER);

  // Send data to Mega
  Wire.beginTransmission(8); // Address of Mega
  Wire.write((byte*)&distance, sizeof(distance)); // Send ultrasonic distance
  Wire.write((byte*)&potValue, sizeof(potValue)); // Send potentiometer value
  Wire.endTransmission();

  delay(500); // Delay for stability
}
