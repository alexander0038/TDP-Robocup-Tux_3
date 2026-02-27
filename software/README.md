# Software Overview — Tux_3

This folder contains the complete software for the Tux_3 RoboCupJunior Entry League robot.

## 🧠 Architecture

The software is structured into the following modules:

- **main.ino** — Main control loop and initialization
- **sensors.cpp / sensors.h** — Handles all sensor reading (IR TSSP6038, light barrier, compass)
- **movement.cpp / movement.h** — Motor control (Pololu drivetrain & dribbler)
- **strategy.cpp / strategy.h** — High-level decision logic (attack/search/rescue)

The ESP32 runs a main loop with approx. 100 Hz frequency. Within this loop:
1. Sensors are updated
2. State decisions are evaluated
3. Movement commands executed

## 🧪 Dependencies

- CoRoSoN Software
- ESP32 Arduino Core
- Pixy2 library
- Motor driver interfaces

## 📌 How to Run

1. Open in Arduino IDE
2. Select ESP32 board
3. Upload `main.ino`

## 🧠 Logic Summary

The software implements a state-based decision system:
- **GET_BALL**
- **SEARCH_FOR_ENEMY_GOAL**
- **TURN_TO_ENEMY_GOAL**
- **DRIVE_TOWARDS_GOAL**
- **RESCUE**

Each state has its own behavior function (see strategy.cpp).

## 🔍 Example Code Snippets

```cpp
if (irRing.BallSeen) {
    currentState = GET_BALL;
}

if (stuck) {
    currentState = RESCUE;
}
