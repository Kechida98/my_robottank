
## Prototype 1
In prototype 1 I tried to to run one dc motor to see if it works good runing it forward and backward with pulse with modulation=pwm.

Components used in Prototype 1:

MCU: ESP32-C6/ will be changed to esp32 wroom 32e for bluedroid/bluetooth classic later. Because Esp32-C6 is only compatible with BLE = Bluetooth low energy and wroom can do both.
Motor Driver: L298N
Power: 2x 3.7V Li-ion batteries with holder
one DC Motor with one plastic wheel
Jumper cables
Motor A was tested for forward and backward movement using PWM.
Picture/Video below:
![Motor setup](https://github.com/user-attachments/assets/0ee76e86-15a2-43bc-93b2-a7ba8820f6ca)


## Prototype 2
In Prototype 2, I focused on running two DC motors simultaneously to test synchronized motion — making sure they start and stop together properly.

I also bought the kit chassi to improve structure, stability, and overall design. Well it was either build my own chassi, wich i would do if i had a 3d printer or material to craft one. This helps with both functionality and future expansion (like mounting sensors, battery, and ESP board).

Components added:
1x Dc motors 12v
2x bearing wheels
2x drive wheels
1x chassi
2x plastic tracks
and some more jumper cables


Picture/Video below:

![Image](https://github.com/user-attachments/assets/8655fdf0-c675-4677-bba9-76596d624ec1)

![Image](https://github.com/user-attachments/assets/f9de452d-4bb6-4732-a826-fda9fbc57b1c)

![Image](https://github.com/user-attachments/assets/cef2f348-9651-4c71-ade7-fe4c406b4a01)


## Prototype 3

In Prototype 3, I tested left and right movement, as well as forward and backward — all **without powering the microcontroller from my laptop**.  
This means the robot runs fully on battery power, which is a step closer to going wireless.  
However, to continue safely, I’ll need to **solder, tape, or glue** the components so they don’t get damaged during further testing.  
I’m saving that part for when I start working with **Bluetooth control** for the robot — **unless I decide to start with Bluetooth right away**.  
For now, I’ll continue powering the microcontroller externally.

Picture/Video below:
![Image](https://github.com/user-attachments/assets/44a64f3d-078d-4d90-97d8-be7de222b782)

## Prototype 4 – PS4 Controller + ESP32 WROOM-32

In this prototype, I connected a PS4 controller to an ESP32 WROOM-32 via Bluetooth. My previous ESP board wasn’t compatible with Bluedroid, so I switched to the WROOM-32.

I’ve been using Ricardo Quesada’s **Bluepad32.h** library to read input from the controller, including joysticks, buttons, R2 (throttle), and L2 (brake).

### Features:
- Bluetooth connection with PS4 controller
- Joystick and button input reading
- Console debug using Bluepad32
- Ready for motor control with PWM
- Control 2 DC motors using joystick and triggers


Extra testing:
I also tested a version of the code that runs the DC motors without using the PS4 controller, just to verify that PWM and motor wiring worked properly with the ESP32 WROOM-32E.
At this point, I’ve also mounted the main components on the robot (not perfectly), mainly to confirm that everything is functional and fits together.

Picture/Video below:

![Image](https://github.com/user-attachments/assets/48a90633-8d2c-4bc6-a11b-93db7a30fcfe)

![Image](https://github.com/user-attachments/assets/8166ed5d-1440-46b8-b354-8f1784d7b184)


## Prototype 5 – PS4 Controller Drive Logic & Pivot Support + ESP32 WROOM-32

In this prototype, I implemented full drive control logic using a PS4 controller connected to an ESP32 WROOM-32 via Bluetooth, using the Bluepad32 library. The goal was smoother driving and adding support for pivoting turning in place.

What was done:
- Bluetooth connection with PS4 controller using Bluepad32.
- Joystick and button input handling (X, Y, A, B, R2, L2).
- Console debugging using Console.printf(), for better realtime feedback.
- Motor control using PWM via ledcWrite() and automatic ledcAttach().
- Directional control using L298N motor driver.

Drive Logic Overview:
- Movement: Forward, backward, and stop via Y-axis.
- Deadzone: Prevents jitter from small stick movements.
- Smooth steering: X-axis adjusts left/right motor speeds while driving.
- Pivot turning: In-place rotation using only X-axis.
- Added ±30 Y-axis noise for stability.
- I applied minimum PWM (minPWM + 30) to prevent stalling.

### Next step:
- Building my pan tilt that will hold the camera.
- Setting up two servos(sg90) with the pan tilt.
- For my servos I will need to setup buck regulator, because my L298N motor driver will give out 11V and have to lower it down to 5V.
- Implement logic for steering servos with ps4 controller.
- And maybe implement logic for wifi and camera.

Picture/Video below:
![Image](https://github.com/user-attachments/assets/98faa8ca-5203-49f3-852d-0d0e68bfeed1)