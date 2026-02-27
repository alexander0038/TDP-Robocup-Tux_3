# Tux_3 — RoboCupJunior Soccer Entry League Robot

Welcome to the official repository of **Team Tux_3**, competing in the RoboCupJunior Soccer Entry League.

This repository contains all documentation and source code for our autonomous soccer robot, designed, built, and programmed by our team.

---

## Team Overview

Team **Tux_3** represents a collaboration of two formerly independent teams (Tux_3 and Tux_4 back then). Both teams competed in 2025 using LEGO-based robots, achieving strong results at the Berlin qualification tournament and qualifying for the German Championship in Nuremberg (Tux_3: 8th place, Tux_4: 13th place).  

In 2026, the two teams merged under the name **Tux_3** and developed a fully custom 3D-printed robot using Autodesk Fusion. This new design allowed us to improve stability, ball control, and overall performance. At the 2026 Berlin qualification tournament, our unified team achieved 2nd place.

---

## Hardware Overview

The Tux_3 robot combines custom 3D-printed mechanical parts, precise motors, and multiple sensors for autonomous operation:

- **Chassis:** 3D-printed, optimized for stability and compactness  
- **Drive Motors:** 3 × Pololu 25Dx48L MP 12V  
- **Dribbler Motor:** GA16Y-050-CE  
- **Motor Drivers:** Infineon IFX9201SG (drive), BTN8982TA (kicker)  
- **Controller:** ESP32 Board  
- **Sensors:** Vishay TSSP6038TR (ball detection), CMPS14 (compass), Pixy2 Camera, light barrier (ball possession)

The hardware design focuses on modularity, robustness, and precise ball handling, while ensuring easy maintenance during competitions.

---

## Software Overview

The software controls all robot functions via a structured main loop (≈100 Hz) and a modular task separation:

- **Sensor acquisition**  
- **Movement control**  
- **High-level strategy (attack, search, rescue)**  
- **Stuck detection and recovery**

All software modules are documented in `software/README.md`, including file structure, functionality, and code snippets for illustration. The goal is to explain the logic clearly without requiring jurors to read the full source code.

---

## Documentation

All technical documentation is provided in this repository:

- **Hardware Design:** `docs/hardware.md`  
- **Software Overview:** `software/README.md`  
- **Flowcharts and Diagrams:** `docs/images/`  

---

## Achievements

- **2025:**  
  - Berlin Qualification: Tux_3 (2nd place), Tux_4 (4th place)  
  - German Championship, Nuremberg: Tux_3 (8th place), Tux_4 (13th place)  

- **2026:**  
  - Berlin Qualification: Unified Tux_3 (2nd place)  
  - Fully custom 3D-printed robot designed and implemented

---

## Notes

This repository is intended to accompany our Technical Description Paper (TDP) and poster presentation for RoboCupJunior competitions. All code, hardware details, and diagrams are open for review and study by jurors, coaches, and team members.
