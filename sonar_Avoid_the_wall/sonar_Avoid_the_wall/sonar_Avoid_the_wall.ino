/*
 Name:		sonar_Avoid_the_wall.ino
 Created:	5/22/2020 8:14:01 PM
 Author:	tomor
*/

#include <Servo.h>

int MOTORL_PIN = 5;
int MOTORR_PIN = 9;
int TRIG_PIN = 3;
int ECHO_PIN = 4;

Servo LServo;
Servo RServo;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

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
    LServo.write(standard - speed); // +가 앞 -가 뒤
    RServo.write(standard - speed); // -가 앞 +가 뒤
}

void TurnRight(int standard, int speed) {
    LServo.write(standard + speed); // +가 앞 -가 뒤
    RServo.write(standard + speed); // -가 앞 +가 뒤
}

void goForward(int standard, int speed) {
    LServo.write(standard + speed); // +가 앞 -가 뒤
    RServo.write(standard - speed); // -가 앞 +가 뒤
}

int flag = 0;
int speed = 30;
int leftSpeed = 1460; // +가 앞 -가 뒤
int rightSpeed = 1460; // -가 앞 +가 뒤

// the loop function runs over and over again until power down or reset
void loop() {
    int standard = 92;
    int distance = 11;
    distance = SonarDistance();

    Serial.print("distance: ");
    Serial.println(distance);

    if (distance && distance <= 10) {    
        TurnLeft(standard, speed);
        delay(840);
        flag = 1;
    }
    else {
        goForward(standard, speed);
        if (flag) {
            delay(2000);
            speed = 0;
        }
    }
    
    
    Serial.print("flag: ");
    Serial.println(flag);
}