#include <SoftwareSerial.h>

// Set up SoftwareSerial for HC-05
SoftwareSerial BTSerial(10, 11); // RX, TX for HC-05 (Arduino Pin 3 to RX, Pin 2 to TX)

void setup() {
  // Start both serial communications
  Serial.begin(9600);           // Communication with PC via USB
  BTSerial.begin(9600);         // Communication with HC-05
  
  // Print welcome message on the Serial Monitor and Bluetooth
  Serial.println("Type a message to send to your phone via Bluetooth.");
  BTSerial.println("Bluetooth connected! Ready to receive data.");
}

void loop() {
  // Check for data from Serial Monitor (Laptop)
  if (Serial.available()) {
    char data = Serial.read();         // Read the data from the Serial Monitor
    BTSerial.write(data);              // Send the data to the HC-05
    Serial.print("Sent to Phone: ");   // Show what was sent
    Serial.println(data);
  }

  // Check for data from the phone via Bluetooth
  if (BTSerial.available()) {
    char data = BTSerial.read();       // Read the data from the phone
    Serial.print("Received from Phone: "); // Show what was received
    Serial.println(data);
  }
}
