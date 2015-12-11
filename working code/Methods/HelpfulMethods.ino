#define FORWARD LOW
#define BACKWARD HIGH
#define STALL_THRESHOLD 80
#define START_DELIM '!'
#define END_DELIM '$'

#define NUM_HMOTORS 4
#define NUM_VMOTORS 2

#define VMOTOR_START (4*NUM_HMOTORS)
#define PID_START (4*NUM_MOTORS)
#define SERVO_START (4*NUM_MOTORS) + 2

//Message = 1 start delim, 1 end delim, 3 digits + 1 motor delim (4)
//* the number of motors, * the number of servos - 1 motor delim
//= 2 + (4*NUM_MOTORS) + (4*NUM_SERVOS) - 1 + 2 for PID state and delim
//If we're adding the pid button/other buttons, needs modifications
#define MESSAGE_SIZE 3 + (4*NUM_MOTORS) + (4*NUM_SERVOS)
char rawInput[MESSAGE_SIZE]; //The raw message from serial
//Will this 

boolean pidState = false; //Whether the PID should be on or off

void loop() {
	parseSerial();
	parseThrustMotorVals();
	parseServoVals();
	for (int i = 0; i < NUM_MOTORS; i++) {
		controlThrustMotor(i, motorValues[i]);
	}
	for (int i = 0; i < NUM_SERVOS; i++) {
		controlServoMotor(i, servoValues[i]);
	}
}
/**
 * Receives the message from serial and places it in the
 * char array rawInput[]
 */
void parseSerial() {
#ifdef DEBUG
	Serial.println("Waiting to receive...");
#endif
	while (Serial.available() <= 0 || (char)Serial.read() != START_DELIM) {} //Wait for Serial
	delay(10); //To give the next byte a chance to come in
	int index = 0; //The array index of rawInput
	while (Serial.available() > 0) {
		char singleChar = (char) Serial.read();

		if (singleChar == END_DELIM) {
			break; //Stop receiving at the end of the message
		}
		rawInput[index] = singleChar;
		index++;
		delay(1); //Remove? Testing needed
	}
	delay(1); //Remove? Testing needed
#ifdef DEBUG
	Serial.print("Values received: ");
	Serial.println(rawInput);
#endif
}

/**
 * Parses the thrust-motor values from the rawInput[] char
 * array and assigns them to int values in motorValues[]
 */
void parseThrustMotorVals() { //Needs work for 5th and 6th motors, which have the same val
	char motorVal[4]; //4 because null terminator? Testing needed
	for (int i = 0; i < (NUM_HMOTORS + 1); i++) { //All the vertical motors have the same value
		for (int j = 0; j < 3; j++) { //j is the digit. Every number is a 3 digit number.
			motorVal[j] = rawInput[4*i + j];
		}
		motorVal[3] = '\0';
		if (i < NUM_HMOTORS) {
			motorValues[i] = (atoi(motorVal) - 256); //Subtract 256 because range is (-255, 255)
		}
		else { //Gives every vertical value the same value on the last run through the for loop
			for (int n = 0; n < NUM_VMOTORS; n++) {
				motorValues[NUM_HMOTORS + n] = (atoi(motorVal) - 256);
			}
		}
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
 * Parses the PID state
 */
void parsePID() {
	char state = rawInput[PID_START];
	pidState = (state == '0')? false : true; //If state == '0', pidState == false, otherwise true
}

/**
 * Parses the servo values from the rawInput[] char
 * array and assigns them to int values in servoValues[]
 */
void parseServoVals() {
	char servoVal[4]; //4 because null terminator? Testing needed
	for (int i = 0; i < NUM_SERVOS; i++) {
		for (int j = 0; j < 3; j++) { //j is the digit. Every number is a 3 digit number.
			//Starts after 6 motors and the pid state
			servoVal[j] = rawInput[SERVO_START + (i*4) + j];
		}
		servoVal[3] = '\0';
		servoValues[i] = atoi(servoVal);
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
 * Writes the motor speed to the specified motor and changes its
 * direction depending on the sign of the speed (+/-)
 * @param  int motorNum      The ID of the motor
 * @param  int motorSpeed    A value in range [-255, 255]
 */
void controlThrustMotor(int motorNum, int motorSpeed) {
	motorSpeed = constrain(motorSpeed, -255, 255); //Ensures the speed is in range
	if (motorSpeed > STALL_THRESHOLD) {
		digitalWrite(motorDirPins[motorNum], FORWARD); //Sets the motor to forward
	}
	else if (motorSpeed < (-1 * STALL_THRESHOLD)) {
		motorSpeed *= -1; //motorSpeed is a negative value, but analogWrite() only reads positive values
		digitalWrite(motorDirPins[motorNum], BACKWARD); //Sets the motor to backwards
	}
	else {
		motorSpeed = 0; //If the motorspeed is not above the threshold, it should not turn on
		digitalWrite(motorDirPins[motorNum], FORWARD);
	}
	analogWrite(motorThrustPins[motorNum], motorSpeed); //Writes the speed to the motor
#ifdef DEBUG
	Serial.print("DEBUG: Writing ");
	Serial.print(motorSpeed);
	Serial.print(" to motor on pin ");
	Serial.println(motorThrustPins[motorNum]);
#endif
}


/**
 * Writes the servo value to the specified servo motor
 * @param  int servoNum      The ID of the servo
 * @param  int servoVal      The degree turn of the servo
 */
void controlServoMotor(int servoNum, int servoVal) {
	servoVal = constrain(servoVal, 0, 179); //Ensures value is in range
	Servo servo = servos[servoNum];
	servo.write(servoVal); //Writes the value to the servo
#ifdef DEBUG
	Serial.print("DEBUG: Writing ");
	Serial.print(servoValues[i]);
	Serial.print(" to servo ");
	Serial.println(i+1);
#endif
}
