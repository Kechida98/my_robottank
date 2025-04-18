// ⚙️ LEDC PWM – automatiskt kanalval (rekommenderat i nyare ESP-IDF)
   ändra resolution till 8
   
   
   fungerar lite bättre än innan
    void processGamepad(ControllerPtr ctl) {
    const int DEADZONE = 50;
    const int minPWM = 70;  // Lägsta PWM så motorn orkar röra sig
    const int maxPWM = (1 << resolution) - 1;

    int x = ctl->axisX();  // Vänster/Höger
    int y = ctl->axisY();  // Framåt/Bakåt

    // 🎮 LED-effekter
    if (ctl->a()) {
        static int colorIdx = 0;
        switch (colorIdx++ % 3) {
            case 0: ctl->setColorLED(255, 0, 0); break;
            case 1: ctl->setColorLED(0, 255, 0); break;
            case 2: ctl->setColorLED(0, 0, 255); break;
        }
    }

    if (ctl->b()) {
        static int led = 0;
        ctl->setPlayerLEDs(++led & 0x0F);
    }

    if (ctl->x()) {
        ctl->playDualRumble(0, 250, 0x80, 0x40);
    }

    dumpGamepad(ctl);  // 🧪 Debug

    // 🛑 Stillastående
    if (abs(x) < DEADZONE && abs(y) < DEADZONE) {
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin3, LOW);
        digitalWrite(motor2Pin4, LOW);
        ledcWrite(enable1Pin, 0);
        ledcWrite(enable2Pin, 0);
        Console.println("🛑 Stopp");
        return;
    }

    // 🚗 Fram/Bak styrning
    int baseSpeed = map(abs(y), 0, 512, minPWM, maxPWM);
    int turnAdjust = map(x, -512, 512, -baseSpeed, baseSpeed);
    int leftSpeed = constrain(baseSpeed - turnAdjust, 0, maxPWM);
    int rightSpeed = constrain(baseSpeed + turnAdjust, 0, maxPWM);

    if (y < -DEADZONE) {
        // Framåt
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin3, LOW);
        digitalWrite(motor2Pin4, HIGH);
        ledcWrite(enable1Pin, leftSpeed);
        ledcWrite(enable2Pin, rightSpeed);
        Console.printf("🚗 Framåt – L:%d R:%d\n", leftSpeed, rightSpeed);
    } 
    else if (y > DEADZONE) {
        // Bakåt
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin3, HIGH);
        digitalWrite(motor2Pin4, LOW);
        ledcWrite(enable1Pin, leftSpeed);
        ledcWrite(enable2Pin, rightSpeed);
        Console.printf("⛽ Bakåt – L:%d R:%d\n", leftSpeed, rightSpeed);
    } 
    else {
        // Pivot (sväng på plats)
        int spinSpeed = map(abs(x), 0, 512, minPWM, maxPWM);
        if (x > 0) {
            // Höger sväng
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin3, LOW);
            digitalWrite(motor2Pin4, HIGH);
        } else {
            // Vänster sväng
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            digitalWrite(motor2Pin3, HIGH);
            digitalWrite(motor2Pin4, LOW);
        }
        ledcWrite(enable1Pin, spinSpeed);
        ledcWrite(enable2Pin, spinSpeed);
        Console.printf("↩️ Pivot – Speed: %d\n", spinSpeed);
    }
}

    
    ändra i setup
    ledcAttach(enable1Pin, freq, resolution);
    ledcAttach(enable2Pin, freq, resolution);











// Setting PWM properties
const int freq = 13000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

    funka mycket bra 

    void processGamepad(ControllerPtr ctl) {
    const int DEADZONE_MOVE = 50;   // För att motorn ska röra sig
    const int DEADZONE_STEER = 100; // För att svänga (minimalt utslag)
    const int minPWM = 70;
    const int maxPWM = 255;

    int x = ctl->axisX();
    int y = ctl->axisY();

    // 🎮 Extra effekter
    if (ctl->a()) {
        static int colorIdx = 0;
        switch (colorIdx++ % 3) {
            case 0: ctl->setColorLED(255, 0, 0); break;
            case 1: ctl->setColorLED(0, 255, 0); break;
            case 2: ctl->setColorLED(0, 0, 255); break;
        }
    }

    if (ctl->b()) {
        static int led = 0;
        ctl->setPlayerLEDs(++led & 0x0F);
    }

    if (ctl->x()) {
        ctl->playDualRumble(0, 250, 0x80, 0x40);
    }

    dumpGamepad(ctl);  // Debug

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

    // 🌀 Pivot (bara X-rörelse)
       // 🌀 Pivot (stå still + sväng)
    if (abs(y) < DEADZONE_MOVE + 30 && abs(x) > DEADZONE_STEER) {  
        // Justering: Tillåt lite Y-brus (t.ex ±80)

        int spinSpeed = map(abs(x), 0, 512, minPWM + 30, maxPWM);  // Mer fart i pivot

        // Failsafe – se till att spinSpeed aldrig blir för låg
        if (spinSpeed < minPWM + 30) spinSpeed = minPWM + 30;

        if (x > 0) {
            // Sväng höger
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            digitalWrite(motor2Pin3, LOW);
            digitalWrite(motor2Pin4, HIGH);
        } else {
            // Sväng vänster
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            digitalWrite(motor2Pin3, HIGH);
            digitalWrite(motor2Pin4, LOW);
        }

        ledcWrite(enable1Pin, spinSpeed);
        ledcWrite(enable2Pin, spinSpeed);
        Console.printf("↩️ Pivot – Speed: %d\n", spinSpeed);
        return;
    }


    // 🚘 Kör + Styr
    int speed = map(abs(y), 0, 512, minPWM, maxPWM);
    int steer = (abs(x) > DEADZONE_STEER) ? map(x, -512, 512, -speed, speed) : 0;

    int leftSpeed = constrain(speed - steer, 0, maxPWM);
    int rightSpeed = constrain(speed + steer, 0, maxPWM);

    if (y < -DEADZONE_MOVE) {
        // Framåt
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin3, LOW);
        digitalWrite(motor2Pin4, HIGH);
    } else if (y > DEADZONE_MOVE) {
        // Bakåt
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin3, HIGH);
        digitalWrite(motor2Pin4, LOW);
    }

    ledcWrite(enable1Pin, leftSpeed);
    ledcWrite(enable2Pin, rightSpeed);
    Console.printf("🚗 Kör – L:%d R:%d\n", leftSpeed, rightSpeed);
}



 // configure LEDC PWM
    ledcAttach(enable1Pin, freq, resolution);
    ledcAttach(enable2Pin, freq, resolution);