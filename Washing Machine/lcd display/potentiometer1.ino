#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins
#define BUTTON_PIN 25
#define POTENTIOMETER_PIN 34

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if necessary

// Variables for the steps
const char *temperatureOptions[] = {"20C", "40C", "60C", "90C"};
const char *timeOptions[] = {"15m", "30m", "45m", "60m"};
const char *functionOptions[] = {"Wash", "Rinse", "Spin"};

int step = 0; // Current step: 0 = Temp, 1 = Time, 2 = Function, 3 = Summary
int selectedIndices[] = {0, 0, 0}; // To store selected index for Temp, Time, and Function
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool buttonState = HIGH;
bool lastButtonState = HIGH;

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Set up button and potentiometer
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_PIN, INPUT);

  // Start with the first step
  lcd.setCursor(0, 0);
  lcd.print("Select Temp:");
  lcd.setCursor(0, 1);
  lcd.print(temperatureOptions[selectedIndices[0]]);
  Serial.begin(115200);
}

void loop() {
  // Read the button state with debouncing
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

  // Adjust value with potentiometer
  if (step < 3) { // Only adjust in steps 0, 1, 2
    int potValue = analogRead(POTENTIOMETER_PIN);
    Serial.println(potValue);
    int maxIndex = (step == 0) ? 3 : (step == 1) ? 3 : 2; // Max index based on step
    selectedIndices[step] = map(potValue, 0, 2500, 0, maxIndex);
    selectedIndices[step] = constrain(selectedIndices[step], 0, maxIndex);

    // Update LCD
    lcd.setCursor(0, 1);
    if (step == 0) {
      lcd.print(temperatureOptions[selectedIndices[0]]);
    } else if (step == 1) {
      lcd.print(timeOptions[selectedIndices[1]]);
    } else if (step == 2) {
      lcd.print(functionOptions[selectedIndices[2]]);
    }
  }
}

void buttonPressed() {
  if (step < 3) {
    // Lock the current selection and move to the next step
    step++;

    lcd.clear();

    if (step == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Select Time:");
      lcd.setCursor(0, 1);
      lcd.print(timeOptions[selectedIndices[1]]);
    } else if (step == 2) {
      lcd.setCursor(0, 0);
      lcd.print("Select Func:");
      lcd.setCursor(0, 1);
      lcd.print(functionOptions[selectedIndices[2]]);
    } else if (step == 3) {
      // Display summary
      lcd.setCursor(0, 0);
      lcd.print("Summary:");
      lcd.setCursor(0, 1);
      lcd.print(temperatureOptions[selectedIndices[0]]);
      lcd.print(", ");
      lcd.print(timeOptions[selectedIndices[1]]);
      lcd.print(", ");
      lcd.print(functionOptions[selectedIndices[2]]);

      delay(3000);

      // Start washing process
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Starting...");
      delay(3000);

      // Start countdown
      startCountdown();

      // Reset system
      resetSystem();
    }
  }
}

void startCountdown() {
  int totalSeconds = parseTime(timeOptions[selectedIndices[1]]);
  while (totalSeconds >= 0) {
    lcd.setCursor(0, 0);
    lcd.print("Time Left:     "); // Ensure the previous text is cleared
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Display the remaining time
    lcd.setCursor(0, 1);
    lcd.print(minutes);
    lcd.print("m ");
    lcd.print(seconds);
    lcd.print("s    "); // Padding to clear previous text

    delay(1000); // Wait for 1 second
    totalSeconds--;
  }

  // Countdown finished
  lcd.clear();
  lcd.setCursor(0, 0);
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
  selectedIndices[0] = 0;
  selectedIndices[1] = 0;
  selectedIndices[2] = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Temp:");
  lcd.setCursor(0, 1);
  lcd.print(temperatureOptions[selectedIndices[0]]);
}
