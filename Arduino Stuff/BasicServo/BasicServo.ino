#include <Servo.h>

Servo myservo;

void setup() { 
  myservo.attach(10);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    int a = Serial.parseInt();
     myservo.write(a);
  }
}

