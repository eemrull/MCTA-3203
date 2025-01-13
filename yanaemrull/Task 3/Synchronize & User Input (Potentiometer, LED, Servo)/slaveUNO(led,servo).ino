#include <Wire.h>
#include <Servo.h>

const int ledPin = 13;  // Pin where the LED is connected
Servo myServo;  // Servo object
int potentiometerValue = 0;  // Value received from the master

void setup() {
  pinMode(ledPin, OUTPUT);  // Set LED pin as output
  myServo.attach(9);  // Attach the servo to pin 9
  Wire.begin(8);  // Start I2C as slave with address 8
  Wire.onReceive(receiveData);  // Set up the function to handle incoming data
  Serial.begin(9600);  // Start serial communication for monitoring
}

void loop() {
  // The slave will constantly check for I2C data, no need to do anything else in the loop
  
  // If serial input is received, perform a sweep
  if (Serial.available() > 0) {
    char input = Serial.read();  // Read the input character
    if (input) {
      digitalWrite(ledPin, HIGH);  // Turn on the LED
      sweepServo();  // Perform the servo sweep
      digitalWrite(ledPin, LOW);  // Turn off the LED after the sweep
    }
  }

  // Move the servo according to the received potentiometer value
  myServo.write(potentiometerValue);
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    potentiometerValue = Wire.read();  // Read the potentiometer value sent by the master
  }
}

void sweepServo() {
  for (int pos = 0; pos <= 180; pos++) {  // Sweep from 0 to 180
    myServo.write(pos);
    delay(15);  // Wait for the servo to reach the position
  }
  for (int pos = 180; pos >= 0; pos--) {  // Sweep back from 180 to 0
    myServo.write(pos);
    delay(15);  // Wait for the servo to reach the position
  }
}
