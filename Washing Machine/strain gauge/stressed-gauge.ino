#include <HX711_ADC.h>

// Pins:
const int HX711_dout = 3; // MCU > HX711 dout pin
const int HX711_sck = 4;  // MCU > HX711 sck pin

// HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

unsigned long t = 0;

void setup() {
  Serial.begin(57600);
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // Precision right after power-up can be improved by adding stabilizing time
  boolean _tare = true; // Set to false if you don't want tare to be performed
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(62781.01); // Hardcoded calibration value
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; // Increase value to slow down serial print activity

  // Check for new data/start next conversion:
  if (LoadCell.update())
    newDataReady = true;

  // Get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load cell output value: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  // Receive command from serial terminal
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't')
      LoadCell.tareNoDelay(); // Tare
  }

  // Check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
