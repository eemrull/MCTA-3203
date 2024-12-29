#include <SoftwareSerial.h>

const int analogPin = A0;        // Analog pin connected to LM35
const int fanPin = 7;            // Pin connected to fan

SoftwareSerial BTSerial(10, 11); // RX, TX 

void setup() {
  pinMode(fanPin, OUTPUT);
  
  Serial.begin(9600);            // Debugging via Serial Monitor
  BTSerial.begin(9600);          // Bluetooth module initialization

  BTSerial.println("Bluetooth Connected!"); // Send a welcome message
}

void loop() {
  // Read temperature data
  int sensorValue = analogRead(analogPin);
  double voltage = sensorValue * (5.0 / 1023.0);
  double temperature = voltage * 100.0;

  // Send temperature data over Bluetooth
  BTSerial.print("Temperature: ");
  BTSerial.print(temperature);
  BTSerial.println(" °C");

  // Check if a command is received from the app
  if (BTSerial.available()) {
    String command = BTSerial.readStringUntil('\n');
    command.trim(); // Remove any extra spaces or newlines

    if (command == "FAN ON") {
      digitalWrite(fanPin, HIGH);
      BTSerial.println("Fan is ON");
    } else if (command == "FAN OFF") {
      digitalWrite(fanPin, LOW);
      BTSerial.println("Fan is OFF");
    }  else if(temperature >= 30) {
      digitalWrite(fanPin, HIGH);
    }  else if (temperature <= 29) {
      digitalWrite(fanPin, LOW);
    } else {
      BTSerial.println("Unknown Command");
    }
  }

  delay(1000); // Delay for 1 second
}
