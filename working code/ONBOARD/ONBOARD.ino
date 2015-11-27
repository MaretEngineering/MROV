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
#define DEBUG false

#define FORWARD HIGH
#define BACKWARD LOW

//************************************
// Motor pins
//************************************

#define NUM_MOTORS 6

//Motor thruster pins
// finalized:
/*

  SEE THE MAP FOR CONVERSIONS BETWEEN DWORKEN AND DAVID SYSTEMS 

  6        3
 
  2-1
 
  5        4

*/
#define MT1t_PIN 12
#define MT2t_PIN 9
#define MT3t_PIN 8
#define MT4t_PIN 13
#define MT5t_PIN 11
#define MT6t_PIN 10
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

// Change this to match spec
#define NUM_SERVOS 3

// Change to match setup

// Pan
#define SERVO_1_PIN 3
// Claw
#define SERVO_2_PIN 5
// Tilt
#define SERVO_3_PIN 2

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
int servoValues[] = {0, 0, 0, 0, 0, 0};

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
    Serial.begin(9600);
    Serial.println("Serial initialized");

    // Set thrust pins to output
    pinMode(MT1t_PIN, OUTPUT);
    pinMode(MT2t_PIN, OUTPUT);
    pinMode(MT3t_PIN, OUTPUT);
    pinMode(MT4t_PIN, OUTPUT);
    pinMode(MT5t_PIN, OUTPUT);
    pinMode(MT6t_PIN, OUTPUT);
    Serial.println("Motors initialized");
  
    // Set direction pins to output
    pinMode(MT1d_PIN, OUTPUT);
    pinMode(MT2d_PIN, OUTPUT);
    pinMode(MT3d_PIN, OUTPUT);
    pinMode(MT4d_PIN, OUTPUT);
    pinMode(MT5d_PIN, OUTPUT);
    pinMode(MT6d_PIN, OUTPUT);
    Serial.println("Direction initialized");
  
    // Attach servos
    for (int i = 0; i < NUM_SERVOS; i++) {
	Servo servo = servos[i];
	int servoPin = servoPins[i];
	servo.attach(servoPin);
//    servo.write(0);
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
	    delay(1);
	}
    
	for (int j = 255; j >= 0; j--) {
	    analogWrite(motorThrustPins[i], j);
	    delay(1);
	}
    }
  
    Serial.println("Testing servos...");
    for (int i = 0; i < NUM_SERVOS; i++) {
	Serial.print("Testing servo ");
	Serial.println(i+1);
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
  
} // End setup

void loop() {
  
    //*********************************
    // Receive serial input
    //*********************************
    if(DEBUG){Serial.println("Waiting for values...");}
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
  
    if(DEBUG){Serial.println("Values received: ");}
    if(DEBUG){Serial.println(inputString);}
  
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
	    inputBuffer[j] = inputString[i*4 + j];
	}
	inputBuffer[3] = '\0';
	int mVal = atoi(inputBuffer);
	if(DEBUG){Serial.print("Parsing value: ");}
	if(DEBUG){Serial.println(mVal);}
	motorValues[i] = mVal - 256;
    }
  
    // For debugging, print out the motor values
    if(DEBUG){Serial.print("Parsed motor values: {");}
    for (int i = 0; i < NUM_MOTORS; i++) {
	if(DEBUG){Serial.print(motorValues[i]);}
	if(DEBUG){Serial.print(", ");}
    }
    if(DEBUG){Serial.println("}");}
  
    // Parse servo values
    for (int i = 0; i < NUM_SERVOS; i++) {
	char inputBuffer[4];
	for (int j = 0; j < 3; j++) {
	    inputBuffer[j] = inputString[24 + i*4 + j];
	}
	inputBuffer[3] = '\0';
	int sVal = atoi(inputBuffer);
	if(DEBUG){Serial.print("Parsing servo value: ");}
	if(DEBUG){Serial.println(sVal);}
	servoValues[i] = sVal;
    }
    
    //*********************************
    // Write out motor values
    //*********************************
  
    for (int i = 0; i < NUM_MOTORS; i++){
	int motorSpeed = motorValues[i];
    
	if (!suckBlowTable[i]) {
	    motorSpeed = -motorSpeed;
	}
    
	controlMotor(i, motorSpeed);
    
	delay(1);
    }
  
    // Write out servo values
    for (int i = 0; i < NUM_SERVOS; i++) {
	if(DEBUG){Serial.print("Writing ");}
	if(DEBUG){Serial.print(servoValues[i]);}
	if(DEBUG){Serial.print(" to servo ");}
	if(DEBUG){Serial.println(i+1);}
    
	Servo servo = servos[i];
	servo.write(servoValues[i]);
	delay(1);
    }

}

//motorNum is one of the motor IDs defined in motor_thrust_pins & motor_dir_pins (0 through NUM_MOTORS - 1)
//motorSpeed is number between -255 and 255 (it is constrained below to be so).
boolean controlMotor(int motorNum, int motorSpeed){
    int forward = LOW;
    int reverse = HIGH;
  
    int stallThreshold = 80;
  
    if (motorSpeed < -stallThreshold) {
	motorSpeed = -motorSpeed;
	analogWrite(motorThrustPins[motorNum], motorSpeed);
	digitalWrite(motorDirPins[motorNum], reverse);
	if(DEBUG){Serial.print("Writing ");}
	if(DEBUG){Serial.print(motorSpeed);}
	if(DEBUG){Serial.print(" to motor on pin " );}
	if(DEBUG){Serial.print(motorThrustPins[motorNum]);}
	if(DEBUG){Serial.print(" reversing on pin ");}
	if(DEBUG){Serial.println(motorDirPins[motorNum]);}
    }
    else if (motorSpeed > stallThreshold) {
	analogWrite(motorThrustPins[motorNum], motorSpeed);
	digitalWrite(motorDirPins[motorNum], forward);
	if(DEBUG){Serial.print("Writing ");}
	if(DEBUG){Serial.print(motorSpeed);}
	if(DEBUG){Serial.print(" to motor on pin ");}
	if(DEBUG){Serial.println(motorThrustPins[motorNum]);}
    }
    else {
	analogWrite(motorThrustPins[motorNum], 0);
	digitalWrite(motorDirPins[motorNum], LOW);
	if(DEBUG){Serial.print("Writing ");}
	if(DEBUG){Serial.print(0);}
	if(DEBUG){Serial.print(" to motor on pin ");}
	if(DEBUG){Serial.println(motorThrustPins[motorNum]);}
    }
}
