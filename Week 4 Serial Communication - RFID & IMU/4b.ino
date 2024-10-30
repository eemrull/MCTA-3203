#include <Servo.h>
Servo servo;
int servoPosition = 90;  // Initial servo position
void setup() {
  servo.attach(9);  // Attach the servo to pin 9
  servo.write(servoPosition);
  Serial.begin(9600);
}
void loop() {
  // Listen for signals from Python
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'A') {
      // Allow servo control
      servoPosition = 180;  // Set the servo to a specific angle
      servo.write(servoPosition);
    } else if (command == 'D') {
      // Disallow servo control
      servoPosition = 90;  // Set the servo back to the default angle
      servo.write(servoPosition);
    }
  }
}