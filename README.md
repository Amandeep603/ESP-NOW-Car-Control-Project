# ESP-NOW-Car-Control-Project

Welcome to the **ESP32 ESP-NOW Car Control System**!  
This project allows you to control your robotic car in two exciting modes:
- 🎮 **Manual Mode** — drive the car yourself using a Joystick over ESP-NOW wireless communication.
- 🤖 **Autonomous Mode** — let the car drive itself and intelligently avoid obstacles!

---

## 📂 Project Structure

| File Name | Description |
|:-----------|:------------|
| `mac_address_finder.ino` | Sketch to find the MAC Address of ESP32 devices |
| `transmitter_joystick.ino` | Code for the Joystick-controlled ESP32 (Transmitter) |
| `receiver_manual_car.ino` | Code for the Car ESP32 (Receiver) in Manual (Joystick) Mode |
| `receiver_autonomous_obstacle_car.ino` | Code for the Car ESP32 (Receiver) in Autonomous Obstacle Avoidance Mode |

---

## 🛠 Hardware Requirements

- 2 × ESP32 Development Boards
- 1 × Joystick Module (for manual driving)
- 1 × L298N Motor Driver Module
- 2 × DC Motors + Wheels + Chassis
- 1 × Ultrasonic Sensor (HC-SR04) (for obstacle detection)
- Power Supply (Rechargeable Battery Pack)
- Jumper Wires, Breadboard

---

## ⚙️ How It Works

### 1. Manual Mode (Joystick Control)

- **Transmitter ESP32** reads joystick input (forward, backward, left, right).
- It sends control commands wirelessly over **ESP-NOW** to the **Receiver ESP32** on the car.
- **Receiver ESP32** drives the motors based on the received direction.

### 2. Autonomous Mode (Obstacle Avoidance)

- The **Receiver ESP32** uses an ultrasonic sensor to measure distances.
- If an obstacle is detected in front, the car automatically turns left or right to avoid collision.
- No joystick needed — fully autonomous operation.

---

## 📡 ESP-NOW Communication Flow

[Transmitter ESP32 (Joystick)] ⇩ ESP-NOW Wireless Signal ⇩ [Receiver ESP32 (Car)] ⇨ Motor Driver ⇨ Motors

- ESP-NOW is a **fast, connectionless protocol** perfect for low-latency robotic projects.
- No WiFi Router required!

---

## 🚀 Getting Started

### Step 1: Find ESP32 MAC Address
- Upload `mac_address_finder.ino` to your **Receiver ESP32**.
- Open Serial Monitor at 115200 baud rate.
- Copy the printed MAC Address — you will use it inside the `transmitter_joystick.ino` to send data to correct receiver.

### Step 2: Manual Joystick Car Mode
1. Upload `transmitter_joystick.ino` to your **Transmitter ESP32** (connected to Joystick).
2. Upload `receiver_manual_car.ino` to your **Receiver ESP32** (on the car).
3. Power both boards.
4. Control your car manually with the Joystick!

### Step 3: Autonomous Obstacle Avoidance Mode
1. Upload `receiver_autonomous_obstacle_car.ino` to your **Receiver ESP32**.
2. No need for transmitter board.
3. The car will automatically start detecting and avoiding obstacles!
