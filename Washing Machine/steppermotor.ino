#define STEP_PIN 14  // Step pin connected to ESP32
#define DIR_PIN 15   // Direction pin connected to ESP32
#define EN_PIN 13    // Enable pin connected to ESP32

int speedDelay = 1000; // Initial delay between steps in microseconds

// Structure to hold received data from master
typedef struct {
  char function[8];
  int timeRemaining;
} ESP3Data;

ESP3Data receivedData;

// Callback function for receiving data from master
void onDataReceive(const uint8_t *data, int len) {
    if (len == sizeof(ESP3Data)) {
        memcpy(&receivedData, data, len);

        // Print the received data in human-readable format
        Serial.print("Received Function: ");
        Serial.println(receivedData.function);
        Serial.print("Time Remaining: ");
        Serial.println(receivedData.timeRemaining);

        // Execute motor functions based on the received data
        if (strcmp(receivedData.function, "Wash") == 0) {
            washMode(receivedData.timeRemaining); // Convert time to seconds
        } else if (strcmp(receivedData.function, "Rinse") == 0) {
            rinseMode(receivedData.timeRemaining); // Convert time to seconds
        } else if (strcmp(receivedData.function, "Spin") == 0) {
            spinMode(receivedData.timeRemaining); // Convert time to seconds
        } else {
            Serial.println("Invalid Function");
        }
    }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP 3 Stepper Motor Control");

  // Initialize pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW); // Enable the motor driver

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // No need for connection

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
    // Nothing to do here
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
