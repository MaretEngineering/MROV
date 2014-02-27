#include <Servo.h>

//###################################
//###################################
// Constants
//###################################
//###################################

//************************************
// Miscellaneous
//************************************

#define FORWARD HIGH
#define BACKWARD LOW

//************************************
// Motor pins
//************************************

#define NUM_MOTORS 6

//Motor thruster pins
#define MT1t_PIN 3
#define MT2t_PIN 5
#define MT3t_PIN 6
#define MT4t_PIN 9
#define MT5t_PIN 10
#define MT6t_PIN 11

int motor_thrust_pins[] = {MT1t_PIN, MT2t_PIN, MT3t_PIN, MT4t_PIN, MT5t_PIN, MT6t_PIN};

//Motor direction pins
#define MT1d_PIN 2
#define MT2d_PIN 4
#define MT3d_PIN 7
#define MT4d_PIN 8
#define MT5d_PIN 12
#define MT6d_PIN 13

int motor_reverse_pins[] = {MT1d_PIN, MT2d_PIN, MT3d_PIN, MT4d_PIN, MT5d_PIN, MT6d_PIN};


//************************************
// Servo pins
//************************************

// Uncomment || to enable servo control
//           \/
//#define USE_SERVOS

#ifdef USE_SERVOS
  #define SERVO_1_PIN 0
  #define SERVO_2_PIN 0
  #define SERVO_3_PIN 0
#endif


//###################################
//###################################
// Global Variables
//###################################
//###################################

//************************************
// Motor Values (signed 8-bit integers)
//************************************

int motor_values[] = {0, 0, 0, 0, 0, 0};


//************************************
// Servo Values (unsigned 0-90 integers)
//************************************
#ifdef USE_SERVOS
  int servo_values[] = {0, 0, 0};

//************************************
// Servo Values (unsigned 0-90 integers)
//************************************

  Servo servo1;
  Servo servo2;
  Servo servo3;
#endif

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
  Serial.begin(115200);
  
  // Set pins to output
  pinMode(13, OUTPUT);
  for (int i = 0; i < NUM_MOTORS; i++) {
    pinMode(motor_thrust_pins[i], OUTPUT);
    pinMode(motor_reverse_pins[i], OUTPUT);
  }
  
#ifdef USE_SERVOS
  // Attach servos
  servo1.attach(SERVO_1_PIN);
  servo2.attach(SERVO_2_PIN);
  servo3.attach(SERVO_3_PIN);
#endif
  
  //*********************************
  // Test Systems
  //*********************************
  
  // Blink to acknowledge
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  
  //Test motors
  for (int i = 0; i < NUM_MOTORS; i++) {
    for (int j = 0; j < 256; j++) {
      analogWrite(motor_thrust_pins[i], j);
      delay(1);
    }
    
    for (int j = 255; j >= 0; j--) {
      analogWrite(motor_reverse_pins[i], j);
      delay(1);
    }
    
    
  }
  
#ifdef USE_SERVOS
  //Test servos
  //  #1
  servo1.write(0);
  delay(500);
  servo1.write(70);
  //  #2
  servo2.write(0);
  delay(500);
  servo2.write(70);
  //  #3
  servo3.write(0);
  delay(500);
  servo3.write(70);
#endif
  
} // End setup

void loop() {
  //*********************************
  // Write out motor values
  //*********************************
  
  for (int i = 0; i < NUM_MOTORS; i++){
    int a = motor_values[i];
    if (a < 0) {
      a = -a;
      a = constrain(a, 50, 256);
      analogWrite(motor_reverse_pins[i], a);
    } else if (a > 0) {
      a = constrain(a, 50, 256);
      analogWrite(motor_thrust_pins[i], a);
    } else {
      analogWrite(motor_thrust_pins[i], 0);
    }
  }
  
  
  //************************************
  // Write out servo values
  //************************************
#ifdef USE_SERVOS
  servo1.write(servo_values[0]);
  servo2.write(servo_values[1]);
  servo3.write(servo_values[2]);
#endif
}

// serialEvent is called by the arduino whenever new Serial data is recieved between each loop()
void serialEvent() {
  while (Serial.available() <= 0 || (char)Serial.read() != '!') {
    // Wait for start signal...
  }
  delay(1); // To give the next byte a chance to come in
  
  // By this point we know that either there is nothing in the RX buffer
  //   or the first byte in the buffer is the second byte of the message
  //   (as we already read the first byte to see if it was a '!')
  String inputString = "";
  while (Serial.available() > 0) {
    char in_char = (char)Serial.read();
    
    if (in_char == '$') {
      break; // Stop byte recieved, breaking out
    }
    
    inputString += in_char;
  }
  
  
  //************************************
  // Parse the received values
  //
  //  Reference Message:
  //    123/123/123/123/123/123/000/000/000/$
  //************************************
  
  for (int i = 0; i < NUM_MOTORS; i++) {
    int m_val = inputString.substring(i*4, i*4 + 3).toInt();
    motor_values[i] = m_val - 256;
  }
  
#ifdef USE_SERVOS
  for (int i = 0; i < 3; i++) {
    int s_val = inputString.substring(24 + i*4, 24 + i*4 + 3).toInt();
    servo_values[i] = s_val;
  }
#endif
}
