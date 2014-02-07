#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define FORWARD HIGH
#define BACKWARD LOW

// Variables!
double motorValues[] = {0, 0, 0, 0, 0};
double servoValues[] = {0, 0, 0, 0};

// Motor Pins!
#define motor1Pin 0
#define motor2Pin 0
#define motor3Pin 0
#define motor4Pin 0
#define motorUp1Pin 0
#define motorUp2Pin 0
#define motor1PinDir 0
#define motor2PinDir 0
#define motor3PinDir 0
#define motor4PinDir 0
#define motorUp1PinDir 0
#define motorUp2PinDir 0
int motorValuePins[] = {motor1Pin, motor2Pin, motor3Pin, motor4Pin, motorUp1Pin, motorUp2Pin};
int motorDirPins[] = {motor1PinDir, motor2PinDir, motor3PinDir, motor4PinDir, motorUp1PinDir, motorUp2PinDir};

String inputData = "";

boolean is_PID_on = false;
double setpoint = 101000;
double sensor = 0;
double depth = 0;
PID pid(&depth, &motorValues[4], &setpoint, 1, 1, 1, DIRECT);

Adafruit_BMP085 bmp;

void setup() {
  initSensors();
  initSerial();
  initPID();
  for (int i = 0; i < 6; i++) {
    pinMode(motorValuePins[i], OUTPUT);
    pinMode(motorDirPins[i], OUTPUT);
  }
}

void loop() {
  String packet = recievePacket();
  parseThePacket(packet);
  if (is_PID_on == true) {
    adjustDepthSetpoint();
    getSensorData();
    pid.Compute();
  }
  
  //actOnDepthValues();
  act();
}

void initSensors() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085. Feed me kittens");
    while(1) {}
  }
}

void initSerial() {
  // Initialize the serial
  Serial.begin(115200);
}
//intializes the PID
void initPID() {
  pid.SetMode(AUTOMATIC);
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

void parseThePacket(String packet) {
  // P
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

boolean getButtonValueAt(int buttonNumber) {
  int placeHolder = 0;
  
  switch (buttonNumber) {
    case 1:
      placeHolder = 22;
      break;
    case 2:
      placeHolder = 24;
      break;
    case 3:
      placeHolder = 26;
      break;
    case 4:
      placeHolder = 28;
      break;
  }
  String val = inputData.substring(placeHolder, placeHolder + 3);
  return val.toInt();
}

// if the PID is on this will allow the user to manually adjust the depth setpoint
void adjustDepthSetpoint() {
  //Adjust depth setpoint
  double adjustment = motorValues[4];
  adjustment = map(adjustment, -256, 256, -10, 10);
  setpoint += adjustment;
}

void getSensorData() {
  sensor = bmp.readPressure();
  depth = (sensor - 101500)/101500;
  Serial.print("Depth: ");
  Serial.println(depth);
}

void act() {
  // Act
  actOnMotors();
  actOnServos();
}

void actOnMotors() {
  for (int i = 0; i < 4; i++) {
    int a = motorValues[i];
    a -= 256;
    if (a < 0) {
      a = abs(a);
      analogWrite(motorValuePins[i], a);
      digitalWrite(motorDirPins[i], -1);
    } else {
      a = abs(a);
      analogWrite(motorValuePins[i], a);
      digitalWrite(motorDirPins[i], 1);
    }
  }
    int a = motorValues[4];
    a -= 256;
    if (a < 0) {
      a = abs(a);
      analogWrite(motorValuePins[4], a);
      digitalWrite(motorDirPins[4], -1);
      analogWrite(motorValuePins[5], a);
      digitalWrite(motorDirPins[5], -1);
    } else {
      a = abs(a);
      analogWrite(motorValuePins[4], a);
      digitalWrite(motorDirPins[4], 1);
      analogWrite(motorValuePins[5], a);
      digitalWrite(motorDirPins[5], 1);
    }
}

void actOnServos() {
  //Act on the servos
}
