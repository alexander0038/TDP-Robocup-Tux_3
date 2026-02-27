# Tux_3 — Strategy Overview

This document describes the high-level strategy and decision-making system of Team Tux_3's RoboCupJunior Soccer Entry League robot. It explains how the robot selects actions during a match, reacts to dynamic game conditions, and implements basic tactical behaviors.

---

## 1. Control Architecture

The robot's software is organized around a **main control loop running at approximately 100 Hz**, combined with modular tasks for:

- Sensor data acquisition  
- Motion control  
- Decision-making and tactical strategy  
- Rescue routines (stuck detection and recovery)

The strategy module is responsible for deciding the robot's behavior based on sensor inputs and match conditions.

---

## 2. High-Level States

The robot operates according to a **state-based system**, switching between the following main states:

1. **GET_BALL**  
   - The robot searches for the ball using infrared sensors (TSSP6038TR).  
   - If the ball is detected, the roboter will drive towards it.  

3. **PLAN_SHOT**  
   - When positioned correctly, the robot evaluates the goal location and determines the best angle and timing for a shot.  
   - Pixy2 camera and orientation data guide the alignment.  

4. **KICK/SHOOT**  
   - Executes the kicking mechanism using the Infineon BTN8982TA driver.  
   - Dribbler motor (GA16Y-050-CE) adjusts ball control during the shot.  

5. **RESCUE / STUCK**  
   - If the robot detects that it is stuck (via sensor readings or lack of movement), a recovery routine is triggered.  
   - This includes reversing, turning, and repositioning to continue play.

---

## 3. Decision Flow

The robot continuously evaluates sensor inputs to determine:

- Ball detection status  
- Obstacle proximity and collisions  
- Robot orientation  
- Field boundaries  

Decisions are made in the following order:

1. Check if **stuck** → trigger RESCUE if necessary  
2. Ball not seen → switch to BACK_TO_OWN_GOAL
3. Detect ball → switch to GET_BALL
4. Check goal visibility → switch to PLAN_SHOT  
5. Ready to shoot → execute KICK/SHOOT  

This priority ensures the robot always reacts to the most critical situation first.

---

## 4. Sensor Integration

- **IR Ball Sensors (TSSP6038TR):** Primary detection of ball position  
- **Pixy2 Camera:** Confirmation of ball and goal position  
- **Compass (CMPS14):** Orientation stabilization for accurate approach and shooting  
- **Light barrier:** Detection of ball possession in the dribbler 

The strategy module fuses these inputs to maintain reliable state transitions and tactical behavior.

---

## 5. Team Considerations

The strategy was designed to be:

- **Simple but effective**: Ideal for Entry League robots with limited processing power  
- **Robust**: Handles unexpected collisions and field dynamics  
- **Modular**: Easy to adjust parameters or add future tactics  

The state-based approach allows the robot to perform consistently in matches, while also making its behavior understandable to judges during interviews and poster presentations.
