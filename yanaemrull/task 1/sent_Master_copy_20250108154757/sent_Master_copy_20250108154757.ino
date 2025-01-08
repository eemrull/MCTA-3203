#include <Wire.h>

void setup() {
  Wire.begin(); // Initialize as I2C Master
  Serial.begin(9600); // Start Serial Monitor communication
  Serial.println("Enter a command: LED ON or LED OFF");
}

void loop() {
  // Check if there's input from the Serial Monitor
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // Read user input
    command.trim(); // Remove any leading/trailing whitespace

    // Send the command to the Slave
    Wire.beginTransmission(8); // Slave address is 8
    Wire.write(command.c_str()); // Send the command as a string
    Wire.endTransmission();

    // Display the command sent for debugging
    Serial.println("Command Sent: " + command);
    delay(500); // Small delay to avoid spamming
  }
}