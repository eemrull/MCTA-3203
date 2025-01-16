#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>
#include "DHT.h"
#include <Pixy.h>

// Define pins
#define BUTTON_PIN 25
#define POTENTIOMETER_PIN 34
const int door = 4;      // Pin for the door sensor
const int trigPin = 32;  // Ultrasonic Trigger Pin
const int echoPin = 33;  // Ultrasonic Echo Pin
const int buzzer = 26;
#define DHT_PIN 26
#define DHTTYPE DHT11

// OLED Initialization
#define OLED1_WIDTH 128
#define OLED1_HEIGHT 32
#define OLED2_WIDTH 128
#define OLED2_HEIGHT 64
#define OLED_RESET -1

DHT dht(DHT_PIN, DHTTYPE);
Pixy pixy;
// Color signatures
#define GREEN 1
#define PINK 2
#define BLUE 3
#define ORANGE 4
#define RED 5
#define MAX_CLOTHES 10

// Clothes detection
int clothesCount = 0;

Adafruit_SSD1306 oled(OLED1_WIDTH, OLED1_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 oled2(OLED2_WIDTH, OLED2_HEIGHT, &Wire, OLED_RESET);

// Define ESP-NOW settings
uint8_t receiverAddress[] = {0xE0, 0x5A, 0x1B, 0xCB, 0x62, 0x28};
uint8_t esp3[] = {0xC0, 0x5D, 0x89, 0xDC, 0xA5, 0x00};


// Data structure for transmission
#pragma pack(push, 1)
typedef struct {
  char temperature[8];
  char time[8];
  char function[8];
  int timeRemaining;
  int clothesCount;
  float detergentVolume;
  float tempsensor;
} UserData;
#pragma pack(pop)


UserData userData;

// Variables for selection
const char *temperatureOptions[] = {"20C", "40C", "60C", "90C"};
const char *timeOptions[] = {"0.1m", "15m", "30m", "45m"};
const char *functionOptions[] = {"Wash", "Rinse", "Spin"};

int step = 0;
int selectedIndices[] = {0, 0, 0};
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

int parseTime(const char *timeStr) {
  int timeValue = atoi(timeStr);
  char unit = timeStr[strlen(timeStr) - 1];

  if (unit == 'm') {
    return timeValue * 60;
  }
  return 0;
}

bool buttonState = HIGH;
bool lastButtonState = HIGH;

// Callback for sending data
void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    // Send full userData to receiverAddress
    // esp_err_t sendStatus1 = esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
    // if (sendStatus1 != ESP_OK) {
    //     Serial.print("Failed to send data to receiverAddress, error: ");
    //     Serial.println(sendStatus1);
    // }

    // Send only timeRemaining and function to esp3
    esp_err_t sendStatus2 = esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
    if (sendStatus2 != ESP_OK) {
        Serial.println("Send Failed ");
        // Serial.println(sendStatus2);
    }
    else{
      Serial.println("Send Success ");
    }
}


void setup() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED1 initialization failed!"));
    while (1);
  }

  if (!oled2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("OLED2 initialization failed!"));
    while (1);
  }

  oled.display();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled2.clearDisplay();  // Clear the screen to check for correct functionality
  oled2.display();
  oled2.setTextSize(1);
  oled2.setTextColor(WHITE);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(door, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo1 = {};
  memcpy(peerInfo1.peer_addr, receiverAddress, 6);
  peerInfo1.channel = 0;
  peerInfo1.encrypt = false;
  esp_now_add_peer(&peerInfo1);

  // esp_now_peer_info_t peerInfo2 = {};
  // memcpy(peerInfo2.peer_addr, esp3, 6);
  // peerInfo2.channel = 0;
  // peerInfo2.encrypt = false;
  // esp_now_add_peer(&peerInfo2);


  oled2.setTextSize(2);
  oled2.setCursor(0, 0);
  oled2.print("Select");
  oled2.setCursor(0, 20);
  oled2.print("Temp:");
  oled2.display();
  Serial.begin(115200);
}

void loop() {
  int doorState = digitalRead(door);
  // Serial.print("Door State: ");
  // Serial.println(doorState);  // Debug the state of the door

  if (doorState == HIGH) {
    // Door is open, show message to close it
    oled2.clearDisplay();
    oled2.setTextSize(2);
    oled2.setCursor(0, 0);
    oled2.print("Close the");
    oled2.setCursor(0, 20);
    oled2.print("door first");
    oled2.display();
    oled.clearDisplay();
    oled.display();
    return;  // Exit the loop and do nothing further if the door is open
  } else {
    // Door is closed, reset and display options

    // Proceed with button handling and potentiometer
    int reading = digitalRead(BUTTON_PIN);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading == LOW && buttonState == HIGH) {
        buttonPressed();
      }
      buttonState = reading;
    }
    lastButtonState = reading;

    if (step < 3) {
      int potValue = analogRead(POTENTIOMETER_PIN);
      int maxIndex = step == 0 ? 3 : step == 1 ? 3 : 2;
      selectedIndices[step] = constrain(map(potValue, 0, 4095, 0, maxIndex), 0, maxIndex);

      oled2.setCursor(0, 10);
      oled2.print("                ");
      oled2.setCursor(0, 10);
      displayOptionsOnOLED(); // Update OLED1 with the current selection
    }
  }
    detectClothes(); // Call to detect clothes
}

void detectClothes() {
  int blocks = pixy.getBlocks();

  if (blocks) {
    for (int i = 0; i < blocks; i++) {
      int signature = pixy.blocks[i].signature;

      if ((signature == GREEN) || (signature == PINK) || (signature == BLUE) ||
          (signature == ORANGE) || (signature == RED)) {
        clothesCount++;
      }
    }
  }
}

void buttonPressed() {
  step++;
  oled2.clearDisplay();
  displayOptionsOnOLED();

  if (step == 1) {
    oled2.setTextSize(2);
    oled2.setCursor(0, 0);
    oled2.print("Select");
    oled2.setCursor(0, 20);
    oled2.print("Time:");
    oled2.display();
  } else if (step == 2) {
    oled2.setTextSize(2);
    oled2.setCursor(0, 0);
    oled2.print("Select");
    oled2.setCursor(0, 20);
    oled2.print("Function:");
    oled2.display();
  } else if (step == 3) {
    // Summary display
    oled2.clearDisplay();
    oled2.setTextSize(2);
    oled2.setCursor(0, 0);
    String message = "Summary:";
    oled2.print(message);
    oled2.setTextSize(1);
    oled2.setCursor(0, 20);
    oled2.print(temperatureOptions[selectedIndices[0]]);
    oled2.print(", ");
    oled2.print(timeOptions[selectedIndices[1]]);
    oled2.print(", ");
    oled2.print(functionOptions[selectedIndices[2]]);
    oled2.display();

    oled2.setTextSize(1);
    oled2.setCursor(0, 30);
    oled2.print("Detergent: ");
    float detergentVolume = calculateDetergentVolume();
    // Serial.print("Calculated Detergent Volume: ");
    // Serial.println(detergentVolume);  // Debug output
    int detergentVolumeInt = (int)detergentVolume;  // Convert to int for display
    oled2.print(detergentVolumeInt);
    oled2.print("%");
    oled2.display();

    delay(500);  // Added delay to avoid flicker during transition
    displayWaitMessageOnOLED();
    delay(2500);

    // Send the data via ESP-NOW
    strcpy(userData.temperature, temperatureOptions[selectedIndices[0]]);
    strcpy(userData.time, timeOptions[selectedIndices[1]]);
    strcpy(userData.function, functionOptions[selectedIndices[2]]);
    userData.timeRemaining = parseTime(userData.time);
    userData.detergentVolume = detergentVolumeInt;
    userData.clothesCount = clothesCount;
    float tempsen = dht.readTemperature();
    userData.tempsensor = tempsen;

    // Start the countdown
    startCountdown(userData.timeRemaining);
    esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
    // Reset after completion
    resetSystem();
  }
}


void resetSystem() {
  // Reset the step and the selected indices for fresh new selection
  step = 0;
  memset(selectedIndices, 0, sizeof(selectedIndices));

  oled2.clearDisplay();
  String message = "Select Temp:";
  int messageLength = message.length();
  int centerPosition = (20 - messageLength) / 2; // Adjust for 20 characters
  oled2.setTextSize(2);
  oled2.setCursor(0, 0);
  oled2.print(message);
  oled2.display();
}

void displayOptionsOnOLED() {
  oled.clearDisplay();
  oled.setTextSize(3);  // Set the text size to 2.5 (approximated as 3)
  oled.setTextColor(WHITE);

  String message = (step == 0) ? temperatureOptions[selectedIndices[0]]
                                : (step == 1) ? timeOptions[selectedIndices[1]]
                                              : functionOptions[selectedIndices[2]];

  int textWidth = message.length() * 15;  // Estimate text width (15px per character)
  int x = (OLED1_WIDTH - textWidth) / 2;
  int y = (OLED1_HEIGHT - 8 * 3) / 2;  // Adjust vertical position based on text height
  y = y + 4; 
  oled.setCursor(0, 10);
  oled.print(message);
  oled.display();
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) * 0.0344;  // Calculate distance in cm
  return distance;
}

float calculateDetergentVolume() {
  long currentHeight = getDistance();  // Get current height from ultrasonic sensor
  const float initialHeight = 9;  // Initial height (for simplicity)
  
  // Detergent volume formula
  float detergentVolume = (initialHeight - currentHeight) / initialHeight * 100;

  // Ensure the detergent volume is never negative
  detergentVolume = max(0.0f, detergentVolume);

  return detergentVolume;
}

void displayWaitMessageOnOLED() {
  oled.clearDisplay();
  oled.setTextSize(3);  // Increase text size for better visibility
  oled.setTextColor(WHITE);

  String message = "Wait...";
  int textWidth = message.length() * 15;  // Estimate text width (15px per character)
  int x = (OLED1_WIDTH - textWidth) / 2;
  int y = (OLED1_HEIGHT - 8 * 3) / 2;  // Adjust vertical position based on text height
  y = y + 4; 
  oled.setCursor(0, 10);
  oled.print(message);
  oled.display();
}

void startCountdown(int totalSeconds) {
  while (totalSeconds >= 0) {
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(0, 10);
    oled.print(totalSeconds / 60);
    oled.print("m ");
    oled.print(totalSeconds % 60);
    oled.print("s");
    oled.display();
    delay(1000);
    totalSeconds--;

    float detergentVolume = calculateDetergentVolume();
    userData.detergentVolume = detergentVolume;
    userData.timeRemaining = totalSeconds;
    oled2.clearDisplay();
    oled2.setTextSize(2);
    oled2.setCursor(0, 0);
    String message = "Summary:";
    oled2.print(message);
    oled2.setTextSize(1);
    oled2.setCursor(0, 20);
    oled2.print(temperatureOptions[selectedIndices[0]]);
    oled2.print(", ");
    oled2.print(timeOptions[selectedIndices[1]]);
    oled2.print(", ");
    oled2.print(functionOptions[selectedIndices[2]]);
    oled2.display();
    oled2.setCursor(0, 30);
    oled2.setTextSize(1);
    oled2.print("Detergent: ");
    oled2.print(detergentVolume);
    oled2.print("%");
    oled2.display();
    esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
  }
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Complete!");
  oled.display();
  digitalWrite(buzzer, HIGH); // Turn the buzzer ON
  delay(1500);                  // Wait for 1 second
  digitalWrite(buzzer, LOW);  // Turn the buzzer OFF
  delay(3000);
}

