#include <Servo.h>

Servo servo;
int servoPosition = 90;  // Initial servo position
const int greenLedPin = 6; // Pin for green LED
const int redLedPin = 7;   // Pin for red LED

void setup() {
  servo.attach(5);  // Attach the servo to pin 5
  servo.write(servoPosition);
  
  // Set LED pins as outputs
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Listen for signals from Python
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read command as string
    Serial.print("Received command: "); // Debugging line
    Serial.println(command); // Print received command
    
    if (command.startsWith("A")) {
      // Allow servo control
      int angle = command.substring(1).toInt(); // Extract angle
      servoPosition = angle;  // Set the servo to the specified angle
      servo.write(servoPosition);
      
      // Indicate access granted
      digitalWrite(greenLedPin, HIGH);  // Turn on green LED
      digitalWrite(redLedPin, LOW);      // Turn off red LED
    } else if (command == "D") {
      // Disallow servo control
      servoPosition = 90;  // Set the servo back to the default angle
      servo.write(servoPosition);
      
      // Indicate access denied
      digitalWrite(greenLedPin, LOW);   // Turn off green LED
      digitalWrite(redLedPin, HIGH);     // Turn on red LED
    }
  }
}
