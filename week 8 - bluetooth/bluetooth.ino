#include <SoftwareSerial.h>

const int analogPin = A0;        // Analog pin connected to LM35
SoftwareSerial BTSerial(10, 11);   // RX, TX for HC-05

void setup() {
  Serial.begin(9600);           // For debugging via Serial Monitor
  BTSerial.begin(9600);         // Initialize HC-05 Bluetooth module
  BTSerial.println("Bluetooth Connected!"); // Send a welcome message
}

void loop() {
  int sensorValue = analogRead(analogPin);    // Read LM35 output
  double voltage = sensorValue * (5.0 / 1023.0); // Convert to voltage
  double temperature = voltage * 100.0;      // Convert to temperature (°C)

  // Send temperature to Bluetooth
  BTSerial.print("Temperature: ");
  BTSerial.print(temperature);
  BTSerial.println(" °C");

  delay(1000); // Delay for 1 second
}
