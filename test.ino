int LEFT_MOTERL_PIN = 5;
int RIGHT_MOTERL_PIN = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEFT_MOTERL_PIN, OUTPUT);
  pinMode(RIGHT_MOTERL_PIN, OUTPUT);
}

void loop() {
  int leftSpeed = 1407;
  int rightSpeed = 1407;

  if(Serial.available()){
    char val = Serial.read();

    if (val == 'q'){
      leftSpeed+=10;
    }
    else if(val == 'w'){
      leftSpeed-=10;
    }
    if (val == 'a'){
      rightSpeed+=10;
    }
    else if(val == 's'){
      rightSpeed-=10;
    }

    
    Serial.println(rightSpeed);
    serial.println(leftSpeed);

  }
  delay(100);
  digitalWrite(LEFT_MOTERL_PIN, HIGH);
  delayMicroseconds(0);
  digitalWrite(LEFT_MOTERL_PIN, LOW);
  delay(0);
  
  digitalWrite(RIGHT_MOTERL_PIN, HIGH);
  delayMicroseconds(0);
  digitalWrite(RIGHT_MOTERL_PIN, LOW);
  
}