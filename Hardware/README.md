# Hardware Overview — Tux_3

This document describes the complete hardware architecture of the Tux_3 RoboCupJunior Entry League robot.
 
---

## 1. Mechanical Design

The robot chassis is fully custom-designed and manufactured using 3D printing.  
All structural components were designed in Autodesk Fusion and optimized for compactness, durability, and easy maintenance.

Design goals included:

- Low center of gravity for stable driving
- Robust frame for collisions
- Secure mounting positions for sensors
- Easy access to electronics for repairs
- Precise alignment of dribbler and kicker system

The transition from LEGO construction (2025 season) to fully custom 3D-printed parts (2026 season) significantly improved mechanical stability and performance.

---

## 2. Drive System

Tux_3 uses a **two-motor differential drive system**.

### Drive Motors
- **Pololu 25Dx48L MP 12V motors**
- Direct wheel mounting (no omni wheels)
- Controlled via **Infineon IFX9201SG motor drivers**

This configuration provides:

- Strong forward acceleration
- Reliable turning behavior
- Mechanical simplicity
- High robustness in collisions

The differential drive system was selected for reliability and mechanical efficiency within the Entry League constraints.

---

## 3. Ball Handling and Kicking System

### Dribbler
- Powered by a **GA16Y-050-CE motor**
- Designed to maintain continuous ball contact
- Positioned for stable ball control during forward movement

### Kicker
- Driven by an **Infineon BTN8982TA** high-current driver
- Mechanically aligned with the dribbler
- Designed for controlled and repeatable shooting

The combined dribbler-kicker system ensures controlled possession and accurate shots.

---

## 4. Sensor System

To achieve reliable perception and orientation, the robot integrates the following sensors:

### Ball Detection
- **Vishay TSSP6038TR infrared sensors**
- Used to detect the modulated IR signal emitted by the ball
- Mounted to ensure consistent field coverage

### Orientation
- **CMPS14 compass module**
- Provides stable heading reference
- Enables consistent alignment toward the opponent goal

### Goal Detection
- **Pixy2 camera**
- Used to detect the colored goals
- Mounted at the front of the robot for optimal visibility

### Ball Possession
- Light barrier system
- Detects when the ball is inside the dribbler mechanism

### Obstacle / Distance Detection
- **DFRobot_URM09**
- Used for distance measurement
- Supports wall detection and collision correction
- Helps stabilize driving behavior near boundaries

All sensors are firmly mounted within the 3D-printed chassis to reduce vibration and signal interference.

---

## 5. Electronics and Control Board

The robot uses a custom electronic setup centered around:

- **ESP32 microcontroller** as the main control unit
- **Infineon IFX9201SG** motor drivers for drivetrain control
- **Infineon BTN8982TA** for kicker actuation
- Integrated power distribution for motors and sensors

The electronic layout was designed to minimize cable length, reduce electrical noise, and allow fast maintenance during competitions.

---

## 6. Design Philosophy

The hardware design of Tux_3 follows four key principles:

1. Robustness — able to withstand collisions and intensive gameplay
2. Simplicity — reduced mechanical complexity for higher reliability
3. Modularity — components can be replaced quickly
4. Performance — optimized weight distribution and sensor placement

By combining custom 3D-printed mechanics with carefully selected electronic components, the robot achieves stable and reliable performance within the Entry League framework.

---

## 7. Visual Documentation

The following diagrams are recommended:

- Top view of chassis layout
- Side view showing dribbler and kicker alignment
- Sensor placement diagram
- Wiring overview

Example:

![Chassis Layout](figures/chassis_layout.png)
