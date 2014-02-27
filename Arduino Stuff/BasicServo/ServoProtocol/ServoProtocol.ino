#include <Servo.h> 

#define servo1Pin 10
#define USE_SERVOS

int servoNumberPin[] = {servo1Pin};
int servoValues[] = {0};

Servo servo1;

String inputData = "";

void setup() {
  initSerial();
  
  servo1.attach(10);  
  
  servo1.write(100);
  delay(500);
  servo1.write(90);
  delay(500);
  servo1.write(120);
}

void loop() {

}

void initSerial() {
  // Initialize the serial
  Serial.begin(9600);
}

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
  
#ifdef USE_SERVOS
  Serial.println(inputString);
  for (int i = 0; i < 1; i++) {
    int s_val = inputString.substring(24 + i*4, 24 + i*4 + 3).toInt();
    servoValues[i] = s_val;
  }
#endif
}
