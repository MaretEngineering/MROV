
#define MOTOR_1_THRUST 11
#define MOTOR_2_THRUST 9
#define MOTOR_3_THRUST 7
#define MOTOR_4_THRUST 5
#define MOTOR_5_THRUST 13
#define MOTOR_6_THRUST 13

#define MOTOR_1_DIR 10
#define MOTOR_2_DIR 8
#define MOTOR_3_DIR 60
#define MOTOR_4_DIR 4
#define MOTOR_5_DIR 2
#define MOTOR_6_DIR 2

float motor1const = 1.0;
float motor2const = 1.0;
float motor3const = 1.0;
float motor4const = 1.0;

void setup(){
  pinMode(MOTOR_1_THRUST, OUTPUT);
  pinMode(MOTOR_1_DIR, OUTPUT);
  pinMode(MOTOR_2_THRUST, OUTPUT);
  pinMode(MOTOR_2_DIR, OUTPUT);
  pinMode(MOTOR_3_THRUST, OUTPUT);
  pinMode(MOTOR_3_DIR, OUTPUT);
  pinMode(MOTOR_4_THRUST, OUTPUT);
  pinMode(MOTOR_4_DIR, OUTPUT);
  pinMode(MOTOR_5_THRUST, OUTPUT);
  pinMode(MOTOR_5_DIR, OUTPUT);
  pinMode(MOTOR_6_THRUST, OUTPUT);
  pinMode(MOTOR_6_DIR, OUTPUT);
  
  motor1const = 1.0;
  motor2const = 1.0;
  motor3const = 1.0;
  motor4const = 1.0;
}

void loop() {
  forward();
  left();
  right();
  back();
  up();
}

void forward() {
  analogWrite(MOTOR_2_THRUST, 255*motor2const);
  analogWrite(MOTOR_3_THRUST, 255*motor4const);
}

void left() {
  analogWrite(MOTOR_1_THRUST, 255*motor2const);
  analogWrite(MOTOR_3_THRUST, 255*motor3const);
}

void right() {
  analogWrite(MOTOR_2_THRUST, 255*motor1const);
  analogWrite(MOTOR_4_THRUST, 255*motor3const);
}

void back() {
  analogWrite(MOTOR_1_THRUST, 255*motor1const);
  analogWrite(MOTOR_4_THRUST, 255*motor4const);
}

void up() {
  analogWrite(MOTOR_5_THRUST, 255);
  analogWrite(MOTOR_6_THRUST, 255);
}
