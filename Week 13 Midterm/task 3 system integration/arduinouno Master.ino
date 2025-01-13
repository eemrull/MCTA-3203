#include <Wire.h>

void setup() {
  Wire.begin(); // Start I²C as master
  Serial.begin(9600); // For debugging
}

void loop() {
  Wire.beginTransmission(8); // Address of Mega
  Wire.write("Start Task");  // Send synchronization command
  Wire.endTransmission();
  delay(5000); // Repeat every 5 seconds
}
