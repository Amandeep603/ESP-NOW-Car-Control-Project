#include <WiFi.h>
#include <esp_now.h>

// Joystick pins
#define VRX_PIN 32
#define VRY_PIN 33
#define SW_PIN  25

// Receiver MAC Address (tumne diya hai)
uint8_t receiverAddress[] = {0x2C, 0xBC, 0xBB, 0x0B, 0xE8, 0x00};

// Structure to send data
typedef struct struct_message {
  int xAxis;
  int yAxis;
  bool buttonPressed;
} struct_message;

struct_message joystickData;

void setup() {
  Serial.begin(115200);

  // Joystick pins
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  // Init WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read Joystick values
  joystickData.xAxis = analogRead(VRX_PIN);
  joystickData.yAxis = analogRead(VRY_PIN);
  joystickData.buttonPressed = !digitalRead(SW_PIN); // Active LOW

  // Send data
  esp_now_send(receiverAddress, (uint8_t *) &joystickData, sizeof(joystickData));

  delay(100);  // Send every 100ms
}
