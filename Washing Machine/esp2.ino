#include <Pixy.h>
#include "DHT.h"

// Stepper motor pins
#define STEP_PIN 14  // Step pin connected to ESP32
#define DIR_PIN 15   // Direction pin connected to ESP32
#define EN_PIN 13    // Enable pin connected to ESP32

// Stepper motor speed
int speedDelay = 1000; // Initial delay between steps in microseconds

// Modes and durations
int selectedMode = 3;   // Change this for testing (1 = Wash, 2 = Rinse, 3 = Spin)
int testDuration = 3;   // Duration in minutes (set to 3 minutes for testing)

// Pixy and DHT sensor
Pixy pixy;
#define DHTPIN 4        // GPIO pin for DHT (D4 on NodeMCU)
#define DHTTYPE DHT11   // DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

// Color signatures
#define GREEN 1
#define PINK 2
#define BLUE 3
#define ORANGE 4
#define RED 5
#define MAX_CLOTHES 10

// Clothes detection
int clothesCount = 0;

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);
  Serial.println("Washing Machine System Initialized");

  // Initialize stepper motor pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW); // Enable the motor driver

  // Initialize Pixy and DHT sensor
  dht.begin();
  pixy.init();
}

void loop() {
  // Detect clothes
  detectClothes();

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // If washer is full, run the selected mode
  if (clothesCount >= MAX_CLOTHES) {
    Serial.println("Starting selected washing mode...");
    runMode(selectedMode, testDuration);
    clothesCount = 0; // Reset clothes count after completion
  }

  delay(1000); // Short delay for stability
}

void detectClothes() {
  int blocks = pixy.getBlocks();

  if (blocks) {
    for (int i = 0; i < blocks; i++) {
      int signature = pixy.blocks[i].signature;

      if ((signature == GREEN || signature == RED || signature == BLUE || signature == ORANGE || signature == PINK) && clothesCount < MAX_CLOTHES) {
        clothesCount++;
      }

      Serial.print("Total clothes detected: ");
      Serial.println(clothesCount);

      if (clothesCount == MAX_CLOTHES) {
        Serial.println("Washer is full!");
        break;
      }
    }
  }
}

void runMode(int mode, int duration) {
  switch (mode) {
    case 1:
      Serial.println("Wash Mode Selected");
      washMode(duration * 60);
      break;
    case 2:
      Serial.println("Rinse Mode Selected");
      rinseMode(duration * 60);
      break;
    case 3:
      Serial.println("Spin Mode Selected");
      spinMode(duration * 60);
      break;
    default:
      Serial.println("Invalid Mode Selected");
      break;
  }
}

void washMode(int durationSeconds) {
  unsigned long startTime = millis();
  while ((millis() - startTime) < (durationSeconds * 1000)) {
    digitalWrite(DIR_PIN, HIGH);
    rotateSteps(200);
    delay(500);

    digitalWrite(DIR_PIN, LOW);
    rotateSteps(200);
    delay(500);
  }
  Serial.println("Wash Mode Complete");
}

void rinseMode(int durationSeconds) {
  unsigned long startTime = millis();
  while ((millis() - startTime) < (durationSeconds * 1000)) {
    digitalWrite(DIR_PIN, HIGH);
    rotateSteps(400);
    delay(1000);
  }
  Serial.println("Rinse Mode Complete");
}

void spinMode(int durationSeconds) {
  unsigned long startTime = millis();
  int maxSpeedDelay = 200;
  while ((millis() - startTime) < (durationSeconds * 1000)) {
    for (int delayTime = 1000; delayTime > maxSpeedDelay; delayTime -= 10) {
      speedDelay = delayTime;
      rotateSteps(10);
    }
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
