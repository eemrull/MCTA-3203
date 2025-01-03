#define STEP_PIN 14  // Step pin connected to ESP32
#define DIR_PIN 15   // Direction pin connected to ESP32
#define EN_PIN 13    // Enable pin connected to ESP32

int speedDelay = 1000; // Initial delay between steps in microseconds

// Test modes and durations
int selectedMode = 1;   // Change this for testing (1 = Wash, 2 = Rinse, 3 = Spin)
int testDuration = 3;   // Duration in minutes (set to 3 minutes for testing)

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Stepper Motor Washing Machine Test");

  // Initialize pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  digitalWrite(EN_PIN, LOW); // Enable the motor driver
}

void loop() {
  Serial.print("Selected Mode: ");
  Serial.println(selectedMode);
  Serial.print("Test Duration (minutes): ");
  Serial.println(testDuration);

  // Select the mode based on the test variable
  switch (selectedMode) {
    case 1: // Wash Mode
      Serial.println("Starting Wash Mode");
      washMode(testDuration * 60); // Convert minutes to seconds
      break;

    case 2: // Rinse Mode
      Serial.println("Starting Rinse Mode");
      rinseMode(testDuration * 60); // Convert minutes to seconds
      break;

    case 3: // Spin Mode
      Serial.println("Starting Spin Mode");
      spinMode(testDuration * 60); // Convert minutes to seconds
      break;

    default:
      Serial.println("Invalid Mode Selected");
      break;
  }

  // Pause for a moment before restarting the loop
  Serial.println("Cycle Complete. Restarting...");
  delay(5000);
}

void washMode(int durationSeconds) {
  unsigned long startTime = millis();
  while ((millis() - startTime) < (durationSeconds * 1000)) {
    Serial.println("Wash Mode: Agitating...");
    // Rotate in one direction
    digitalWrite(DIR_PIN, HIGH);
    rotateSteps(200); // Rotate 200 steps
    delay(500);       // Pause for agitation effect

    // Rotate in the opposite direction
    digitalWrite(DIR_PIN, LOW);
    rotateSteps(200);
    delay(500);

    // Debug remaining time
    unsigned long elapsedTime = (millis() - startTime) / 1000; // Convert to seconds
    unsigned long remainingTime = durationSeconds - elapsedTime;
    Serial.print("Elapsed Time (s): ");
    Serial.println(elapsedTime);
    Serial.print("Remaining Time (s): ");
    Serial.println(remainingTime);
  }
  Serial.println("Wash Mode Complete");
}

void rinseMode(int durationSeconds) {
  unsigned long startTime = millis();
  while ((millis() - startTime) < (durationSeconds * 1000)) {
    Serial.println("Rinse Mode: Rotating...");
    // Rotate continuously at moderate speed
    digitalWrite(DIR_PIN, HIGH);
    rotateSteps(400);
    delay(1000); // Pause for a moment

    // Debug remaining time
    unsigned long elapsedTime = (millis() - startTime) / 1000; // Convert to seconds
    unsigned long remainingTime = durationSeconds - elapsedTime;
    Serial.print("Elapsed Time (s): ");
    Serial.println(elapsedTime);
    Serial.print("Remaining Time (s): ");
    Serial.println(remainingTime);
  }
  Serial.println("Rinse Mode Complete");
}

void spinMode(int durationSeconds) {
  unsigned long startTime = millis();
  int maxSpeedDelay = 200; // Minimum delay for max speed
  while ((millis() - startTime) < (durationSeconds * 1000)) {
    Serial.println("Spin Mode: Ramping Up...");
    // Ramp up speed
    for (int delayTime = 1000; delayTime > maxSpeedDelay; delayTime -= 10) {
      speedDelay = delayTime;
      rotateSteps(10);
    }
    Serial.println("Spin Mode: Ramping Down...");
    // Ramp down speed
    for (int delayTime = maxSpeedDelay; delayTime < 1000; delayTime += 10) {
      speedDelay = delayTime;
      rotateSteps(10);
    }

    // Debug remaining time
    unsigned long elapsedTime = (millis() - startTime) / 1000; // Convert to seconds
    unsigned long remainingTime = durationSeconds - elapsedTime;
    Serial.print("Elapsed Time (s): ");
    Serial.println(elapsedTime);
    Serial.print("Remaining Time (s): ");
    Serial.println(remainingTime);
  }
  Serial.println("Spin Mode Complete");
}

void rotateSteps(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speedDelay);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(speedDelay);
  }
}
