#define NUM_MOTORS 6

//real pins
#define MT1t_PIN 8
#define MT2t_PIN 9
#define MT3t_PIN 10
#define MT4t_PIN 11
#define MT5t_PIN 12
#define MT6t_PIN 13
int motorThrustPins[] = {MT1t_PIN, MT2t_PIN, MT3t_PIN, MT4t_PIN, MT5t_PIN, MT6t_PIN};


//real pins
#define MT1d_PIN 24 
#define MT2d_PIN 26
#define MT3d_PIN 28
#define MT4d_PIN 30
#define MT5d_PIN 32
#define MT6d_PIN 34
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
        analogWrite(motorThrustPins[i], 100);
        digitalWrite(motorDirPins[i], HIGH);
    }
}
