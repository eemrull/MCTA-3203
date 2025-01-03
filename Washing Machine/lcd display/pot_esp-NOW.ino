#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <esp_now.h>
#include <WiFi.h>

// Define pins
#define BUTTON_PIN 25
#define POTENTIOMETER_PIN 34

// LCD Initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define ESP-NOW settings
uint8_t receiverAddress[] = {0xC0, 0x5D, 0x89, 0xDC, 0xA5, 0x00}; // Replace with ESP1 MAC address

// Data structure for transmission
typedef struct {
  char temperature[8];
  char time[8];
  char function[8];
  int timeRemaining;
} UserData;

UserData userData;

// Variables for selection
const char *temperatureOptions[] = {"20C", "40C", "60C", "90C"};
const char *timeOptions[] = {"15m", "30m", "45m", "60m"};
const char *functionOptions[] = {"Wash", "Rinse", "Spin"};

int step = 0;
int selectedIndices[] = {0, 0, 0};
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool buttonState = HIGH;
bool lastButtonState = HIGH;

// Callback for sending data
void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Send Success" : "Send Fail");
}

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_PIN, INPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  lcd.setCursor(0, 0);
  lcd.print("Select Temp:");
  lcd.setCursor(0, 1);
  lcd.print(temperatureOptions[selectedIndices[0]]);

  Serial.begin(115200);
}

void loop() {
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

    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear line
    lcd.setCursor(0, 1);
    lcd.print(step == 0 ? temperatureOptions[selectedIndices[0]]
                        : step == 1 ? timeOptions[selectedIndices[1]]
                                    : functionOptions[selectedIndices[2]]);
  }
}

void buttonPressed() {
  step++;
  lcd.clear();

  if (step == 1) {
    lcd.print("Select Time:");
    lcd.setCursor(0, 1);
    lcd.print(timeOptions[selectedIndices[1]]);
  } else if (step == 2) {
    lcd.print("Select Func:");
    lcd.setCursor(0, 1);
    lcd.print(functionOptions[selectedIndices[2]]);
  } else if (step == 3) {
    lcd.print("Summary:");
    lcd.setCursor(0, 1);
    lcd.print(temperatureOptions[selectedIndices[0]]);
    lcd.print(", ");
    lcd.print(timeOptions[selectedIndices[1]]);
    lcd.print(", ");
    lcd.print(functionOptions[selectedIndices[2]]);
    delay(3000);

    strcpy(userData.temperature, temperatureOptions[selectedIndices[0]]);
    strcpy(userData.time, timeOptions[selectedIndices[1]]);
    strcpy(userData.function, functionOptions[selectedIndices[2]]);
    userData.timeRemaining = parseTime(userData.time);

    esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));

    startCountdown(userData.timeRemaining);
    resetSystem();
  }
}

void startCountdown(int totalSeconds) {
  while (totalSeconds >= 0) {
    lcd.clear();
    lcd.print("Time Left:");
    lcd.setCursor(0, 1);
    lcd.print(totalSeconds / 60);
    lcd.print("m ");
    lcd.print(totalSeconds % 60);
    lcd.print("s");
    delay(1000);
    totalSeconds--;

    userData.timeRemaining = totalSeconds;
    esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
  }

  lcd.clear();
  lcd.print("Complete!");
  delay(3000);
}

int parseTime(const char *timeOption) {
  if (strcmp(timeOption, "15m") == 0) return 15 * 60;
  if (strcmp(timeOption, "30m") == 0) return 30 * 60;
  if (strcmp(timeOption, "45m") == 0) return 45 * 60;
  if (strcmp(timeOption, "60m") == 0) return 60 * 60;
  return 0;
}

void resetSystem() {
  step = 0;
  memset(selectedIndices, 0, sizeof(selectedIndices));

  lcd.clear();
  lcd.print("Select Temp:");
  lcd.setCursor(0, 1);
  lcd.print(temperatureOptions[0]);
}
