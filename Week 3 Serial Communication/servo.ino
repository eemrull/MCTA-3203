#include <Servo.h>

Servo myservo;  // Create Servo object to control a servo
int potPin = A0; // Pin connected to the potentiometer
int pos = 0;     // Variable to store the servo position
char incomingByte; // Variable for incoming serial data

void setup() {
  myservo.attach(5);  // Attaches the servo on pin 5 to the Servo object
  Serial.begin(9600);
}

void loop() {
  // Check for incoming serial data to stop execution
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'q') { // If 'q' is received, stop execution
      while (true); // Infinite loop to halt the execution
    }
  }

  // Read the potentiometer value (0-1023)
  int potValue = analogRead(potPin);
  // Map the potentiometer value to servo angle (0-180)
  pos = map(potValue, 0, 1023, 0, 180);
  myservo.write(pos); // Set the servo position
  Serial.println(pos); // Print the current position

  delay(100); // Small delay for stability
}
