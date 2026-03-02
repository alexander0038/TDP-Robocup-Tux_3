# TEAM DESCRIPTION PAPER 
 
## Short Team Summary

Team Tux_3 competes in the RoboCupJunior Soccer Entry League and was formed in 2026 by merging two previously independent teams.

In the 2025 season, the members competed separately as Tux_3 (Alexander and Henning) and Tux_4 (Max and Toni), both using LEGO-based robots. At the Berlin qualification tournament, Tux_3 achieved 2nd place and Tux_4 achieved 4th place. Both teams qualified for the German Championship in Nuremberg, where Tux_3 finished 8th (one match away from European qualification) and Tux_4 finished 13th.

In 2026, the two teams combined their experience and formed a single team under the name Tux_3, redesigning the robot with fully custom 3D-printed components. At the Berlin qualification tournament 2026, the new team achieved 2nd place.

## Short Robot Description 

Tux_3 is a custom-built RoboCupJunior Entry League soccer robot designed for reliability, robustness, and continuous improvement. After competing with LEGO-based robots in 2025, the team fully redesigned the system for the 2026 season using custom 3D-printed components and a self-developed electronic architecture. 

The robot is based on a two-motor differential drive system with integrated ball control and goal detection mechanisms. The overall design focuses on mechanical stability, precise ball handling, and structured decision-making during matches. It uses an IR-Ring to detect the ball's direction, Ultrasonic Sensors to find the ball.

 
## Hardware 

#### Drive System 

  - 2× Pololu 25Dx48L MP 12V motors 

  - Differential drive configuration 

   - Infineon IFX9201SG motor drivers 

#### Ball Handling 

  - GA16Y-050-CE motor for dribbler 

  - Infineon BTN8982TA driver for kicker 

  - Light barrier for ball possession detection 

#### Sensors 

  - Vishay TSSP6038TR infrared sensors for ball detection 

  - CMPS14 compass module for orientation stabilization 

  - Pixy2 camera for goal detection 

  - DFRobot URM09 Ultrasonic Sensor for distance measurement and wall correction 

#### Electronics 

  - ESP32 microcontroller 

  - Custom wiring and structured component layout 

  - 3D-printed chassis designed in Autodesk Fusion 

 

More about the Hardware in: `/Hardware/README.md`

 

## Software 

The software is structured around a high-frequency main control loop running on the ESP32. The system separates sensor acquisition, decision logic, and motor control into modular components. We got our program-base from the CoRoSoN-Kit (https://github.com/ItzJannis/CoRoSoN-Package_Draft).

Key characteristics: 

  - State-based decision logic 

  - Ball tracking using infrared sensors 

  - Goal alignment using camera and compass 

  - Custom stuck-detection and recovery behavior 

  - Wall correction using ultrasonic feedback 

The software was developed with a focus on maintainability, clarity, and match robustness. 

 

More about software in `/software/README.md`

## Achievements

### 2025:
- Berlin qualification: *2nd Place: Tux_3; 4th Place: Tux_4*
- German Open Nuremberg: *8th Place: Tux_3; 12th Place: Tux_4*

### 2026:
- Berlin qualification: *2nd: Tux_3* 

## Challenges During the Season 

During the transition from LEGO to a fully custom 3D-printed robot, several challenges were encountered: 

  - **Mechanical Stability:** 
    Early prototypes suffered from structural weaknesses and misalignment in the dribbler system. 

  - **Different programming languages:** 
    We started with the programming language NXC, which translates to “Not exactly C”, and later switched to C/C++. 

  - **Sensor Calibration:** 
    Reliable ball detection and consistent compass readings required extensive testing and calibration. 

  - **Differential Drive Precision:** 
    Achieving accurate turning and goal alignment without omni wheels required significant tuning of motor control. 

 

## Challenges Successfully Mastered 

  - Structural redesign improved mechanical robustness. 

  - Stable ball control was achieved through optimized dribbler positioning. 

  - Sensor placement and filtering significantly increased detection reliability. 

  - Stuck-detection logic improved match recovery performance. 

  - Team collaboration improved after merging two former teams into one unified structure. 

These improvements led to a 2nd place finish at the 2026 Berlin qualification tournament. 

 

## Future Plans 

For the next season, the team plans to: 

  - Compete in Robocup Junior Soccer Lightweight. 

  - Learn more about the hardware. 

  - Deepen our knockledge about programming
    
  - Faster analyzing/finding mistake in hardware/software 

  - Use more teampower than before (if ’s even possible) 

We hope that one day we compete in the World Championship and be the first place. What sounds like a dream might come true one day. 
