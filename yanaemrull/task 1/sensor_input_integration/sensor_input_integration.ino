// Define pins for the ultrasonic sensor
const int trigPin = 10;
const int echoPin = 9;

// Define pin for the LM35 temperature sensor
const int tempPin = A0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Set up ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Optional: Set up temperature pin as input
  pinMode(tempPin, INPUT);

  Serial.println("Sensor readings start...");
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

  // Print readings to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Wait for 1 second before next reading
  delay(1000);
}
