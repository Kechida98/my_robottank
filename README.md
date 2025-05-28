
## Bluetooth-Controlled Robot — Thesis Project Documentation

## Introduction

![Image](https://github.com/user-attachments/assets/56c68bdc-5fcf-4afe-8d5d-3ee4bed5888d)

This project is my thesis work, where I practice documenting my development process on GitHub. The aim is to build a Bluetooth-controlled robot using the ESP32 microcontroller, integrating motors, servos, and a camera module for live streaming.

Throughout the prototypes, I explore hardware setups, motor control, Bluetooth integration with a PS4 controller, servo mechanisms for pan-tilt camera control, and Wi-Fi streaming using ESP32-CAM. I also address common challenges such as power management, Bluetooth and Wi-Fi interference, and MQTT messaging for remote alerts.

Each prototype details what was done, the problems encountered, and how they were solved — providing a clear and structured record of progress and lessons learned. This documentation is part of my thesis project and serves both as a practical development diary and as a demonstration of effective project documentation.

## Prototype 1
In prototype 1 I tried to to run one dc motor to see if it works good runing it forward and backward with pulse with modulation=pwm.

Components used in Prototype 1:

- MCU: ESP32-C6/ will be changed to esp32 wroom 32e for bluedroid/bluetooth - classic later. Because Esp32-C6 is only compatible with BLE = Bluetooth low energy and wroom can do both.
- Motor Driver: L298N
- Power: 2x 3.7V Li-ion batteries with holder
- one DC Motor with one plastic wheel
- Jumper cables
Motor A was tested for forward and backward movement using PWM.

### Picture/Video below:
https://github.com/user-attachments/assets/0ee76e86-15a2-43bc-93b2-a7ba8820f6ca

### Problem: 
A screw got stuck inside the DC motor because it was fastened in the wrong direction, which locked the motor. It had to be removed.

### Solution: 
Replaced the motor to resolve the issue.


## Prototype 2
In Prototype 2, I focused on running two DC motors simultaneously to test synchronized motion — making sure they start and stop together properly.

I also bought the kit chassi to improve structure, stability, and overall design. Well it was either build my own chassi, wich i would do if i had a 3d printer or material to craft one. This helps with both functionality and future expansion (like mounting sensors, battery, and ESP board).

### Components added:
- 1x Dc motors 12v
- 2x bearing wheels
- 2x drive wheels
- 1x chassi
- 2x plastic tracks
- jumper cables


### Picture/Video below:
![Image](https://github.com/user-attachments/assets/8655fdf0-c675-4677-bba9-76596d624ec1)

![Image](https://github.com/user-attachments/assets/f9de452d-4bb6-4732-a826-fda9fbc57b1c)

https://github.com/user-attachments/assets/cef2f348-9651-4c71-ade7-fe4c406b4a01

### Problem: 
The motor control was behaving strangely and was lagging because the functions ledcSetup and ledcAttachPin had been removed in the Arduino library I was testing with.

### Temporary solution: 
Used analogWrite() instead, but it caused sluggish motor performance.

### Final fix: 
Switched to the updated ledcAttach() function after reviewing Arduino ESP32 migration guide from 2.x to 3.0 (solved in Prototype 5).


## Prototype 3
In Prototype 3, I tested left and right movement, as well as forward and backward — all **without powering the microcontroller from my laptop**.  
This means the robot runs fully on battery power, which is a step closer to going wireless. 

However, to continue safely, I’ll need to **solder, tape, or glue** the components so they don’t get damaged during further testing.  
I’m saving that part for when I start working with **Bluetooth control** for the robot — **unless I decide to start with Bluetooth right away**.  
For now, I’ll continue powering the microcontroller externally.

### Picture/Video below:
https://github.com/user-attachments/assets/44a64f3d-078d-4d90-97d8-be7de222b782

See problem/solution under Prototype 2, as they share the same issue.

## Prototype 4 – PS4 Controller + ESP32 WROOM-32
In this prototype, I connected a PS4 controller to an ESP32 WROOM-32 via Bluetooth. My previous ESP board wasn’t compatible with Bluedroid, so I switched to the WROOM-32.

I’ve been using Ricardo Quesada’s **Bluepad32.h** library to read input from the controller, including joysticks, buttons, R2 (throttle), and L2 (brake).

### Features:
- Bluetooth connection with PS4 controller
- Joystick and button input reading
- Console debug using Bluepad32
- Ready for motor control with PWM
- Control 2 DC motors using joystick and triggers


### Extra testing:
I also tested a version of the code that runs the DC motors without using the PS4 controller, just to verify that PWM and motor wiring worked properly with the ESP32 WROOM-32E.
At this point, I’ve also mounted the main components on the robot (not perfectly), mainly to confirm that everything is functional and fits together.

### Picture/Video below:
https://github.com/user-attachments/assets/48a90633-8d2c-4bc6-a11b-93db7a30fcfe

https://github.com/user-attachments/assets/8166ed5d-1440-46b8-b354-8f1784d7b184

### Problem: 
Initial issues using the Bluepad32 library due to using the ESP32-C6 (not compatible with Bluedroid). Switched to ESP32 WROOM-32E.
Repeatedly updated the wrong CMakeLists.txt file, causing the library not to link properly.
Had an incompatible ESP-IDF version (5.5) that was too new.
Conflicts with Arduino core files causing corruption, especially with IPv6 and WiFi/DNS logic, which conflicted with own wifi_handler.

### Solution: 
Corrected CMakeLists.txt in main, downgraded ESP-IDF to 5.3.2, and manually removed conflicting Arduino files.

## Prototype 5 – PS4 Controller Drive Logic & Pivot Support + ESP32 WROOM-32
In this prototype, I implemented full drive control logic using a PS4 controller connected to an ESP32 WROOM-32 via Bluetooth, using the Bluepad32 library. The goal was smoother driving and adding support for pivoting turning in place.

### What was done:
- Bluetooth connection with PS4 controller using Bluepad32.
- Joystick and button input handling (X, Y, A, B, R2, L2).
- Console debugging using Console.printf(), for better realtime feedback.
- Motor control using PWM via ledcWrite() and automatic ledcAttach().
- Directional control using L298N motor driver.

### Drive Logic Overview:
- Movement: Forward, backward, and stop via Y-axis.
- Deadzone: Prevents jitter from small stick movements.
- Smooth steering: X-axis adjusts left/right motor speeds while driving.
- Pivot turning: In-place rotation using only X-axis.
- Added ±30 Y-axis noise for stability.
- I applied minimum PWM (minPWM + 30) to prevent stalling.

### Next step:
- Building my pan tilt that will hold the camera.
- Setting up two servos(sg90) with the pan tilt.
- For my servos I will need to setup buck regulator, because my L298N motor - driver will give out 11V and have to lower it down to 5V.
- Implement logic for steering servos with ps4 controller.
- And maybe implement logic for wifi and camera.

Picture/Video below:

https://github.com/user-attachments/assets/98faa8ca-5203-49f3-852d-0d0e68bfeed1

### Problem: 
- Like in Prototypes 2 and 3, sluggish motor control caused by outdated API usage.
- Continued conflicts with Arduino libraries and Bluepad32 integration.

### Solution: 
Switched to using the newer ledcAttach() (merged version of removed ledcSetup + ledcAttachPin) after finding migration docs from ESP32 Arduino 2.x to 3.0, which restored expected motor performance.

## Prototype 6 – Servo Pan Tilt Setup
I built my pan tilt for the camera and set up two SG90 servos powered through a buck regulator. All grounds (buck regulator, ESP32, battery, later the camera) share the same common ground.

### What was done:
- Constructed a mechanical pan tilt for camera mounting
- Connected two SG90 servos powered via a buck regulator to convert battery voltage down to 5V
- Shared a common ground across all components (battery, buck regulator, ESP32, camera)
- Integrated servo control using PS4 controller inputs with deadzone filtering

### Picture/Video below:
![Image](https://github.com/user-attachments/assets/8951795e-4900-4d01-a5f6-9cd30512eaa7)

https://github.com/user-attachments/assets/692a3758-78b6-447e-84f4-228ae45a82cc


### Problems:
- Power supply shared between motors and servos caused voltage dips and back-EMF noise, making one servo act erratically and motors sluggish
- Multiple servos damaged due to electrical interference
- Joystick Y-axis noisy, causing servo jitter
- Some servos were continuous 360° types instead of 180°, complicating control
- Joystick control range was not precise, leading to overshoot

### Solution:
- Separated power lines with a Y cable: one output to buck regulator for servos, another directly to motor driver for motors
- Implemented a deadzone filter on joystick input to reduce jitter
- Switched servo Y-axis control to use controller pads with incremental adjustments (+/- 10°), improving precision and stability
- Tested and replaced 360° servos with 180° servos

## Prototype 7 – ESP32-CAM Wi-Fi Streaming
I faced the challenge that the ESP32-CAM has no built-in USB for flashing, so I used an FTDI programmer. Initially, I borrowed the USB controller from my ESP32 WROOM to flash the ESP32-CAM by connecting grounds and power, swapping RX/TX, and controlling enable pins to enter programming mode.

### What was done:
- Used FTDI programmer to flash ESP32-CAM.
- Connected ESP32 WROOM’s USB controller pins to ESP32-CAM to facilitate flashing.
- Configured IO0 to GND for flashing mode and controlled resets.
- Set up Wi-Fi connection using custom wifi_handler.
- Developed simple HTTP web server showing live camera stream via multipart/x-mixed-replace MIME type for real-time updates.
- Tested streaming on multiple devices and browsers.

### Picture/Video below:
<img width="359" alt="Image" src="https://github.com/user-attachments/assets/fe9cd0d8-69ee-4c4a-b2a2-3a37b2c9f496" />

![Image](https://github.com/user-attachments/assets/ee26d418-685f-4798-99ea-e9ed6fbde4cc)

https://github.com/user-attachments/assets/b107ab88-bce2-4b9d-825a-a92f53a278f4

### Problems:
- Flashing ESP32-CAM without built-in USB was complex and error-prone.
- Had to manually manipulate IO0 and EN pins and swap RX/TX lines during programming.
- esp_http_server does not support HTTPS, limiting security.

### Solution:
- Acquired and used an FTDI programmer to simplify flashing.
- Understood and followed wiring and boot mode requirements carefully.
- Accepted HTTP-only streaming for now; planned HTTPS as a future improvement.

## Prototype 8 – Wi-Fi, MQTT, and Server Integration
I worked on integrating Wi-Fi, MQTT messaging, and server communication but faced memory (IRAM) issues due to multiple libraries and background tasks. There were conflicts between Arduino libraries and my custom wifi_handler.

### What was done:
- Removed conflicting Arduino Wi-Fi and DNS headers to use my own wifi_handler.
- Disabled unused SDK configurations to free IRAM.
- Set up Mosquitto MQTT broker on local PC; worked on publishing and subscribing with another ESP device.
- Configured Mosquitto with anonymous access and opened firewall ports.
- Implemented MQTT clients for alarm status messages and buzzer activation from a classmates alarmsystem.
- Separated Wi-Fi/MQTT and Bluetooth tasks on different CPU cores to avoid radio interference.

### Picture/Video below:
<img width="700" alt="Image" src="https://github.com/user-attachments/assets/27d5e17a-1d23-4f57-b85d-bbd324430692" />

### Problems:
- IRAM overflow caused by overlapping libraries and unnecessary background tasks.
- MQTT broker blocked by firewall and required configuration changes.
- Bluetooth and Wi-Fi interference led to lost MQTT messages and delayed logs.

### Solution:
- Removed conflicting Arduino libraries and disabled unused configs to free memory.
- Updated Mosquitto config to allow anonymous access and opened firewall ports.
- Split Bluetooth and Wi-Fi/MQTT into separate FreeRTOS tasks pinned to different CPU cores to prevent interference.

## All combined final product Picture/Video:
https://github.com/user-attachments/assets/75f41d0b-faed-450d-9148-06a95fff24d3


## Possible improvements and future developments
- Improved video resolution and streaming quality by using a Raspberry Pi and RTSP streaming instead of HTTP streaming on the ESP32-CAM
- Better wiring management and a larger, more stable chassis, as the current prototype shows room for mechanical and structural improvements
- Potential future expansions such as adding a robotic arm equipped with an IR sensor and incorporating AI/machine learning for autonomous features
- Due to limited time, I was not able to implement the AI and machine learning aspects, which were scoped out at the start of the project to keep the focus clear

## References
Random Tutorials, “ESP32 with DC Motor and L298N Motor Driver – Control Speed and Direction” — Guide for controlling DC motors with ESP32 and L298N driver.
https://randomnerdtutorials.com/esp32-dc-motor-l298n-motor-driver-control-speed-direction/

- Espressif Systems, ESP32-C6 Series Data Sheet — Technical specifications for ESP32-C6 microcontroller.
https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf

- Ricardo Quesada, Bluepad32 Supported Gamepads — Documentation of supported game controllers for Bluepad32 library.
https://bluepad32.readthedocs.io/en/latest/supported_gamepads/

- Espressif Systems, ESP32-WROOM-32E and ESP32-WROOM-32EU Datasheet — Datasheet for ESP32 WROOM modules.
https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf

- Espressif Systems, Arduino ESP32 Migration Guide 2.x to 3.0 — Migration instructions for ESP32 Arduino core APIs.
https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html

- Ricardo Quesada, esp-idf-arduino-bluepad32-template — GitHub repository with Bluepad32 ESP-IDF integration examples.
https://github.com/ricardoquesada/esp-idf-arduino-bluepad32-template

- Arduino Forum, Servo twitches heavily while DC motor is running — Discussion about servo interference caused by DC motors.
https://forum.arduino.cc/t/servo-twitches-heavily-while-dc-motor-is-running/232065/6

- Arduino Forum, Servo does not stop rotating — Discussion on continuous servo behavior and fixes.
https://forum.arduino.cc/t/servo-do-not-stop-rotating/643478/2

- Davy Wybiral, ESP32-CAM Programming without USB / Quickstart with Arduino — Video tutorial on flashing ESP32-CAM.
https://www.youtube.com/watch?v=Sb08leLWOgA&t=130s

- Handsontec, ESP32-CAM Datasheet — Technical specs for ESP32-CAM module.
https://www.handsontec.com/dataspecs/module/ESP32-CAM.pdf

- Random Tutorials, ESP32-CAM programming schematic — Wiring guide for ESP32-CAM flashing and programming.
https://randomnerdtutorials.com/program-upload-code-esp32-cam/

- Espressif Systems, Optimizing RAM Usage — Guide on managing IRAM and RAM on ESP32.
https://docs.espressif.com/projects/esp-idf/en/v5.2.2/esp32s3/api-guides/performance/ram-usage.html#optimizing-iram-usage

- Great Learning, Difference between C and C++ — Overview of C and C++ programming languages.
https://www.mygreatlearning.com/blog/difference-between-c-and-c/#what-is-c

- Espressif Systems, ESP-IDF Guide — Official guide to ESP-IDF development framework.
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#software

- Espressif Systems, Arduino as a Component — Documentation on using Arduino as component in ESP-IDF.
https://docs.espressif.com/projects/arduino-esp32/en/latest/esp-idf_component.html

- Espressif Systems, FreeRTOS Overview — Overview of FreeRTOS usage in ESP32.
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos.html

- madhephaest, ESP32Servo — GitHub repository for servo control library on ESP32.
https://github.com/madhephaestus/ESP32Servo

- Osama Kechida, GitHub/Readme.md File / Full Project — Complete robot project repository from hardware to logic.
https://github.com/Kechida98/my_robottank