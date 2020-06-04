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

Servo LServo;
Servo RServo;

IRrecv irrecv(2);
decode_results results;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);

    Serial.println("Enabling IRin");
    irrecv.enableIRIn();
    Serial.println("Enabled IRin");

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    LServo.attach(MOTORL_PIN);
    RServo.attach(MOTORR_PIN);
}
    
int leftStandard = 93;
int rightStandard = 90;

void TurnLeft(int speed) {
    LServo.write(leftStandard - speed); // +가 앞 -가 뒤
    RServo.write(rightStandard - speed); // -가 앞 +가 뒤
}

void TurnRight(int speed) {
    LServo.write(leftStandard + speed); // +가 앞 -가 뒤
    RServo.write(rightStandard + speed); // -가 앞 +가 뒤
}

void goForward(int speed) {
    LServo.write(leftStandard + speed); // +가 앞 -가 뒤
    RServo.write(rightStandard - speed); // -가 앞 +가 뒤
}

// the loop function runs over and over again until power down or reset
void loop() {
static int speed = 5;
    if (irrecv.decode(&results)) {
        int count = results.rawlen;

        if (results.decode_type == UNKNOWN) {
            Serial.print("UNKNOWN : ");
        }
        else if (results.decode_type == NEC)
        {
            Serial.print("Decoded NEC : ");
        }
        else if (results.decode_type == SONY) {
            Serial.print("Decoded SONY : ");
        }
        else if (results.decode_type == RC5) {
            Serial.print("Decoded RC5 : ");
        }
        else if (results.decode_type == RC6) {
            Serial.print("Decoded RC6 : ");
        }
        else if (results.decode_type == PANASONIC) {
            Serial.print("Decoded PANASONIC : ");
        }
        else if (results.decode_type == LG) {
            Serial.print("Decoded LG : ");
        }
        else if (results.decode_type == JVC) {
            Serial.print("Decoded JVC : ");
        }

        Serial.print(results.value, HEX);
        Serial.print("(");
        Serial.print(results.bits, DEC);
        Serial.println("bits)");

        Serial.print("Raw (");
        Serial.print(count, DEC);
        Serial.println(")");

        int standard = 85;

        switch (results.value)
        {
        case 0xFF02FD: goForward(speed); delay(30); break;
        case 0xFF9867: goForward(-(speed)); delay(30); break;
        case 0xFF906F: TurnLeft(0.5*speed); delay(30); break;
        case 0xFFE01F: TurnRight(0.5*speed); delay(30); break;
        case 0xFFA857: goForward(0); delay(30); break;

        default:
            break;
        }

        irrecv.resume();
    }
}
