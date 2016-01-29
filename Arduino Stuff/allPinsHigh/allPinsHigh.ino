#define NUM_MOTORS 6

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
//define MT1d_PIN 39
//define MT2d_PIN 33
//define MT3d_PIN 41
//define MT4d_PIN 31
//define MT5d_PIN 37
//define MT6d_PIN 35

//real pins
#define MT1d_PIN 39
#define MT2d_PIN 33
#define MT3d_PIN 41
#define MT4d_PIN 31
#define MT5d_PIN 37
#define MT6d_PIN 35
int motorDirPins[] = {MT1d_PIN, MT2d_PIN, MT3d_PIN, MT4d_PIN, MT5d_PIN, MT6d_PIN};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < NUM_MOTORS; i++) {
    pinMode(motorThrustPins[i], OUTPUT);
    pinMode(motorDirPins[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    for (int i = 0; i < NUM_MOTORS; i++){
        analogWrite(motorThrustPins[i], 255);
        digitalWrite(motorDirPins[i], HIGH);
    }
}
