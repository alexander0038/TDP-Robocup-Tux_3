# Tux_3 Hardware Overview

This document describes the hardware architecture of the **Tux_3** RoboCupJunior Entry League robot.

## Mechanical Design

The chassis of Tux_3 is fully **3D-printed** and designed in **Autodesk Fusion**.  
It was built to be compact, stable, and easy to maintain.

Compared to the **LEGO-based robot used in the 2025 season**, the custom chassis provides significantly improved stability and durability.

## Drive System

Tux_3 uses a **two-motor drive system**.

- Pololu **25Dx48L MP 12V motors**
- Direct wheel mounting
- **Infineon IFX9201SG motor drivers**

This configuration provides strong acceleration, reliable turning, and a simple mechanical setup.

## Ball Handling and Kicking

### Dribbler

- Powered by a **GA16Y-050-CE motor**
- Maintains ball contact while driving

### Kicker

- Controlled by an **Infineon BTN8982TA** driver
- Mechanically aligned with the dribbler for consistent shots

## Sensors

Tux_3 uses several sensors for perception and navigation.

- **Vishay TSSP6038TR IR sensors** — ball detection  
- **CMPS14 compass** — orientation  
- **Pixy2 camera** — goal detection  
- **Light barrier** — ball possession detection  
- **DFRobot URM09** — distance measurement and wall detection

All sensors are integrated into the chassis to ensure stable readings.

## Electronics

The robot is controlled by an **ESP32 microcontroller**.

- **Infineon IFX9201SG** drivers control the drive motors
- **Infineon BTN8982TA** controls the kicker

The electronics layout is optimized for short cable paths and quick maintenance during competitions.
