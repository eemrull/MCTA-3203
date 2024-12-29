#include <esp_now.h>
#include <WiFi.h>

#define BUILTIN_LED 2 // Built-in LED pin (usually 2 on ESP32)

// Variable to store received state
bool limitSwitchState = false;

// Callback when data is received
void onDataRecv(const esp_now_recv_info *recvInfo, const uint8_t *data, int len) {
  memcpy(&limitSwitchState, data, sizeof(limitSwitchState));
  digitalWrite(BUILTIN_LED, limitSwitchState ? HIGH : LOW); // Light LED if state is true

  // Debugging information
  Serial.print("Data received from: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(recvInfo->src_addr[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set up LED pin
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // Nothing to do here; the callback handles everything
}
