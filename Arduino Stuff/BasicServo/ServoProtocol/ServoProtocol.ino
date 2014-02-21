#include <Servo.h> 

#define servo1Pin 10
#define servo1

int servoNumberPin[] = {servo1Pin};
Servo servo[] = {servo1};
int servoValues[] = {0};

String inputData = "";

void setup() {
  initSerial();
  
  for (int i = 0; i <= 0; i++) {
    servo[i].attach(servoNumberPin[i]);
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
  Serial.begin(9600);
}

String recievePacket() {
  int index = 0;
  char data[36]; //32 + 4 for 1 servo
  
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
 // parseMotorValues();
  parseServoValues();

}

void act() {
  // Act
  //actOnMotors();
  actOnServos();
  
}

void parseServoValues() {
  for (int i = 1; i <= 1; i++) {
    servoValues[i-1] = getServoValueAt(i);
  }
}

int getServoValueAt(int servoNumber) {
  int placeHolder = 0;
  switch(servoNumber) {
    case 1:
      placeHolder = 25;
      break;
  }
  String val = inputData.substring(placeHolder, placeHolder+3);
  return val.toInt();
}

void actOnServos() {
  for (int i = 0; i <= 0; i++) {
    int a = servoValues[i];
    servo[i].write(a);
  }
}








  
