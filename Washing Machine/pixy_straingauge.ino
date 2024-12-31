#include <Pixy.h>
#include <HX711.h>

// Initialize Pixy
Pixy pixy;

// Define color signatures
#define GREEN 1
#define PINK 2
#define BLUE 3
#define ORANGE 4
#define RED 5
#define MAX_CLOTHES 10
#define MAX_WEIGHT 1000  // Maximum weight in grams (1 kg)

// Pins for HX711
#define DT 4
#define SCK 2

// Initialize HX711
HX711 scale;

// Persistent variable to track the total number of clothes
int clothesCount = 0;

void setup() {
  // Start the serial monitor and initialize Pixy
  Serial.begin(9600);
  pixy.init();

  // Initialize HX711
  scale.begin(DT, SCK);
  scale.set_scale();  // Set the scale factor to default
  scale.tare();  // Set the current load as zero (tare)
  
  Serial.println("HX711 ready.");
}

void loop() {
  // Retrieve the number of detected blocks
  int blocks = pixy.getBlocks();
  long weight = scale.get_units(10);  // Get the weight in grams, average over 10 readings

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
      Serial.print("Weight: ");
      Serial.print(weight);
      Serial.println(" grams");

      // Check if the number of clothes exceeds the washer's capacity or weight exceeds 1kg
      if (clothesCount == MAX_CLOTHES) {
        Serial.println("Washer is full!");
        break;  // Stop checking for more clothes once the limit is reached
      }

      if (weight > MAX_WEIGHT) {
        Serial.println("Exceeded weight limit of 1kg!");
        break;  // Stop checking if the weight exceeds 1kg
      }
    }
  }

  delay(1000); // Short delay for stability
}
