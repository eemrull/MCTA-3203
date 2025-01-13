#include <Wire.h>

void setup() {
  Wire.begin(8); // Join I²C as slave with address 8
  Wire.onReceive(receiveEvent); // Register function to handle incoming data
  Serial.begin(9600); // For debugging
}

void loop() {
  // Slave stays idle until it receives a command
}

void receiveEvent(int bytes) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c); // Display received command
  }
  Serial.println();
  // Perform synchronized actions here
}
