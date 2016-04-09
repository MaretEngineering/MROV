#include <Servo.h>

#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

//BUFFER CODE
//###################################
//###################################
// Constants
//###################################
//###################################

//************************************
// Miscellaneous
//************************************

// True if you want debug information

#define FORWARD HIGH
#define BACKWARD LOW
#define STALL_THRESHOLD 80 

#define START_DELIM 1
#define END_DELIM 255

//************************************
// Motor pins
//************************************

#define NUM_HORIZ_MOTORS 4
#define NUM_VERT_MOTORS 2
//use below for serial size calulations
#define NUM_VERT_MOTORS_SERIAL 1
#define NUM_MOTORS NUM_VERT_MOTORS + NUM_HORIZ_MOTORS
#define NUM_SERVOS 4

//Note: MESSAGE_SIZE doesn't include the PID toggle
#define MESSAGE_SIZE 2 + NUM_HORIZ_MOTORS + NUM_VERT_MOTORS_SERIAL + NUM_SERVOS

//Motor thruster pins
// finalized:
/*

  SEE THE MAP FOR CONVERSIONS BETWEEN DWORKEN AND DAVID SYSTEMS

  6        3

     2-1

  5        4

*/
////light board pins
//#define MT1t_PIN 11
//#define MT2t_PIN 10
//#define MT3t_PIN 5
//#define MT4t_PIN 1
//#define MT5t_PIN 9
//#define MT6t_PIN 6

//real pins
#define MT1t_PIN 12
#define MT2t_PIN 9
#define MT3t_PIN 8
#define MT4t_PIN 13
#define MT5t_PIN 11
#define MT6t_PIN 10
int motorThrustPins[] = {MT1t_PIN, MT2t_PIN, MT3t_PIN, MT4t_PIN, MT5t_PIN, MT6t_PIN};

//Motor direction pins
// ADJUST THESE WHEN PINS ARE FINALIZED
//light board
//#define MT1d_PIN 2
//#define MT2d_PIN 4
//#define MT3d_PIN 7
//#define MT4d_PIN 8
//#define MT5d_PIN 12
//#define MT6d_PIN 13

//real pins
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
#define SERVO_PAN 3
// Tilt
#define SERVO_TILT 7
// Claw
#define SERVO_CLAW 5
// Claw Wrist
#define SERVO_WRIST 6

#define SERVO_5_PIN 0
#define SERVO_6_PIN 0
int servoPins[] = {SERVO_PAN, SERVO_TILT, SERVO_CLAW, SERVO_WRIST, SERVO_5_PIN, SERVO_6_PIN};

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
int rawInput[MESSAGE_SIZE];
//The raw message from serial

//pressure sensor
MS5803 sensor(ADDRESS_HIGH);

float temp;
double pressure;
double height;
double setpoint;

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

  sensor.reset();
  sensor.begin();

  //*********************************
  // Test Systems
  //*********************************

  //Test motors
  for (int i = 0; i < NUM_MOTORS; i++) {  
    Serial.print("Testing motor ");
    Serial.println(i);
    for (int j = STALL_THRESHOLD; j < 256; j++) {
      analogWrite(motorThrustPins[i], j);
      delay(3);
    }
    
    delay(3);
    
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


    Serial.println("Testing setpoint");
    setpoint = sensor.getPressure(ADC_4096);
    Serial.print("Setpoint = "); Serial.println(setpoint);
    
    Serial.println("Ending Setup");
  
} // End setup

void loop() {

  //*********************************
  // Receive serial input
  //*********************************
  
    parseSerial();
    
  //************************************
  // Parse the received values
  //
  //  Reference Message:
  //     [1]dddddZZZ[255] corresponds to:
  //     [1][100][100][100][100][100][90][90][90][255] 
  //************************************

  
    parseThrustMotorVals();
    parseServoVals();

    
  //*********************************
  // Write out motor values
  //*********************************
    for (int i = 0; i < NUM_MOTORS; i++) {
        controlThrustMotor(i, motorValues[i]);
    }
    for (int i = 0; i < NUM_SERVOS; i++) {
        controlServoMotor(i, servoValues[i]);
    }

    //*****************************
    // Generating Sensor Values
    //*****************************

    temp = random(0,100);
    height = random(0, 1000);

    Serial.print("t$"); Serial.println(temp);
    Serial.print("h$"); Serial.println(height);
    
    
}


//###################################
//###################################
//NEW METHODS FOR PARSING NEW FORMAT OF MESSAGE
//Updated: 1/17/16
//###################################
//###################################

/**
 * Receives the message from serial and places it in
 * char rawInput[]
 */
void parseSerial() {
#ifdef DEBUG
    Serial.println("Waiting for values...");
#endif

    while (Serial.available() <= 0 || (int)Serial.read() != 1) {
        //Wait for start
    }
    rawInput[0] = 36; //for the missing one taken by the while loop
    
    delay(10); //Give the next byte a chance
    int i = 1;
    while (0==0) {
        int input  = Serial.read();
        rawInput[i] = input;
        if (input == 255) {
            break;
        }
        i++;
        delay(1);
    }
#ifdef DEBUG
    Serial.println("Values received: ");
    for(int i = 0; i < 7; i++){
        Serial.println(rawInput[i] + ",");
    }
    Serial.println();
#endif
}

/**
 * Parses the thrust-motor values from char rawInput[]
 * and assigns them to int values in motorValues[]
 */
void parseThrustMotorVals() {
    int motorVal;
    for (int i=0; i < NUM_MOTORS; i++) {
        //This if statement ensures that the 1 vert motor val maps to all the vert motors
        if (i <= NUM_HORIZ_MOTORS) { 
            motorVal = rawInput[i + 1]; //+1 for start delim
            //Map raw values in range [2,254] to [-255,255]
            motorVal = (motorVal - 128)*2; //Range: [-252,252]
            if (motorVal < 0) motorVal -= 3;
            else if (motorVal > 0) motorVal += 3; //Range[-255,255]
        }
        motorValues[i] = motorVal;
    }
#ifdef DEBUG
    Serial.print("DEBUG: Parsing motor values: {");
    for (int i = 0; i < NUM_MOTORS; i++) {
        Serial.print(motorValues[i]);
        Serial.print(", ");
    }
    Serial.println("}");
#endif
}

/**
 * Parses the servo values from char rawInput[]
 * and assigns them to int values in servoValues[]
 */
void parseServoVals() {
    for (int i = 0; i < NUM_SERVOS; i++) {
        servoValues[i] = (int)rawInput[NUM_MOTORS + i] - 1; //-1 to make the range [0,179]. Should it be - 2?
    }
#ifdef DEBUG
    Serial.print("DEBUG: Parsing servo values: {");
    for (int i = 0; i < NUM_SERVOS; i++) {
        Serial.print(servoValues[i]);
        Serial.print(", ");
    }
    Serial.println("}");
#endif
}

/**
 * Write the motor speed to the specified motor and changes
 *its direction depending on the sign (+/-) of the speed
 * @param   int motorNum        The ID of the motor
 * @param   int motorSpeed      A value in range [-255,255]
 */
void controlThrustMotor(int motorNum, int motorSpeed) {
    motorSpeed = constrain(motorSpeed, -255,255); //Keeps in range
    if (suckBlowTable[motorNum]) motorSpeed = 0 - motorSpeed; //dammit ethan
    if (motorSpeed > STALL_THRESHOLD) {
        digitalWrite(motorDirPins[motorNum], FORWARD); //Set forward
    }
    else if (motorSpeed < (0 - STALL_THRESHOLD)) { //Subtraction is more efficient than multiplication
            motorSpeed = 0 - motorSpeed; //Can only write positive values, but change direction
            digitalWrite(motorDirPins[motorNum], BACKWARD);
    }
    else {
        motorSpeed = 0;
        digitalWrite(motorDirPins[motorNum], FORWARD);
    }
    analogWrite(motorThrustPins[motorNum], motorSpeed); //Write the actual value
#ifdef DEBUG
Serial.print("DEBUG: Writing");
    Serial.print(motorSpeed);
    Serial.print(" to motor on pin ");
    Serial.println(motorThrustPins[motorNum]);
#endif
}

/**
 * Write the servo value to the specified servo motor
 * @param   int servoNum      The ID of the servo
 * @param   int servoVal      The degree turn of the servo
 */
void controlServoMotor(int servoNum, int servoVal) {
    servoVal = constrain(servoVal, 0, 179); //Keeps in range
    Servo servo = servos[servoNum];
    servo.write(servoVal);
#ifdef DEBUG
    Serial.print("DEBUG: Writing ");
    Serial.print(servoValues[servoNum]);
    Serial.print(" to servo ");
    Serial.println(servoNum);
#endif
}
