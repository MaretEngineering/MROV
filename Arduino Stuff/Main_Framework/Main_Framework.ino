#include <PID_v1.h>
#include <Adafruit_BMP085.h>

#define FORWARD HIGH
#define BACKWARD LOW

// Variables!
double motorValues[] = {0, 0, 0, 0, 0};
double servoValues[] = {0, 0, 0, 0};

// Motor Pins!
int motor1Pin = 0;
int motor2Pin = 0;
int motor3Pin = 0;
int motor4Pin = 0;
int motorUp1Pin = 0;
int motorUp2Pin = 0;
int motor1PinDir = 0;
int motor2PinDir = 0;
int motor3PinDir = 0;
int motor4PinDir = 0;
int motorUp1PinDir = 0;
int motorUp2PinDir = 0;
int motorValuePins[] = {motor1Pin, motor2Pin, motor3Pin, motor4Pin, motorUp1Pin, motorUp2Pin};
int motorDirPins[] = {motor1PinDir, motor2PinDir, motor3PinDir, motor4PinDir, motorUp1PinDir, motorUp2PinDir};


boolean is_PID_on = false;
double setpoint = 101000;
double sensor = 0;
PID pid(&sensor, &motorValues[4], &setpoint, 1, 1, 1, DIRECT);

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
    pid.compute();
  }
  
  //actOnDepthValues();
  act();
}

void initSensors() {
  if (!bmp.begin()) {
    while(1) {}
  }
}

void initSerial() {
  // Initialize the serial
  Serial.begin(9600);
}
//intializes the PID
void initPID() {
  pid.SetMode(AUTOMATIC);
}

String recievePacket() {
  // E&Z's code for recieving from Serial goes here
}

void parseThePacket(String packet) {
  // Parse the packet also part of E&Z's code
}

int getMotorValueAt(int mototIndex) {
  // Ethan & Zane's code for getting motor values
}

boolean getButtonValueAt(int buttonIndex) {
  // Adaptation of E&Z's code
}
// if the PID is not on this will allow the user to manually adjust the depth setpoint
void adjustDepthSetpoint() {
  //Adjust depth setpoint
  double adjustment = motorValues[4];
  adjustment = map(adjustment, -256, 256, -10, 10);
  setpoint += adjustment;
}

void getSensorData() {
  // Get the sensor data
}

void actOnDepthValues() {
  //Act on the depth values
  control_value = motor_values[4];
  
  //Readjust the scale
  control_value -= 256;
  
  //Set the motor value accordingly
  if (control_value < 0) {
    run_motor(4, abs(control_value), BACKWARD);
    run_motor(5, abs(control_value), BACKWARD);
  } else {
    run_motor(4, abs(control_value), FORWARD);
    run_motor(5, abs(control_value), FORWARD);
  }
}

void act() {
  // Act
  actOnMotors();
  actOnServos();
}

void actOnMotors() {
  for (int i = 0; i < 4; i++) {
    a = motorValues[i];
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
    a = motorValues[4];
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
