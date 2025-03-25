/*This code was first created in Arduino Ide.
Prototype one was focus on getting motor A to move forward and backward,
check Readme.md file for Pictures.
*/

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
