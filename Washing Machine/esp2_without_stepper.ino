#include <Pixy.h>
#include "DHT.h"

// Initialize Pixy
Pixy pixy;

#define DHTPIN 4        // GPIO pin for DHT (D4 on NodeMCU)
#define DHTTYPE DHT11    // DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

// Define color signatures
#define GREEN 1
#define PINK 2
#define BLUE 3
#define ORANGE 4
#define RED 5
#define MAX_CLOTHES 10

// Persistent variable to track the total number of clothes
int clothesCount = 0;

void setup() {
  // Start the serial monitor and initialize Pixy
  Serial.begin(115200);
  dht.begin();
  pixy.init();
}

void loop() {
  // Retrieve the number of detected blocks
  int blocks = pixy.getBlocks();

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

  delay(1000);

  if (blocks) {
    // Iterate through detected blocks
    for (int i = 0; i < blocks; i++) {
      int signature = pixy.blocks[i].signature;

      // Check if the detected block matches any of the trained color signatures
      if ((signature == GREEN || signature == RED || signature == BLUE || signature == ORANGE || signature == PINK) && clothesCount < MAX_CLOTHES) {
        clothesCount++;  // Increment the count only if it is less than the maximum
      }

      // Print the total number of detected clothes
      Serial.print("Total clothes detected: ");
      Serial.println(clothesCount);
      delay(1000);

      // Check if the number of clothes exceeds the washer's capacity
      if (clothesCount == MAX_CLOTHES) {
        Serial.println("Washer is full!");
        break;  // Stop checking for more clothes once the limit is reached
      }
    }
  }

  delay(1000); // Short delay for stability
}