#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Struct to hold received data
typedef struct {
  char temperature[8];
  char time[8];
  char function[8];
  int timeRemaining;
  float detergentVolume;
} UserData;

UserData receivedData;

// Callback function for receiving data
void onDataReceive(const esp_now_recv_info *recvInfo, const uint8_t *data, int len) {
    // Print sender MAC address
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
             recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);
    Serial.print("Received data from: ");
    Serial.println(macStr);

    // Check if the received data matches the size of the structure
    if (len == sizeof(UserData)) {
        memcpy(&receivedData, data, len);

        // Print the received data in human-readable format
        Serial.println("Received User Data:");
        Serial.print("Temperature: ");
        Serial.println(receivedData.temperature);
        Serial.print("Time: ");
        Serial.println(receivedData.time);
        Serial.print("Function: ");
        Serial.println(receivedData.function);
        Serial.print("Time Remaining: ");
        Serial.println(receivedData.timeRemaining);
        Serial.print("Detergent level: ");
        Serial.println(receivedData.detergentVolume);
    } else {
        Serial.println("Invalid data size received!");
    }
}

void setup() {
    Serial.begin(115200);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // No need for connection

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the receive callback
    esp_now_register_recv_cb(onDataReceive);
}

void loop() {
    // Nothing to do here
}
