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

long duration, cm, in;

Servo LServo;
Servo RServo;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    goForward();

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    LServo.attach(MOTORL_PIN);
    RServo.attach(MOTORR_PIN);
}

int leftSpeed = 1460; // +가 앞 -가 뒤
int rightSpeed = 1460; // -가 앞 +가 뒤

// the loop function runs over and over again until power down or reset
void loop() {
    int distance;
    int sonarPwn;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    sonarPwn = pulseIn(ECHO_PIN, 1, 18500L);
    distance = sonarPwn / 60;

    Serial.print("distance: ");
    Serial.println(distance);


    if (distance < 10) {
        RServo.write(0);
        LServo.write(0);
        delay(800);
    }
    else {
        RServo.write(30);
        LServo.write(120);
    }
    Serial.print("right: ");
    Serial.println(rightSpeed);
    Serial.print("left: ");
    Serial.println(leftSpeed);
}

void goForward() {
    digitalWrite(MOTORL_PIN, HIGH);
    delayMicroseconds(1470 + 500);
    digitalWrite(MOTORL_PIN, LOW);

    digitalWrite(MOTORR_PIN, HIGH);
    delayMicroseconds(1470 - 500);
    digitalWrite(MOTORR_PIN, LOW);
}

void Backward() {
    digitalWrite(MOTORL_PIN, HIGH);
    delayMicroseconds(1470 - 500);
    digitalWrite(MOTORL_PIN, LOW);

    digitalWrite(MOTORR_PIN, HIGH);
    delayMicroseconds(1470 + 500);
    digitalWrite(MOTORR_PIN, LOW);
}

void turnRight() {
    digitalWrite(MOTORL_PIN, HIGH);
    delayMicroseconds(1470 + 500);
    digitalWrite(MOTORL_PIN, LOW);
    digitalWrite(MOTORR_PIN, HIGH);
    delayMicroseconds(1470 + 500);
    digitalWrite(MOTORR_PIN, LOW);
}