#include <Wire.h>

const int potentiometerPin = A0;  // Pin where the potentiometer is connected

void setup() {
  Wire.begin();  // Start I2C as master
  Serial.begin(9600);  // Start serial communication for monitoring
}

void loop() {
  int potentiometerValue = analogRead(potentiometerPin);  // Read potentiometer
  potentiometerValue = map(potentiometerValue, 0, 1023, 0, 180);  // Map to servo range (0-180)

  Wire.beginTransmission(8);  // Communicate with slave with address 8
  Wire.write(potentiometerValue);  // Send the potentiometer value
  Wire.endTransmission();  // End transmission

  delay(100);  // Small delay to avoid flooding the I2C bus
}
