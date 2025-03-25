/*This code was first created in Arduino Ide.
Prototype one was focus on getting motor A to move forward and backward,
check Readme.md file for Pictures.
*/
#include <Arduino.h>
// Motor A
int motor1Pin1 = 21; 
int motor1Pin2 = 20; 
int enable1Pin = 22; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int resolution = 8;
int dutyCycle = 200;

void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure LEDC PWM
  ledcAttachChannel(enable1Pin, freq, resolution, pwmChannel1);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
}
