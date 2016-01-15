#include <Servo.h>


//###################################
//###################################
// Constants
//###################################
//###################################

//************************************
// Miscellaneous
//************************************

// True if you want debug information
#define DEBUG

#define FORWARD HIGH
#define BACKWARD LOW
#define STALL_THRESHOLD 80

//************************************
// Motor pins
//************************************

#define NUM_MOTORS 6
#define NUM_SERVOS 0
#define MESSAGE_SIZE 40

//Motor thruster pins
// finalized:
/*

  SEE THE MAP FOR CONVERSIONS BETWEEN DWORKEN AND DAVID SYSTEMS

  6        3

     2-1

  5        4

*/
#define MT1t_PIN 11
#define MT2t_PIN 10
#define MT3t_PIN 9
#define MT4t_PIN 6
#define MT5t_PIN 5
#define MT6t_PIN 3
int motorThrustPins[] = {MT1t_PIN, MT2t_PIN, MT3t_PIN, MT4t_PIN, MT5t_PIN, MT6t_PIN};

//Motor direction pins
// ADJUST THESE WHEN PINS ARE FINALIZED
#define MT1d_PIN 39
#define MT2d_PIN 33
#define MT3d_PIN 41
#define MT4d_PIN 31
#define MT5d_PIN 37
#define MT6d_PIN 35
int motorDirPins[] = {MT1d_PIN, MT2d_PIN, MT3d_PIN, MT4d_PIN, MT5d_PIN, MT6d_PIN};

bool suckBlowTable[] = {true, false, false, false, false, false};
// bool suck_blow_table[] = {true, false, false, false, false, false};

//************************************
// Servo Values (unsigned 0-90 integers)
//************************************

// Change to match setup

// Pan
#define SERVO_1_PIN 0
// Tilt
#define SERVO_2_PIN 0
// Claw
#define SERVO_3_PIN 0

#define SERVO_4_PIN 0
#define SERVO_5_PIN 0
#define SERVO_6_PIN 0
int servoPins[] = {SERVO_1_PIN, SERVO_2_PIN, SERVO_3_PIN, SERVO_4_PIN, SERVO_5_PIN, SERVO_6_PIN};

Servo servos[NUM_SERVOS];

//###################################
//###################################
// Global Variables
//###################################
//###################################

//************************************
// Motor Values (signed 8-bit integers)
//************************************

int motorValues[] = {0, 0, 0, 0, 0};
//5 because the vertical motors have the same
int servoValues[] = {0, 0, 0, 0, 0, 0};
char rawInput[MESSAGE_SIZE];
//The raw message from serial

//###################################
//###################################
// Code!
//###################################
//###################################

void setup() {
  //*********************************
  // Initialize Systems
  //*********************************

  // Init Serial
  Serial.begin(38400);
  Serial.println("Serial initialized");

  // Set thrust pins to output
  for (int i = 0; i < NUM_MOTORS; i++) {
    pinMode(motorThrustPins[i], OUTPUT);
    pinMode(motorDirPins[i], OUTPUT);
  }
  Serial.println("Motors initialized");
  Serial.println("Direction initialized");

  // Attach servos
  for (int i = 0; i < NUM_SERVOS; i++) {
    Servo servo = servos[i];
    servo.attach(servoPins[i]);
  }

  //*********************************
  // Test Systems
  //*********************************

  //Test motors
  for (int i = 0; i < NUM_MOTORS; i++) {  
    Serial.print("Testing motor ");
    Serial.println(i);
    for (int j = 0; j < 256; j++) {
      analogWrite(motorThrustPins[i], j);
      delay(3);
    }
    
    delay(200);
    
    for (int j = 255; j >= 0; j--) {
      analogWrite(motorThrustPins[i], j);
      delay(3);
    }
  }
  

  Serial.println("Testing servos...");
  for (int i = 0; i < NUM_SERVOS; i++) {
    Serial.print("Testing servo ");
    Serial.println(i + 1);
    Servo servo = servos[i];
    servo.write(80);
    delay(500);
    servo.write(100);
    delay(500);
  }
  Serial.println("Servos tested");

  Serial.println("Clearing serial buffer");
  while (Serial.available() > 0) {
    Serial.print((char)Serial.read());
  }
  Serial.println("Done clearing buffer");

    
    Serial.println("END SETUP");
    Serial.println("END SETUP");
    Serial.println("END SETUP");
    Serial.println("END SETUP");
    Serial.println("END SETUP");
    Serial.println("END SETUP");
  
} // End setup

void loop() {

  //*********************************
  // Receive serial input
  //*********************************
#ifdef DEBUG
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
    Serial.println("Waiting for values...");
#endif

  while (Serial.available() <= 0 || (char)Serial.read() != '!') {
    // Wait for start signal...
  }

  delay(10); // To give the next byte a chance to come in
  // By this point we know that either there is nothing in the RX buffer
  //   or the first byte in the buffer is the second byte of the message
  //   (as we already read the first byte to see if it was a '!')
  char inputString[40];
  int i = 0;
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();

    if (inChar == '$') {
      break; // Stop byte recieved, breaking out
    }

    inputString[i] = inChar;
    i++;
    delay(1);
  }

  delay(1);

#ifdef DEBUG
  Serial.println("Values received: ");
  Serial.println(inputString);
#endif

  //************************************
  // Parse the received values
  //
  //  Reference Message:
  //    !123/123/123/123/123/123/000/000$
  //  note: ! is received, but is not stored
  //************************************

  for (int i = 0; i < NUM_MOTORS; i++) {
    char inputBuffer[4];
    for (int j = 0; j < 3; j++) {
      inputBuffer[j] = inputString[i * 4 + j];
    }
    inputBuffer[3] = '\0';
    int mVal = atoi(inputBuffer);
#ifdef DEBUG
    Serial.print("Parsing value: ");
    Serial.println(mVal);
#endif
    motorValues[i] = mVal - 256;
  }

  // For debugging, print out the motor values
#ifdef DEBUG
  Serial.print("Parsed motor values: {");
  for (int i = 0; i < NUM_MOTORS; i++) {
    Serial.print(motorValues[i]);
    Serial.print(", ");
  }
  Serial.println("}");
#endif

  // Parse servo values
  for (int i = 0; i < NUM_SERVOS; i++) {
    char inputBuffer[4];
    for (int j = 0; j < 3; j++) {
      //24 is the start of the servo values (with 6 motors)
      //i*4 is servo number * characters per servo (###/ = 4)
      //j is the digit of the servo value
      inputBuffer[j] = inputString[24 + i * 4 + j];
    }
    inputBuffer[3] = '\0';
    int sVal = atoi(inputBuffer);
#ifdef DEBUG
    Serial.print("Parsing servo value: ");
    Serial.println(sVal);
#endif
    servoValues[i] = sVal;
  }

  //*********************************
  // Write out motor values
  //*********************************

  for (int i = 0; i < NUM_MOTORS; i++) {
    int motorSpeed = motorValues[i];

    if (!suckBlowTable[i]) {
      motorSpeed = -motorSpeed;
    }

    controlMotor(i, motorSpeed);

    delay(1);
  }

  // Write out servo values
  for (int i = 0; i < NUM_SERVOS; i++) {
#ifdef DEBUG
    Serial.print("Writing ");
    Serial.print(servoValues[i]);
    Serial.print(" to servo ");
    Serial.println(i + 1);
#endif
    Servo servo = servos[i];
    servo.write(servoValues[i]);
    delay(1);
  }

}

//motorNum is one of the motor IDs defined in motor_thrust_pins & motor_dir_pins (0 through NUM_MOTORS - 1)
//motorSpeed is number between -255 and 255 (it is constrained below to be so).
boolean controlMotor(int motorNum, int motorSpeed) {
  int forward = LOW;
  int reverse = HIGH;

  int stallThreshold = 80;

  if (motorSpeed < -stallThreshold) {
    motorSpeed = -motorSpeed;
    analogWrite(motorThrustPins[motorNum], motorSpeed);
    digitalWrite(motorDirPins[motorNum], reverse);
#ifdef DEBUG
    Serial.print("Writing ");
    Serial.print(motorSpeed);
    Serial.print(" to motor on pin " );
    Serial.print(motorThrustPins[motorNum]);
    Serial.print(" reversing on pin ");
    Serial.println(motorDirPins[motorNum]);
#endif
  }
  else if (motorSpeed > stallThreshold) {
    analogWrite(motorThrustPins[motorNum], motorSpeed);
    digitalWrite(motorDirPins[motorNum], forward);
#ifdef DEBUG
    Serial.print("Writing ");
    Serial.print(motorSpeed);
    Serial.print(" to motor on pin ");
    Serial.println(motorThrustPins[motorNum]);
#endif
  }
  else {
    analogWrite(motorThrustPins[motorNum], 0);
    digitalWrite(motorDirPins[motorNum], LOW);
#ifdef DEBUG
    Serial.print("Writing ");
    Serial.print(0);
    Serial.print(" to motor on pin ");
    Serial.println(motorThrustPins[motorNum]);
#endif
  }
}
