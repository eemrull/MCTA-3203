#include <Servo.h>

// Create servo object
Servo myservo;

// Define pins for the ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Define pin for the LM35 temperature sensor
const int tempPin = A0;

// Define pin for the LED
const int ledPin = 13;

// Define variables for servo and sensor readings
int pos = 0; // Variable to store the servo position

// Define thresholds
const float tempThreshold = 30.0; // Temperature threshold in Celsius
const float distanceThreshold = 10.0; // Distance threshold in cm

void setup() {
  // Attach the servo on pin 6
  myservo.attach(6);

  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set up LED pin as output
  pinMode(ledPin, OUTPUT);

  // Initialize Serial Monitor
  Serial.begin(9600);

  Serial.println("Starting sensor-controlled servo and LED...");
}

void loop() {
  // Ultrasonic sensor reading
  long duration;
  float distance;

  // Send a pulse to the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Convert duration to distance in centimeters
  distance = duration * 0.034 / 2;

  // Temperature sensor reading
  int tempReading = analogRead(tempPin);
  float voltage = tempReading * (5.0 / 1023.0); // Convert analog value to voltage
  float temperature = voltage * 100.0; // Convert voltage to Celsius (LM35: 10mV/°C)

  // Control servo based on sensor readings
  if (distance > 0 && distance <= 180) {
    pos = map(distance, 0, 180, 0, 180); // Map distance to servo angle
  } else if (temperature > 25.0) { // Example condition for temperature
    pos = map(temperature, 25, 50, 0, 180); // Map temperature to servo angle (adjust limits as needed)
  } else {
    pos = 0; // Default position
  }

  myservo.write(pos); // Move servo to the calculated position
  delay(15); // Wait for the servo to reach the position

  // Control LED based on thresholds
  if (distance <= distanceThreshold || temperature > tempThreshold) {
    digitalWrite(ledPin, HIGH); // Turn LED on
  } else {
    digitalWrite(ledPin, LOW); // Turn LED off
  }

  // Print sensor data and servo position
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Servo Angle: ");
  Serial.println(pos);

  Serial.print("LED State: ");
  Serial.println(digitalRead(ledPin) == HIGH ? "ON" : "OFF");

  Serial.println();
  delay(500); // Delay for next loop iteration
}
