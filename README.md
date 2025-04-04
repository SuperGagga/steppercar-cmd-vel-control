# steppercar-cmd-vel-control
Mecanum Wheel Control with Stepper Motors & ROS 2 on Wemos D1 R32 (ESP32) + CNC Shield V3
my test video
https://youtu.be/DWA5EpyQzwQ?si=feCKhfnWkkzm6Umq
อันนี้เป็นสิ่งที่เกิดขึ้นในวิดีโอ เราใช้จอยในการควบคุมแล้ว ROS ก็แปลงจอยออกมาเป็น cmd_vel
![image](https://github.com/user-attachments/assets/1ad71b93-bfb3-4e33-8a22-8aebcc55ecee)

โดยภายใน ROS จะ

![image](https://github.com/user-attachments/assets/d9365329-0cb6-402d-8934-6d0d4069a4a8)

อันนี้เป็น serial_bridge_node ที่เราเขียนขึ้นมาจำทำการดึงค่าจาก topic cmd_vel

![image](https://github.com/user-attachments/assets/9bd89352-faf3-4a98-9e0d-ec436ba283b7)

แล้วส่งผ่าน serial ให้ กับ MCU WemosD1R32
