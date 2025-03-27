
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
![Motor setup](https://github.com/user-attachments/assets/0f29f549-6c30-4501-aa57-32d214b5d122)


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
