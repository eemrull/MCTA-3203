#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
const int threshold = 1000; // Adjust this threshold based on testing
const int gyroscopeThreshold = 500; // Additional threshold for gyroscope data
int previousGesture = -1;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
}

void loop() {
  int gesture = detectGesture();
  
  if (gesture != previousGesture) { // Check if gesture has changed
    Serial.print("Detected Gesture: ");
    if (gesture == 1) {
      Serial.println("Gesture 1");
      // Action for Gesture 1
    } else if (gesture == 2) {
      Serial.println("Gesture 2");
      // Action for Gesture 2
    } else if (gesture == 3) {
      Serial.println("Gesture 3");
      // Action for Gesture 3
    } else if (gesture == 4) {
      Serial.println("Gesture 4");
      // Action for Gesture 4
    }
    // Add more gesture cases if needed
    previousGesture = gesture;
  }
}

int detectGesture() {
  int ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Recognize specific gestures by evaluating conditions on accelerometer and gyroscope data

  // Gesture 1: Fast movement in positive x-direction
  if (ax > threshold && ay < threshold && gx > gyroscopeThreshold) {
    return 1;
  }
  // Gesture 2: Fast movement in negative x-direction
  else if (ax < -threshold && ay > threshold && gy > gyroscopeThreshold) {
    return 2;
  }
  // Gesture 3: Upward movement along y-axis with significant rotation
  else if (ay > threshold && az > threshold && gx < -gyroscopeThreshold) {
    return 3;
  }
  // Gesture 4: Downward movement along y-axis with rotation in opposite direction
  else if (ay < -threshold && az > threshold && gy < -gyroscopeThreshold) {
    return 4;
  }
  // Add more gestures as needed
  return 0; // No gesture detected
}
