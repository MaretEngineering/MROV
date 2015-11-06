//Example Serial: !256/345/422/504/{0|1|0|1|$
#define NUM_MOTORS 4 //Number
#define NUM_BUTTONS 4 //Number


int motorPins[NUM_MOTORS] = {5,6,10,11}; //The arduino pins for the motors
int motorInputPower[NUM_MOTORS] = {0,0,0,0};

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_MOTORS; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  delay(2000);
}

void loop() {
  parseSerial();
  for (int i = 0; i < NUM_MOTORS; i++) {
    analogWrite(motorPins[i], abs(motorInputPower[i])); //NOTE: Absolute val
  }
  delay(10);
}

void parseSerial() {
  int motorDelim = (int)'/';

  int index = 0;
  String input = "";

  if (Serial.available() > 0) {
    if (Serial.read() != (int)'!') { return; } //Don't parse wrong
  }

  while (Serial.available() > 0 && index < NUM_MOTORS) {
    int numeral = Serial.read(); //Read in an individual number
    if (numeral = motorDelim) {
      int motorInputLength = input.length() + 1; //+1 to make room for the null terminator
      char motorInputArr[motorInputLength]; //convert string
      input.toCharArray(motorInputArr, motorInputLength);
      motorInputPower[index] = atoi(motorInputArr);

      index++; //Next motor
      input = ""; //Reset value
    }
    else {
      input += (char) numeral; //Adds the individual numbers together
    }
  }
}


