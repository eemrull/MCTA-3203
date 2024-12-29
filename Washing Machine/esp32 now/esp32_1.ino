#include <esp_now.h>
#include <WiFi.h>

#define LIMIT_SWITCH_PIN 13 // Adjust this pin number as needed

// MAC address of ESP 2 (receiver)
uint8_t esp2Address[] = {0xE0, 0x5A, 0x1B, 0xCB, 0x4D, 0x6C};

// Variable to store the state of the limit switch
bool limitSwitchState = false;

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set up limit switch pin
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);

  // Initialize WiFi in station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(onDataSent);

  // Add peer (ESP 2)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, esp2Address, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read limit switch state
  limitSwitchState = digitalRead(LIMIT_SWITCH_PIN) == LOW; // LOW when pressed

  // Send state to ESP 2
  esp_now_send(esp2Address, (uint8_t *)&limitSwitchState, sizeof(limitSwitchState));

  // Small delay for stability
  delay(100);
}
