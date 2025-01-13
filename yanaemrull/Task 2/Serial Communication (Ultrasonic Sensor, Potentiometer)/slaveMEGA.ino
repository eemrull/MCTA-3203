#include <Wire.h>

long ultrasonicDistance = 0;
int potentiometerValue = 0;

void setup() {
  Wire.begin(8);               // Initialize I2C with address 8
  Wire.onReceive(receiveData); // Register receive event
  Serial.begin(9600);          // For sending data to the computer
}

void loop() {
  // Send data to the computer
  Serial.print("Ultrasonic: ");
  Serial.print(ultrasonicDistance);
  Serial.print(" cm, Potentiometer: ");
  Serial.println(potentiometerValue);

  delay(500); // Delay for readability
}

// Function to handle received I2C data
void receiveData(int howMany) {
  if (howMany >= sizeof(long)) {
    Wire.readBytes((byte*)&ultrasonicDistance, sizeof(long));
  }
  if (howMany >= sizeof(long) + sizeof(int)) {
    Wire.readBytes((byte*)&potentiometerValue, sizeof(int));
  }
}
