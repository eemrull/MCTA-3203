#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>

// Define pins
#define BUTTON_PIN 25
#define POTENTIOMETER_PIN 34
const int door = 2;  // Pin for the door sensor
const int trigPin = 32;  // Ultrasonic Trigger Pin
const int echoPin = 33;  // Ultrasonic Echo Pin

// LCD Initialization for 20x4 screen
LiquidCrystal_I2C lcd(0x27, 20, 4);

// OLED Initialization
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define ESP-NOW settings
uint8_t receiverAddress[] = {0xE0, 0x5A, 0x1B, 0xCB, 0x62, 0x28};

// Data structure for transmission
typedef struct {
  char temperature[8];
  char time[8];
  char function[8];
  int timeRemaining;
  float detergentVolume;  // Detergent volume in percentage
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
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Send Success" : "Send Fail");
}

void setup() {
  lcd.init();
  lcd.backlight();

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED initialization failed!"));
    while (1);
  }
  oled.display();

  oled.setTextSize(2); // Change to a smaller text size for better centering
  oled.setTextColor(WHITE);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(door, INPUT_PULLUP);  // Configure door sensor pin

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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

  // Centered LCD output
  lcd.setCursor(0, 0);
  String message = "Select Temp:";
  int messageLength = message.length();
  int centerPosition = (20 - messageLength) / 2; // Adjust for 20 characters
  lcd.setCursor(centerPosition, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
  Serial.begin(115200);
}

void loop() {
  int doorState = digitalRead(door);  // Read the door sensor state

  // Check if the door is open, if it is, show the "Close the door" message
  if (doorState == HIGH) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Close the door first");
    return;  // Exit the loop to prevent proceeding with option selection
  }

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

    // Clear LCD
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear line
    lcd.setCursor(0, 1);
    displayOptionsOnOLED();
  }
}

void buttonPressed() {
  step++;
  lcd.clear();
  displayOptionsOnOLED();
  
  if (step == 1) {
    String message = "Select Time:";
    int messageLength = message.length();
    int centerPosition = (20 - messageLength) / 2; // Adjust for 20 characters
    lcd.setCursor(centerPosition, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
  } else if (step == 2) {
    String message = "Select Func:";
    int messageLength = message.length();
    int centerPosition = (20 - messageLength) / 2; // Adjust for 20 characters
    lcd.setCursor(centerPosition, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
  } else if (step == 3) {
    // Display Summary on LCD (Row 1 and 2 for title, Row 3 and 4 for options)
    lcd.clear();
    String message = "Summary:";
    int messageLength = message.length();
    int centerPosition = (20 - messageLength) / 2;
    lcd.setCursor(centerPosition, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
    
    lcd.setCursor(0, 2);
    lcd.print(temperatureOptions[selectedIndices[0]]);
    lcd.print(", ");
    lcd.print(timeOptions[selectedIndices[1]]);
    lcd.print(", ");
    lcd.print(functionOptions[selectedIndices[2]]);
    
    // Calculate and display detergent volume
    lcd.setCursor(0, 3);
    lcd.print("Detergent: ");
    int detergentVolume = (int)calculateDetergentVolume();  // Convert to int
    lcd.print(detergentVolume);  // Display as integer (no decimals)
    lcd.print("%");

    delay(500);
    displayWaitMessageOnOLED();
    delay(2500);

    strcpy(userData.temperature, temperatureOptions[selectedIndices[0]]);
    strcpy(userData.time, timeOptions[selectedIndices[1]]);
    strcpy(userData.function, functionOptions[selectedIndices[2]]);
    userData.timeRemaining = parseTime(userData.time);
    userData.detergentVolume = detergentVolume;

    esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
    
    startCountdown(userData.timeRemaining);
    resetSystem();
  }
}

// Ultrasonic sensor distance measurement function
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

// Calculate detergent volume as a percentage
float calculateDetergentVolume() {
  long currentHeight = getDistance();  // Get current height from ultrasonic sensor
  const float initialHeight = 10;  // Initial height (for simplicity)
  
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
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = (SCREEN_HEIGHT - 8 * 3) / 2;  // Adjust vertical position based on text height
  y = y + 4; 
  oled.setCursor(x, y);
  oled.print(message);
  oled.display();
}

void startCountdown(int totalSeconds) {
  while (totalSeconds >= 0) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setCursor(0, 10);
    oled.print(totalSeconds / 60);
    oled.print("m ");
    oled.print(totalSeconds % 60);
    oled.print("s");
    oled.display();
    delay(1000);
    totalSeconds--;

    // Dynamically update detergent volume during the countdown on LCD
    float detergentVolume = calculateDetergentVolume();
    userData.detergentVolume = detergentVolume;

    // Update the detergent volume on the LCD (Row 3)
    lcd.setCursor(0, 3);
    lcd.print("Detergent: ");
    lcd.print(detergentVolume);
    lcd.print("%");

    // Send updated data to the receiver
    esp_now_send(receiverAddress, (uint8_t *)&userData, sizeof(userData));
  }

  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("Complete!");
  oled.display();
  delay(3000);
}


void resetSystem() {
  step = 0;
  memset(selectedIndices, 0, sizeof(selectedIndices));

  lcd.clear();
  String message = "Select Temp:";
  int messageLength = message.length();
  int centerPosition = (20 - messageLength) / 2; // Adjust for 20 characters
  lcd.setCursor(centerPosition, 0);
  lcd.print(message);
  lcd.setCursor(0, 1);
}

void displayOptionsOnOLED() {
  oled.clearDisplay();
  oled.setTextSize(3);  // Set the text size to 2.5 (approximated as 3)
  oled.setTextColor(WHITE);

  String message = (step == 0) ? temperatureOptions[selectedIndices[0]]
                                : (step == 1) ? timeOptions[selectedIndices[1]]
                                              : functionOptions[selectedIndices[2]];

  int textWidth = message.length() * 15;  // Estimate text width (15px per character)
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = (SCREEN_HEIGHT - 8 * 3) / 2;  // Adjust vertical position based on text height
  y = y + 4; 
  oled.setCursor(x, y);
  oled.print(message);
  oled.display();
}
