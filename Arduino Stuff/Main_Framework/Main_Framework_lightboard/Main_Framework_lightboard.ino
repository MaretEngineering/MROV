#define FORWARD HIGH
#define BACKWARD LOW

//Variables!
double motorValues[] = {0, 0, 0, 0, 0};

//Motor Pins
<<<<<<< HEAD
#define motor1Pin 3
=======
#define motor1Pin 2
>>>>>>> 776b462ea472f9fe6b2637f6fad5401ac3b57919
#define motor2Pin 5
#define motor3Pin 6
#define motor4Pin 9
#define motorUp1Pin 0
#define motorUp2Pin 0
<<<<<<< HEAD
#define motor1PinDir 2
=======
#define motor1PinDir 3
>>>>>>> 776b462ea472f9fe6b2637f6fad5401ac3b57919
#define motor2PinDir 4
#define motor3PinDir 7
#define motor4PinDir 8
#define motorUp1PinDir 0
#define motorUp2PinDir 0
int motorValuePins[] = {motor1Pin, motor2Pin, motor3Pin, motor4Pin, motorUp1Pin, motorUp2Pin};
int motorDirPins[] = {motor1PinDir, motor2PinDir, motor3PinDir, motor4PinDir, motorUp1PinDir, motorUp2PinDir};

String inputData = "";

void setup() {
  
  initSerial();
  
  for (int i = 0; i < 6; i++) {
    pinMode(motorValuePins[i], OUTPUT);
    pinMode(motorDirPins[i], OUTPUT);
  }
}

void loop() {
  recievePacket();
  parseThePacket();
//actOnDepthValues();
  act();
}

void initSerial() {
  // Initialize the serial
  Serial.begin(115200);
}

String recievePacket() {
  int index = 0;
  char data[32];
  
  while (Serial.available() <= 0 || Serial.read() != '!') {}
  delay(1);
  while (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '$') { break; }
    data[index] = input;
    index ++;
    delay(1);
 }
 data[index] = '\0';
 inputData = String(data);
}

void parseThePacket() {
  parseMotorValues();

}

void parseMotorValues() {
  for (int i = 1; i <= 5; i++) {
    motorValues[i-1] = getMotorValueAt(i) - 256;
  }
}

int getMotorValueAt(int motorNumber) {
  int placeHolder = 0;
  
  switch (motorNumber) {
    case 1:
      placeHolder = 0;
      break;
    case 2:
      placeHolder = 4;
      break;
    case 3:
      placeHolder = 8;
      break;
    case 4:
      placeHolder = 12;
      break;
    case 5:
      placeHolder = 16;
      break;
  }
  String val = inputData.substring(placeHolder, placeHolder + 3);
  return val.toInt();
}

void act() {
  // Act
  actOnMotors();
  
}

void actOnMotors() {
  //Deal with horizontal motors
  for (int i = 0; i < 4; i++) {
    int a = motorValues[i];
    if (a < 0) {
      a = -a;
      a = constrain(a, 10, 256);
      analogWrite(motorValuePins[i], a);
      digitalWrite(motorDirPins[i], BACKWARD);
    } else {
      a = constrain(a, 10, 256);
      analogWrite(motorValuePins[i], a);
      digitalWrite(motorDirPins[i], FORWARD);
    }
  }
  
  //Now deal with those pesky depth motors
  int a = motorValues[4];
  if (a < 0) {
    a = -a;
    a = constrain(a, 10, 256);
    analogWrite(motorValuePins[4], a);
    digitalWrite(motorDirPins[4], BACKWARD);
    analogWrite(motorValuePins[5], a);
    digitalWrite(motorDirPins[5], BACKWARD);
  } else {
    a = constrain(a, 10, 256);
    analogWrite(motorValuePins[4], a);
    digitalWrite(motorDirPins[4], FORWARD);
    analogWrite(motorValuePins[5], a);
    digitalWrite(motorDirPins[5], FORWARD);
  }
}









