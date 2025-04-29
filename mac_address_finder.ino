#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  // WiFi mode set karna IMPORTANT hai
  WiFi.mode(WIFI_STA);

  delay(100);  // Thoda wait karna bhi zaruri hai

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}

find mac address of esp32