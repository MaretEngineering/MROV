#define NUM_MOTORS 4
#define NUM_BUTTONS 4

int motorPins[NUM_MOTORS] = {7, 6, 5, 8};
int motorPower[NUM_MOTORS];
int buttonPins[NUM_MOTORS] = {13, 12, 11, 10};
int buttonStates[NUM_BUTTONS] = {0, 0, 0, 0};

// vvvvv-----------------------------------------------------------
int[] thrustValues;
#include "I2CDev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "Servo.h"

int ax, ay, az, gx, gy, gz;
long cax, cay, caz, cgx, cgy, cgz;
long prevTime, curTime;

long lastAccelRead = 0;
long lastGyroRead = 0;

MPU6050 ag;

// Tuning Constants
double Kx = 1.0000;
double Ky = 1.0000;
//double aConst = 1/(sqrt(2)*2*(Kx + Ky));
//double bConst = 1/(sqrt(2)*2*(Ky - Kx));
double aConst = 1;
double bConst = 1;
double aConstR = 1;
double bConstR = 1;
// ^^^^^--------------------------------------------------------
void setup() {
  //Set up serial
  Serial.begin(9600);  //  opens serial port 
  
  // vvvvv--------------------------------------------------------------
  
  Wire.begin();
  ag.initialize();
  ag.setSleepEnabled(false);
  
  ag.setRate(B11111111);
  lastAccelRead = micros();
  lastGyroRead = micros();
  
  // ^^^^---------------------------------------------------------------
  
  
  //Set up pins and arrays
  for (int n = 0; n < NUM_MOTORS; n++) {
    pinMode(motorPins[n], OUTPUT);
    pinMode(buttonPins[n], OUTPUT);
    motorPower[n] = 0;
  }
 
 digitalWrite(buttonPins[0], HIGH);
 
 delay(1000);
 
 digitalWrite(buttonPins[0], LOW);
  
 }

void loop () {
  
  readCharArrayFromSerial();
  
  //Constrain to make sure nothing breaks
  for ( int h = 0; h < NUM_MOTORS; h++ ) {
    if ( motorPower[h] > 0 ) { motorPower[h] = map(motorPower[h], 0, 255, 50, 255); }
    else if (motorPower[h] < 0 ) { motorPower[h] = map(motorPower[h], -255, 0, -255, 50); }
  }
  /*
  for ( int i = 0; i < NUM_MOTORS; i++) {
    analogWrite( motorPins[i], abs(motorPower[i]) );
  }
  */
  
  for ( int i = 0; i < NUM_BUTTONS; i++) {
    if ( buttonStates[i] == 1 ) {
      Serial.print("1");
      digitalWrite( buttonPins[i], HIGH);
    } else {
      digitalWrite( buttonPins[i], LOW);
      Serial.print("0");
    }
  }
  
  Serial.println();
  
  
  delay(10);
}

//Utility method to read a string (in the form of a char array) from Serial
void readCharArrayFromSerial() {
  Serial.println("Beginning Read");
  //Wait for serial to become available
  while (Serial.available() <= 0){}
  
  //Initialize vars
  String num = "";
  int i = 0;
  
  //Get first char
  char no = Serial.read();
  
  //If the first number is not the start character, abort
  if(no != (int)"!") { return; }
  
  //Read motor values
  while(Serial.available() > 0){
    no = Serial.read();
    if(no == (int)"//"){
      char charBuffer[50]; 
      num.toCharArray(charBuffer, 50);
      motorPower[i] = atoi(charBuffer);
      num = "";
      i++;
    } else if (no == (int)"{") {
      break;
    }
    num = num + no;
  }
  
  //Reset
  i = 0;
  num = "";
  
  while (Serial.available() > 0) {
    no = Serial.read();
    
    if (no == (int)"|") {
      char charBuffer[50]; 
      num.toCharArray(charBuffer, 50);
      buttonStates[i] = atoi(charBuffer);
      num = "";
      i ++;
    } else if (no == (int)"$") {
      break;
    }
    
    num = num + no;
  }
  
}

// vvvvv-------------------------------------------------------------
int[] getTranslation(int x, int y){
  int[] vals = new int[4];
  int a = (int)((x + y)*aConst);
  int b = -(int)((y - x)*bConst);
  int d = -a;
  int c = -b;
  vals[0] = a;
  vals[1] = b;
  vals[2] = c;
  vals[3] = d;
  return vals;
}

int[] getRotation(int x) {
  int[] vals = new int[4];
  int a = (int)(x*aConstR);
  int b = -(int)(x*bConstR);
  int d = a;
  int c = b;
  vals[0] = a;
  vals[1] = b;
  vals[2] = c;
  vals[3] = d;
  return vals;
}
// ^^^^^--------------------------------------------------------------------
