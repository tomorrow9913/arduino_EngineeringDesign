/*
 Name:		Bluetooth.ino
 Created:	6/11/2020 11:18:08 AM
 Author:	tomor
*/

// the setup function runs once when you press reset or power the board
#include <Servo.h>

int MOTORL_PIN = 5;
int MOTORR_PIN = 9;
int TRIG_PIN = 3;
int ECHO_PIN = 4;

Servo LServo;
Servo RServo;

void setup() {
    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    LServo.attach(MOTORL_PIN);
    RServo.attach(MOTORR_PIN);
}

int leftStandard = 93;
int rightStandard = 90;

void TurnLeft(int speed) {
    LServo.write(leftStandard - speed); // +?? ?? -?? ??
    RServo.write(rightStandard - speed); // -?? ?? +?? ??
}

void TurnRight(int speed) {
    LServo.write(leftStandard + speed); // +?? ?? -?? ??
    RServo.write(rightStandard + speed); // -?? ?? +?? ??
}

void goForward(int speed) {
    LServo.write(leftStandard + speed); // +?? ?? -?? ??
    RServo.write(rightStandard - speed); // -?? ?? +?? ??
}

// the loop function runs over and over again until power down or reset
void loop() {
    const int speed = 5;
    if (Serial.available()) {
        char value = Serial.read();
        Serial.print("help ");
        Serial.println(value);

        
        switch (value)
        {
        case '1':
            TurnLeft(speed); break;
        case '2':
            goForward(speed); break;
        case '3':
            TurnRight(speed); break;
        case '4':
            goForward(-speed); break;
        case '5':
            goForward(0); break;
        default:
            break;
        }
    }
}
