/*This code was first created in Arduino Ide.
Prototype two was focus on getting motor A and B to move forward and backward,
check Readme.md file for Pictures.
*/
#include "sdkconfig.h"
#include <Arduino.h>
#include <Bluepad32.h>
// Motor A
int motor1Pin1 = 18; 
int motor1Pin2 = 5; 
int enable1Pin = 19; 

// Motor B
int motor2Pin3 = 25; 
int motor2Pin4 = 26;     
int enable2Pin = 27; 

// Setting PWM properties
const int freq = 15000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Console.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Console.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Console.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Console.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ()        // Accelerometer Z
    );
}

void processGamepad(ControllerPtr ctl) {
     // Query whether a button is pressed
    //  a(), b(), x(), y(), l1(), etc...
    const int DEADZONE_MOVE = 50;   // For the motor to run
    const int DEADZONE_STEER = 100; // Minimally value to steer
    const int minPWM = 70;
    const int maxPWM = 255;

    int x = ctl->axisX();
    int y = ctl->axisY();

    if (ctl->a()){
        static int colorIdx = 0;

        switch (colorIdx % 3){
        case 0:
            // Red
            ctl->setColorLED(255, 0, 0);
            break;
        case 1:
            // Green
            ctl->setColorLED(0, 255, 0);
            break;
        case 2:
            // Blue
            ctl->setColorLED(0, 0, 255);
            break;
        }
        colorIdx++;
    }

    if (ctl->b()){
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        ctl->setPlayerLEDs(led & 0x0f);
    }
    if (ctl->x()) {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
        ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
                            0x40 /* strongMagnitude */);
    }
    dumpGamepad(ctl); //Debug

    // 🛑 Stopp
    if (hypot(x, y) < DEADZONE_MOVE) {
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin3, LOW);
        digitalWrite(motor2Pin4, LOW);
        ledcWrite(enable1Pin, 0);
        ledcWrite(enable2Pin, 0);
        Console.println("🛑 Stopp");
        return;
    }

    // 🌀 Pivot ,only X-movement
       // 🌀 Pivot ,stopp  + steering
    if (abs(y) < DEADZONE_MOVE + 30 && abs(x) > DEADZONE_STEER) {  
        //Adjustment: allowing some Y-axis noise  ±80

        int spinSpeed = map(abs(x), 0, 512, minPWM + 30, maxPWM);  // More speed in pivot

        // Failsafe – Have to make sure that spinspeed is never to low
        if (spinSpeed < minPWM + 30) spinSpeed = minPWM + 30;

        if (x > 0) {
            // Steering right
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin3, LOW);
            digitalWrite(motor2Pin4, HIGH);
        } else {
            // Steering left
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            digitalWrite(motor2Pin3, HIGH);
            digitalWrite(motor2Pin4, LOW);
        }

        ledcWrite(enable1Pin, spinSpeed);
        ledcWrite(enable2Pin, spinSpeed);
        Console.printf("↩️ Pivot - Speed: %d\n", spinSpeed);
        return;
    }

    //🚘 Drivie mechanic + steering
    int speed = map(abs(y), 0, 512, minPWM, maxPWM);
    int steer = (abs(x) > DEADZONE_STEER) ? map(x, -512, 512, -speed, speed) : 0;

    int leftSpeed = constrain(speed - steer, 0, maxPWM);
    int rightSpeed = constrain(speed + steer, 0, maxPWM);

    if (y < -DEADZONE_MOVE) {
        // Forward
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin3, LOW);
        digitalWrite(motor2Pin4, HIGH);
        Console.printf("Forward - PWM: %d\n", speed);
    } else if (y > DEADZONE_MOVE) {
        // Backward
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin3, HIGH);
        digitalWrite(motor2Pin4, LOW);
        Console.printf("Backward - PWM: %d\n", speed);
    }

    ledcWrite(enable1Pin, leftSpeed);
    ledcWrite(enable2Pin, rightSpeed);

    Console.printf("🚘 Drive - L:%d R:%d\n", leftSpeed, rightSpeed);
}

void processControllers(){
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()){
                processGamepad(myController);
            }
        }
    }
}

void setup() {
      // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LEDC PWM
    ledcAttach(enable1Pin, freq, resolution);
    ledcAttach(enable2Pin, freq, resolution);

    Console.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Console.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

    //"Start scanning" will try to auto-connect to devices that are compatible with Bluepad32.
    // E.g: if a Gamepad, keyboard or mouse are detected, it will try to auto connect to them.
    // Initialize Bluepad32 with callbacks and start scanning
    bool startScanning = true;
    BP32.setup(&onConnectedController, &onDisconnectedController, startScanning);

    // Optional: clear paired Bluetooth devices
    BP32.forgetBluetoothKeys();
     // Optional: enable BLE service (e.g. for apps like LightBlue) change to "true" if i want it activated
    BP32.enableBLEService(false);
  
}

void loop() {

    // This call fetches all the controllers' data.
    bool dataUpdated = BP32.update();
    if (dataUpdated)
        processControllers();

    //change to vTaskDelay when am using realtimesteering
    vTaskDelay(1);
    //delay(150);
}
