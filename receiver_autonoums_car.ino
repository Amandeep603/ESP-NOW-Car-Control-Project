#include <WiFi.h>
#include <esp_now.h>

// Motor pins
#define ENA 22
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define ENB 23

// Ultrasonic pins
#define TRIG_PIN 26
#define ECHO_PIN 27

// Structure to receive data
typedef struct struct_message {
  int xAxis;
  int yAxis;
  bool buttonPressed;
} struct_message;

struct_message incomingData;

// Mode: 0 = Manual, 1 = Autonomous
int mode = 0;
bool lastButtonState = false;

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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

  // Detect button press (only on transition)
  if (incomingData.buttonPressed && !lastButtonState) {
    mode = 1 - mode;  // Toggle mode
    Serial.print("Mode Changed to: ");
    Serial.println(mode == 0 ? "Manual" : "Autonomous");
  }
  lastButtonState = incomingData.buttonPressed;
}

void controlCarManual() {
  int xAxis = incomingData.xAxis;
  int yAxis = incomingData.yAxis;

  if (yAxis > 3000 && abs(xAxis - 2048) < 800) { 
    // Move Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  else if (yAxis < 1000 && abs(xAxis - 2048) < 800) {
    // Move Backward
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

long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
  long distance = duration * 0.034 / 2; // cm

  return distance;
}

void controlCarAutonomous() {
  long distance = readUltrasonicDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 20 || distance == 0) { 
    // Move Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
  }
  else {
    // Obstacle detected, move back and turn
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
    delay(300); // Move back a little

    // Turn
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(400); // Turn time
  }
}

void loop() {
  if (mode == 0) {
    controlCarManual();
  }
  else {
    controlCarAutonomous();
  }
}
