/*
 Name:		QTI.ino
 Created:	5/28/2020 11:17:59 AM
 Author:	tomor
*/

// the setup function runs once when you press reset or power the board
#include <Servo.h>

int qtiL = 11;
int qtiR = 12;

int MOTORL_PIN = 5;
int MOTORR_PIN = 9;

int TRIG_PIN = 3;
int ECHO_PIN = 4;

Servo LServo;
Servo RServo;

void setup() {
    Serial.begin(9600);
    pinMode(qtiL, INPUT);
    pinMode(qtiR, INPUT);
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    LServo.attach(MOTORL_PIN);
    RServo.attach(MOTORR_PIN);
}

int SonarDistance() {
    int sonarPwn;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    sonarPwn = pulseIn(ECHO_PIN, 1, 18500L);
    return sonarPwn / 60;
}

void TurnLeft(int standard, int speed) {
    LServo.write(standard - speed); // +front -back
    RServo.write(standard - speed); // -front +back
}

void TurnRight(int standard, int speed) {
    LServo.write(standard + speed); // +front -back
    RServo.write(standard + speed); // -front +back
}

void goForward(int standard, int speed) {
    LServo.write(standard + speed); // +front -back
    RServo.write(standard - speed); // -front +back
}

// the loop function runs over and over again until power down or reset
void loop() {
    int LeftQti = digitalRead(qtiL);    // black 1 else 0
    int RightQti = digitalRead(qtiR);

    int distance = SonarDistance();
    
    Serial.println(LeftQti);
    Serial.println(RightQti);

    int standard = 92;

    Serial.print("distance : ");
    Serial.println(distance);

    if (distance > 3 && distance < 8) {
        TurnLeft(standard, 35);
        delay(840);
    }
    else if (LeftQti && RightQti) {
        goForward(standard, 0);
    }
    else if (LeftQti) {
        TurnLeft(standard, 4);
        delay(3);
    }
    else if (RightQti) {
        TurnRight(standard, 3);
        delay(3);
    }
    else
    {
        goForward(standard, 10);
    }
}
