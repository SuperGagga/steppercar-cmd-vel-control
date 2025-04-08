# Mecanum Wheel Control with Stepper Motors & ROS 2  
**Platform:** Wemos D1 R32 (ESP32) + CNC Shield V3
![image](https://github.com/user-attachments/assets/1ad71b93-bfb3-4e33-8a22-8aebcc55ecee)

## Demo Video  
Watch the demo here: [YouTube Demo](https://youtu.be/DWA5EpyQzwQ)

## Overview  
This project demonstrates how to control a 4-wheeled Mecanum robot using stepper motors and ROS 2.  
A joystick is used to control the robot, with ROS 2 converting joystick inputs into velocity commands (`cmd_vel`).  
These commands are sent to the ESP32 via UART for motor control.
![image](https://github.com/user-attachments/assets/d9365329-0cb6-402d-8934-6d0d4069a4a8)

## Features
- ROS 2 integration
- Serial communication between ROS 2 and ESP32
- Mecanum wheel kinematics with `vx`, `vy`, and `wz` velocity components
- Stepper motor control using CNC Shield V3
![image](https://github.com/user-attachments/assets/9bd89352-faf3-4a98-9e0d-ec436ba283b7)

## ROS 2 Setup  
Within ROS 2, the robot is controlled using the standard `cmd_vel` topic.  
A custom node called `serial_bridge_node` subscribes to this topic and transmits the following values over UART:
- `vx`: Linear velocity in the x-direction  
- `vy`: Linear velocity in the y-direction  
- `wz`: Angular velocity around the z-axis  

## Serial Bridge Node  
The `serial_bridge_node` is a simple ROS 2 node that:
- Subscribes to `/cmd_vel`
- Extracts `vx`, `vy`, and `wz`
- Sends these values via UART to the MCU (ESP32)

## Why I Chose Wemos D1 R32 (ESP32)  
I initially tried using the **Arduino UNO R4 WiFi**, but encountered an issue where the board would **freeze after reading serial data for a short time**.  
I suspect this was due to the **lower clock speed** (48 MHz) of its main microcontroller (Renesas RA4M1).  

## Comparison: Arduino UNO R4 WiFi vs Wemos D1 R32 (ESP32)

| Feature                 | Arduino UNO R4 WiFi                             | Wemos D1 R32 (ESP32)                          |
|------------------------|--------------------------------------------------|-----------------------------------------------|
| **Main MCU**           | Renesas RA4M1 (Cortex-M4 @ 48 MHz)              | ESP32 (Xtensa LX6 Dual-core @ 240 MHz)        |
| **Wireless**           | ESP32-S3 (Wi-Fi + BLE)                          | Integrated Wi-Fi + Bluetooth 4.2              |
| **Flash Memory**       | 256 KB                                          | 4 MB                                           |
| **SRAM**               | 32 KB                                           | 520 KB                                         |
| **Operating Voltage**  | 5V                                              | 3.3V (5V input via VIN or USB)                |
| **Logic Level**        | 5V                                              | 3.3V                                           |
| **GPIO Pins**          | 14 Digital, 6 Analog                            | ~30 (Shared across multiple functions)        |
| **USB Interface**      | USB-C                                           | Micro USB                                     |
| **Form Factor**        | Arduino UNO Compatible                          | Arduino UNO Compatible                        |
| **Best For**           | Simpler, 5V projects with Arduino ecosystem     | Performance-intensive or Wi-Fi/BLE projects   |

After switching to the **Wemos D1 R32**, which is based on the **ESP32** running at up to **240 MHz**, the issue was resolved.  
The ESP32 provided better stability and performance for my application.

### CNC Shield Modification  
When using the CNC Shield V3 with the Wemos D1 R32, I had to:
- Disconnect the 5V resistor on the shield  
This minor hardware tweak allows the system to work properly without power conflicts.
![image](https://github.com/user-attachments/assets/2527e12d-39b8-4f54-b7d0-242d67e6be79)

## Sequence Diagram
This sequence diagram illustrates how the system works when running the code on the Wemos D1 R32 (ESP32).
![สกรีนช็อต 2025-04-03 022816](https://github.com/user-attachments/assets/b5dd2071-487e-4b6f-b195-e9314e54da75)

