/*
 Name:		IRremote.ino
 Created:	6/4/2020 11:31:41 AM
 Author:	tomor
*/

#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremoteInt.h>
#include <IRremote.h>
#include <boarddefs.h>
#include <Servo.h>

int MOTORL_PIN = 5;
int MOTORR_PIN = 9;
int TRIG_PIN = 3;
int ECHO_PIN = 4;

#define qtiL 11
#define qtiR 12

Servo LServo;
Servo RServo;

IRrecv irrecv(2);
decode_results results;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    pinMode(qtiL, INPUT);
    pinMode(qtiR, INPUT);

    Serial.println("Enabling IRin");
    irrecv.enableIRIn();
    Serial.println("Enabled IRin");

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

int leftStandard = 90;
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
int mode = 1;
void loop() {
    int speed = 15;

    if (Serial.available()) {
        char value = Serial.read();
        Serial.println(value);


        switch (value)
        {
        case '1':
            TurnLeft(0.5 * speed); break;
        case '2':
            goForward(speed); break;
        case '3':
            TurnRight(0.5 * speed); break;
        case '4':
            goForward(-speed); break;
        case '5':
            goForward(0); break;
        case '6':
            mode = mode ? 0 : 1; break;
        default:
            break;
        }
    }

    if (irrecv.decode(&results)) {
        int count = results.rawlen;

        switch (results.value)
        {
        case 0xFF02FD: goForward(speed); delay(30); break;
        case 0xFF9867: goForward(-(speed)); delay(30); break;
        case 0xFF906F: TurnRight(0.5 * speed); delay(30); break;
        case 0xFFE01F: TurnLeft(0.5 * speed); delay(30); break; 
        case 0xFFA857: goForward(0); delay(30); break;
        case 0xFF22DD: mode = mode==1?0:1;
                
        default:
            break;
        }

        irrecv.resume();
    }

    if (mode==0) {
        int LeftQti = digitalRead(qtiL);    // black 1 else 0
        int RightQti = digitalRead(qtiR);

        int distance = SonarDistance();

        Serial.println(LeftQti);
        Serial.println(RightQti);

        int standard = 90;

        Serial.print("distance : ");
        Serial.println(distance);

        if (LeftQti) {
            TurnLeft(speed-5);
            delay(3);
        }
        else if (RightQti) {
            TurnRight(speed-5);
            delay(3);
        }
        else
        {
            goForward(speed);
        }
    }
}