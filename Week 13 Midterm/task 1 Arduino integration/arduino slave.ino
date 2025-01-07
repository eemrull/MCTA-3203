#include <Wire.h>

void setup() {
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register receive event
  Serial.begin(9600);
}

void loop() {
  // Main loop does nothing
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}
