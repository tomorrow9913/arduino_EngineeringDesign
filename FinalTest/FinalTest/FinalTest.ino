/****************************************************************
 * File Name:   Final.ino
 *
 * Description: ������ ���� ���� RC���� �ڵ�
 * Created:	6/18/2020 11:31:41 AM
 * Author:	Jeong MinGyu
 *
 * Extern File
 * Name                             Description
 * ================================ ============================
 * boarddefs.h                      IR ���� ���
 * ir_Lego_PF_BitStreamEncoder.h    IR ���� ���
 * IRremoteInt.h                    IR ���� ���
 * IRremote.h                       IR ���� ���
 * Servo.h                          ���� ����(����) ����
 *
 * Extern Variables
 * Name         Type        Description
 * ============ =========== ====================================
 * None
 *
 * Global Variables
 * Name         Type        Description
 * ============ =========== ====================================
 * Dir          enum        ���� ����
 * leftStandard int         ���� ���� ���� ��ġ
 * rightStandard int        ���� ���� ���� ��ġ
 * mode         int         ���� ���(1-�������� 0-��������)
 ****************************************************************/

#include <ir_Lego_PF_BitStreamEncoder.h>        
#include <IRremoteInt.h>                        
#include <IRremote.h>                           
#include <boarddefs.h>                          

#include <Servo.h>                              

// ���ͱ��� ��
#define MOTORL_PIN 5
#define MOTORR_PIN 9

// ������ ����
#define TRIG_PIN 3
#define ECHO_PIN 4

// QTI����
#define qtiL 11
#define qtiR 12

// ������ ����
#define FRONT 0xFF02FD
#define BACK 0xFF9867
#define RIGHT 0xFF906F
#define LEFT 0xFFE01F
#define STOP 0xFFA857
#define MODE 0xFF22DD

// ���� ����
Servo LServo;
Servo RServo;

// IR����
IRrecv irrecv(2);
decode_results results;

// ����
enum Dir
{
    forward = 0,
    backward = 1,
    left = 2,
    right = 3
};

int leftStandard = 90;      // ���� ���� ���� ��
int rightStandard = 90;     // ���� ���� ���� ��
int mode = 1;               // ���� ��� (1-�������� 0-��������)

/****************************************************************
* Function Name: setup
* Description:     
* the setup function runs once when you press reset or power the board
*
* @return: void
* @param: void
* Name         Type        Description
* ============ =========== ====================================
*
* Author:       Jeong MinGyu
* Creat Date:   2020.06.18.
* Last Edit:    2020.06.18.
*
* Global Variables
* Name         Type        Description
* ============ =========== ====================================
* None
*
* Algorithm :
***************************************************************/
void setup() {
    Serial.begin(9600);

    // QTI ����
    pinMode(qtiL, INPUT);
    pinMode(qtiR, INPUT);

    // IR ����
    Serial.println("Enabling IRin");
    irrecv.enableIRIn();
    Serial.println("Enabled IRin");

    // ������ ����
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // ���� ����
    LServo.attach(MOTORL_PIN);
    RServo.attach(MOTORR_PIN);
}

/****************************************************************
* Function Name: SonarDistance
* Description:
* ������ �Ÿ��� ������ ������ ���� �����Ѵ�.
*
* @return:  int (������ �Ÿ�)
* @param:   void
* Name         Type        Description
* ============ =========== ====================================
*
* Author:       Jeong MinGyu
* Creat Date:   2020.06.18.
* Last Edit:    2020.06.18.
*
* Global Variables
* Name         Type        Description
* ============ =========== ====================================
* None
*
* Algorithm :
***************************************************************/
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

/****************************************************************
* Function Name: Move
* Description:
* ��ü�� �̵��ϵ��� ������ �����մϴ�.
*
* @return:  void
* @param:   int
* Name         Type        Description
* ============ =========== ====================================
* speed        int          ���� Ŭ���� �̵�(��ȯ) �ӵ� ����
* mode         Dir          �̵� ������ �����մϴ�
*
* Author:       Jeong MinGyu
* Creat Date:   2020.06.18.
* Last Edit:    2020.06.18.
*
* Global Variables
* Name         Type        Description
* ============ =========== ====================================
* leftStandard  int         ���� ������ ���ߴ� ��
* rightStandard int         ���� ������ �����ϴ� ��
*
* Algorithm :
* num   dir	��	��  %3   %2
* 0	    ��	+	-   0    0
* 1     ��	-	+   1    1
* 2	    ��	-	-   2    0
* 3	    ��	+	+   0    1
***************************************************************/
void Move(int speed, Dir mode) {
    int LSpeed = mode % 3 ? -speed : speed;
    int RSpeed = mode % 2 ? speed : -speed;

    LServo.write(leftStandard - speed);     // +is Front -is Back
    RServo.write(rightStandard - speed);    // -is Front  +is Back
}

/****************************************************************
* Function Name: loop
* Description:
* the loop function runs over and over again until power down or reset
*
* @return:  void
* @param:   void
* Name         Type        Description
* ============ =========== ====================================
*
* Author:       Jeong MinGyu
* Creat Date:   2020.06.18.
* Last Edit:    2020.06.18.
*
* Global Variables
* Name         Type        Description
* ============ =========== ====================================
* mode         int         ���� ���(1-�������� 0-��������): 1�� �⺻��
*
* Algorithm :
***************************************************************/
void loop() {
    // ���۽� �⺻ �ӵ�
    int speed = 15;

    // ������� ����
    if (Serial.available()) {
        char value = Serial.read();
        Serial.println(value);

        switch (value)
        {
        //ȸ���� ������ ���ϵ��� �ӵ� �� ��������
        case '1':
            Move(0.5 * speed, left); break;
        case '2':
            Move(speed, forward); break;
        case '3':
            Move(0.5 * speed, right); break;
        case '4':
            Move(speed, backward); break;
        case '5':
            Move(0, forward); break;
        
        // ��� ���� ���
        case '6':
            mode = mode ? 0 : 1; break;
        default:
            break;
        }
    }

    // IR ���� ����
    if (irrecv.decode(&results)) {
        int count = results.rawlen;

        switch (results.value)
        {
        case FRONT: Move(speed, forward); delay(30); break;
        case BACK: Move(speed, backward); delay(30); break;
        case RIGHT: Move(0.5 * speed, right); delay(30); break;
        case LEFT: Move(0.5 * speed, left); delay(30); break; 
        case STOP: Move(0, forward); delay(30); break;
        // ��� ���� ���
        case MODE: mode = mode ? 0 : 1; break;
                
        default:
            break;
        }

        irrecv.resume();
    }

    // QTI������ �̿��� ���� ����
    if (!mode) {
        int LeftQti = digitalRead(qtiL);    // black 1 else 0
        int RightQti = digitalRead(qtiR);
        
        Serial.println(LeftQti);
        Serial.println(RightQti);

        // �� ���ϴ� ��� �߰��� ��� �ӵ��� ���� ��Ȱ��ȭ
        // int distance = SonarDistance();

        // Serial.print("distance : ");
        // Serial.println(distance);

        // ������ �������̸� ���� �������� �̵�
        if (LeftQti) {
            Move(speed-5, left);
            delay(3);
        }
        // ������ �������̸� ������������ �̵�
        else if (RightQti) {
            Move(speed-5, right);
            delay(3);
        }
        // �ܴ̿� ���� ����
        else
        {
            Move(speed, forward);
        }
    }
}