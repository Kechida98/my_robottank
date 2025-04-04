/*This code was first created in Arduino Ide.
Prototype two was focus on getting motor A and B to move forward and backward,
check Readme.md file for Pictures.
*/
#include "sdkconfig.h"
#include <Arduino.h>
#include <Bluepad32.h>
// Motor A
int motor1Pin1 = 21; 
int motor1Pin2 = 20; 
int enable1Pin = 22; 

int motor2Pin3 = 2; 
int motor2Pin4 = 3;     
int enable2Pin = 4; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;
int dutyCycle = 200;

ControllerPtr myController [BP32_MAX_GAMEPADS];

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

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin3, OUTPUT);
  pinMode(motor2Pin4, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel1);
  ledcAttachChannel(enable2Pin, freq, resolution, pwmChannel2);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
}

void loop() {
  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, HIGH);
  ledcWrite(pwmChannel1, dutyCycle);
  ledcWrite(pwmChannel2, dutyCycle);
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, LOW);
  ledcWrite(pwmChannel1, 0);
  ledcWrite(pwmChannel2, 0);
  delay(1000);

  //Turn Right (Motor A forward and B Stop)
  Serial.println("Pivot Right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, LOW);
  ledcWrite(pwmChannel1, dutyCycle);
  ledcWrite(pwmChannel2, 0);
  delay(1000);

  //Stop
  Serial.println("Stop")
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, LOW);
  ledcWrite(pwmChannel1, 0);
  ledcWrite(pwmChannel2, 0);
  delay(1000);

  //Opposite
  Serial.println("Pivot Left");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, HIGH);
  ledcWrite(pwmChannel1, 0);
  ledcWrite(pwmChannel2, dutyCycle);
  delay(1000);

  //Stop
  Serial.println("Stop")
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, LOW);
  ledcWrite(pwmChannel1, 0);
  ledcWrite(pwmChannel2, 0);
  delay(1000);

  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin3, HIGH);
  digitalWrite(motor2Pin4, LOW);

  ledcWrite(pwmChannel1, dutyCycle);
  ledcWrite(pwmChannel2, dutyCycle);
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, LOW);

  ledcWrite(pwmChannel1, 0);
  ledcWrite(pwmChannel2, 0);
  delay(1000);


  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin3, LOW);
  digitalWrite(motor2Pin4, HIGH);
  while (dutyCycle <= 255){
    ledcWrite(enable1Pin, dutyCycle);
    ledcWrite(enable2Pin, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
}
