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

void loop() {
 // Move the DC motor forward at maximum speed
  Serial.println("Motor A: Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  ledcWrite(pwmChannel1, dutyCycle);
  delay(2000);

  // Stop Motor 
  Serial.println("Motor A: Stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel1, 0);
  delay(1000);

  // Move Motor backward at maximum speed
  Serial.println("Motor A: Moving Backward");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel1, dutyCycle);
  delay(2000);

  // Stop Motor 
  Serial.println("Motor A: Stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel1, 0);
  delay(1000);

  // Move Motor forward with increasing speed
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  while (dutyCycle <= 255) {
    ledcWrite(pwmChannel1, dutyCycle);
    Serial.print("Motor A: Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle += 5;
    delay(500);
  }

  // Reset duty cycle for next loop
  dutyCycle = 200;
}
