#include <WiFi.h>
#include <esp_now.h>

// Motor pins
#define ENA 22
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define ENB 23

// Structure to receive data
typedef struct struct_message {
  int xAxis;
  int yAxis;
  bool buttonPressed;
} struct_message;

struct_message incomingData;

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Init WiFi
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  Serial.print("X: ");
  Serial.print(incomingData.xAxis);
  Serial.print(" | Y: ");
  Serial.println(incomingData.yAxis);

  controlCar();
}

void controlCar() {
  int xAxis = incomingData.xAxis;
  int yAxis = incomingData.yAxis;

  if (yAxis > 3000 && abs(xAxis - 2048) < 800) { 
    // Move Forward (and joystick is straight)
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  else if (yAxis < 1000 && abs(xAxis - 2048) < 800) {
    // Move Backward (and joystick is straight)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  else if (xAxis < 1000) {
    // Turn Left
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  else if (xAxis > 3000) {
    // Turn Right
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  else {
    // Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
}

void loop() {
  // Nothing here, everything handled in OnDataRecv()
}