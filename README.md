Mecanum Wheel Control with Stepper Motors & ROS 2 on Wemos D1 R32 (ESP32) + CNC Shield V3

My Test Video
Watch the demo here: https://youtu.be/DWA5EpyQzwQ

Video Overview
In this video, we use a joystick to control the robot, and ROS 2 converts the joystick input into cmd_vel messages. The robot movement is then controlled via cmd_vel commands.
![image](https://github.com/user-attachments/assets/1ad71b93-bfb3-4e33-8a22-8aebcc55ecee)


ROS Setup
Within ROS, the cmd_vel message is used to control the movement of the robot.
![image](https://github.com/user-attachments/assets/d9365329-0cb6-402d-8934-6d0d4069a4a8)


Serial Bridge Node
We created a serial_bridge_node to extract values from the cmd_vel topic and send them through Serial (UART) to the MCU (Wemos D1 R32).

The node only transmits vx, vy, and wz (linear velocity and angular velocity).
![image](https://github.com/user-attachments/assets/9bd89352-faf3-4a98-9e0d-ec436ba283b7)

Why Wemos D1 R32?
The reason we chose the Wemos D1 R32 (ESP32) over the R4 WiFi is because we encountered a problem where the R4 WiFi would freeze after reading data for a while. We suspect that this issue is related to the clock speed, and we believe the ESP32 offers better performance for this task.

Additionally, when using the CNC Shield V3 with the Wemos D1 R32, we had to make a minor modification: by disconnecting the resistor connected to 5V, the system works properly without issues.
![image](https://github.com/user-attachments/assets/2527e12d-39b8-4f54-b7d0-242d67e6be79)

Sequence Diagram of Code on Wemos D1 R32 (ESP32)
This sequence diagram illustrates how the system works when running the code on the Wemos D1 R32 (ESP32). It shows the interactions between the joystick, ROS 2, the serial communication, and the motor control via the CNC Shield V3.
![สกรีนช็อต 2025-04-03 022816](https://github.com/user-attachments/assets/b5dd2071-487e-4b6f-b195-e9314e54da75)

