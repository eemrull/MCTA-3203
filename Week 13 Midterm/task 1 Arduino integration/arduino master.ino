#include <Wire.h>

void setup() {
  Wire.begin(); // Join I2C bus as master
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(8); // Address of the slave
  Wire.write("Hello Slave"); // Send message
  Wire.endTransmission();
  delay(500);
}
