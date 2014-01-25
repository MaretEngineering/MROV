#include <PID_v1.h>
#include <Adafruit_BMP085.h>

// Variables!
double motorValues[] = {0, 0, 0, 0, 0};
double servoValues[] = {0, 0, 0, 0};

boolean is_PID_on = false;
double setpoint = 101000;
double sensor = 0;
PID pid(&sensor, &motorValues[4], &setpoint, 1, 1, 1, DIRECT);

Adafruit_BMP085 bmp;

void setup() {
  initSensors();
  initSerial();
  initPID();
}

void loop() {
  String packet = recievePacket();
  parseThePacket(packet);
  if (is_PID_on == true) {
    adjustDepthSetpoint();
    getSensorData();
    computePID();
  }
  actOnDepthValues();
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

void initPID() {
  pid.SetMode(AUTOMATIC);
}

String recievePacket() {
  // E&Z's code for recieving from Serial goes here
}

void parseThePacket(String packet) {
  // Parse the packet
}

int getMotorValueAt(int mototIndex) {
  // Ethan & Zane's code for getting motor values
}

boolean getButtonValueAt(int buttonIndex) {
  // Adaptation of E&Z's code
}

void adjustDepthSetpoint() {
  //Adjust depth setpoint
  double adjustment = motorValues[4];
  adjustment = map(adjustment, -256, 256, -10, 10);
  setpoint += adjustment;
}

void getSensorData() {
  // Get the sensor data
}

void computePID() {
  // Compute the PID
}

void actOnDepthValues() {
  //Act on the depth values
  
  
  
  
}

void act() {
  // Act
  actOnMotors();
  actOnServos();
}

void actOnMotors() {
  //Act on the motors
}

void actOnServos() {
  //Act on the servos
}
