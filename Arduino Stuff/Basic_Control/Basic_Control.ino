#define FORWARD HIGH
#define BACKWARD LOW

#define motor1Pin 0
#define motor2Pin 0
#define motor3Pin 0
#define motor4Pin 0
#define motorUp1Pin 0
#define motorUp2Pin 0
//motor Direction
#define motor1PinDir 0
#define motor2PinDir 0
#define motor3PinDir 0
#define motor4PinDir 0
#define motorUp1PinDir 0
#define motorUp2PinDir 0
int motorValuePins[] = {motor1Pin, motor2Pin, motor3Pin, motor4Pin, motorUp1Pin, motorUp2Pin};
int motorDirPins[] = {motor1PinDir, motor2PinDir, motor3PinDir, motor4PinDir, motorUp1PinDir, motorUp2PinDir};


void setup() {
  initSerial();
  
  for (int i = 0; i < 6; i++) {
    pinMode(motorValuePins[i], OUTPUT);
    pinMode(motorDirPins[i], OUTPUT);
  }
}

void loop() {
  int val;
  while (Serial.available() <= 0) {
    val = Serial.read();
  }
  switch (val) {
    case 0:
    //Off
      for (int i = 0; i < 6; i++) {
        digitalWrite(motorValuePins[i], 0);
      }
      break;
    case 1:
    //Forwards
      digitalWrite(motorValuePins[0], HIGH);
      digitalWrite(motorDirPins[0], FORWARD);
      digitalWrite(motorValuePins[1], HIGH);
      digitalWrite(motorDirPins[1], FORWARD);
      digitalWrite(motorValuePins[2], HIGH);
      digitalWrite(motorDirPins[2], BACKWARD);
      digitalWrite(motorValuePins[3], HIGH);
      digitalWrite(motorDirPins[3], BACKWARD);
      digitalWrite(motorValuePins[4], LOW);
      digitalWrite(motorDirPins[4], FORWARD);
      digitalWrite(motorValuePins[5], LOW);
      digitalWrite(motorDirPins[5], FORWARD);
      break;
    case 2: 
   //Backwards 
      digitalWrite(motorValuePins[0], HIGH);
      digitalWrite(motorDirPins[0], BACKWARD);
      digitalWrite(motorValuePins[1], HIGH);
      digitalWrite(motorDirPins[1], BACKWARD);
      digitalWrite(motorValuePins[2], HIGH);
      digitalWrite(motorDirPins[2], FORWARD);
      digitalWrite(motorValuePins[3], HIGH);
      digitalWrite(motorDirPins[3], FORWARD);
      digitalWrite(motorValuePins[4], LOW);
      digitalWrite(motorDirPins[4], FORWARD);
      digitalWrite(motorValuePins[5], LOW);
      digitalWrite(motorDirPins[5], FORWARD);
      break;
    case 3:
   //Left
      digitalWrite(motorValuePins[0], HIGH);
      digitalWrite(motorDirPins[0], BACKWARD);
      digitalWrite(motorValuePins[1], HIGH);
      digitalWrite(motorDirPins[1], FORWARD);
      digitalWrite(motorValuePins[2], HIGH);
      digitalWrite(motorDirPins[2], FORWARD);
      digitalWrite(motorValuePins[3], HIGH);
      digitalWrite(motorDirPins[3], BACKWARD);
      digitalWrite(motorValuePins[4], LOW);
      digitalWrite(motorDirPins[4], FORWARD);
      digitalWrite(motorValuePins[5], LOW);
      digitalWrite(motorDirPins[5], FORWARD);
      break;
    case 4:
   //Right
      digitalWrite(motorValuePins[0], HIGH);
      digitalWrite(motorDirPins[0], FORWARD);
      digitalWrite(motorValuePins[1], HIGH);
      digitalWrite(motorDirPins[1], BACKWARD);
      digitalWrite(motorValuePins[2], HIGH);
      digitalWrite(motorDirPins[2], BACKWARD);
      digitalWrite(motorValuePins[3], HIGH);
      digitalWrite(motorDirPins[3], FORWARD);
      digitalWrite(motorValuePins[4], LOW);
      digitalWrite(motorDirPins[4], FORWARD);
      digitalWrite(motorValuePins[5], LOW);
      digitalWrite(motorDirPins[5], FORWARD);
      break;
    case 5:
   //Up
      digitalWrite(motorValuePins[0], LOW);
      digitalWrite(motorDirPins[0], BACKWARD);
      digitalWrite(motorValuePins[1], LOW);
      digitalWrite(motorDirPins[1], BACKWARD);
      digitalWrite(motorValuePins[2], LOW);
      digitalWrite(motorDirPins[2], FORWARD);
      digitalWrite(motorValuePins[3], LOW);
      digitalWrite(motorDirPins[3], FORWARD);
      digitalWrite(motorValuePins[4], HIGH);
      digitalWrite(motorDirPins[4], FORWARD);
      digitalWrite(motorValuePins[5], HIGH);
      digitalWrite(motorDirPins[5], FORWARD);
      break;
    case 6:
   //Down
      digitalWrite(motorValuePins[0], LOW);
      digitalWrite(motorDirPins[0], BACKWARD);
      digitalWrite(motorValuePins[1], LOW);
      digitalWrite(motorDirPins[1], BACKWARD);
      digitalWrite(motorValuePins[2], LOW);
      digitalWrite(motorDirPins[2], FORWARD);
      digitalWrite(motorValuePins[3], LOW);
      digitalWrite(motorDirPins[3], FORWARD);
      digitalWrite(motorValuePins[4], HIGH);
      digitalWrite(motorDirPins[4], BACKWARD);
      digitalWrite(motorValuePins[5], HIGH);
      digitalWrite(motorDirPins[5], BACKWARD);
      break;
  }
}
void initSerial() {
  Serial.begin(9600);
}


  
