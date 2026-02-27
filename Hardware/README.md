# Hardware Overview — Tux_3

This folder documents the complete hardware design of the Tux_3 RoboCupJunior Entry League robot.

---

## 1. Mechanical Design

The chassis and structural components of Tux_3 are fully custom and **3D-printed** using Autodesk Fusion. The design priorities were:

- **Compactness:** All components fit tightly within the chassis to minimize space.
- **Stability:** Low center of gravity for reliable ball control and collision resistance.
- **Modularity:** Components can be replaced or adjusted without redesigning the entire robot.
- **Precision:** Accurate positioning of sensors and actuators for reliable gameplay.

All mechanical parts, including motor mounts and dribbler assemblies, are printed with durable materials suitable for repeated matches.

---

## 2. Drive System

The robot moves omnidirectionally using three **Pololu 25Dx48L MP 12V motors**:

- Configured for stable movement and precise turning.
- Controlled via **Infineon IFX9201SG motor drivers** on the mainboard.
- Designed for responsive acceleration and deceleration to optimize ball handling.

---

## 3. Ball Handling & Kicking

- **Dribbler:** Powered by a **GA16Y-050-CE motor** for reliable ball control.
- **Kicker:** Driven by **Infineon BTN8982TA** high-current driver for strong, precise kicks.
- Both systems are mechanically aligned to ensure accurate ball trajectory.

---

## 4. Sensors

Tux_3 integrates multiple sensors for perception and field awareness:

- **Vishay TSSP6038TR IR sensors** — detect the ball and assist with positioning.
- **CMPS14 compass module** — provides orientation feedback for navigation.
- **Pixy2 camera** — detects goal positions and supports tactical decision-making.
- **Light barrier system** — monitors ball possession during dribbling.
- **DFRobot_URM09** — detects distance

All sensors are securely mounted on the 3D-printed chassis to ensure stability and minimize interference.

---

## 5. Electronics / Mainboard

The robot’s electronic core is a custom board built around an **ESP32 microcontroller**.  

Key components:

| Component                 | Purpose                                  |
|---------------------------|------------------------------------------|
| ESP32                     | Main control, communication, decision logic |
| Infineon IFX9201SG        | Motor drivers for drivetrain             |
| Infineon BTN8982TA        | High-current driver for kicker           |
| Pixy2                     | Goal detection and image processing      |
| Compass CMPS14            | Orientation stabilization                |
| Light barrier & IR Sensors | Ball detection and possession           |
| DFRobot_URM09             | detecting distance to the wall           |

The mainboard integrates power distribution and signal routing for all motors and sensors.

---

## 6. Design Philosophy

- **Reliability:** Mechanical and electrical design is robust under collisions and fast maneuvers.
- **Maintainability:** Easy access to motors, sensors, and electronics for quick repairs.
- **Performance:** Optimized layout ensures fast response to software commands.
- **Team Collaboration:** Components are modular, allowing multiple team members to work simultaneously on assembly, tuning, or replacement.

---

## 7. Visual Overview

For better understanding, see the following diagrams (to be added):

- Chassis layout (top view, side view)
- Sensor placement diagram
- Dribbler & kicker assembly diagram
- Motor orientation diagram
