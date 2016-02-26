#include <Servo.h>

#define SERVO_1_PIN 10

Servo servo1;

void setup() {

  Serial.begin(9600);
  servo1.attach(SERVO_1_PIN);
  
}

void loop() {
  if(Serial.available() > 0) {
    delay(5);
    int a = Serial.parseInt();
    servo1.write(a);
    Serial.println(a);
  }
}

