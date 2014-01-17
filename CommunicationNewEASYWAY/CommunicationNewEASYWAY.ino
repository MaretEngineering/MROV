//Ethan's BOSS WAY of communication!!!!!

void setup() {
  Serial.begin(9600);
}

void loop() {
  int initialVal = Serial.read();
  
  if (initialVal%257 == 0) {
    motor1 = ((initialVal%257)-1);
  }
  if (initialVal%258 == 0) {
    motor2 = ((
