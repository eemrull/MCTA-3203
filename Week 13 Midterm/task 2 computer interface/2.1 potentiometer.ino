void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
}

void loop() {
  int potValue = analogRead(A0);  // Read the potentiometer value (0–1023)
  Serial.println(potValue);       // Send the value to the serial port
  delay(100);                     // Wait 100 milliseconds before sending the next value
}
