#include <Pixy.h>

// Initialize Pixy
Pixy pixy;

// Define color signatures
#define GREEN 1
#define PINK 2
#define BLUE 3
#define ORANGE 4
#define RED 5
#define MAX_CLOTHES 10 // Maximum capacity of the washer

void setup() {
  // Start the serial monitor and initialize Pixy
  Serial.begin(9600);
  pixy.init();
}

void loop() {
  // Retrieve the number of detected blocks
  int blocks = pixy.getBlocks();
  static int clothesCount = 0; // Use static so the count persists across loop iterations

  if (blocks) {
    // Iterate through detected blocks
    for (int i = 0; i < blocks; i++) {
      int signature = pixy.blocks[i].signature;

      // Count only blocks with the 5 predefined color signatures
      if (signature == GREEN || signature == PINK || signature == BLUE || signature == ORANGE || signature == RED) {
        // Increment the count
        clothesCount++;
      }
    }
  }

  // Display appropriate message
  if (clothesCount > MAX_CLOTHES) {
    Serial.println("Exceeded washer capacity! Please remove some clothes.");
  } else {
    Serial.print("Total clothes detected: ");
    Serial.println(clothesCount);
  }

  delay(1000); // Short delay for stability
}
