char no;
int i = 0;

void setup() {
  
}

void loop() {
  while (Serial.available() <= 0) {
    String num = "";
    no = Serial.read();
    if (no != "!") {return;)
  while (Serial.available() > 0) {
    no = Serial.read();
    if (no == "//") {
      motors[i] = atoi(num);
      num = "";
      i++;
    }
    else if (no == "{") {
      break;
    }
    
    if (no =="|") {
      buttons[i] = atoi(num);
      num = "";
      i++;
    }
    else if (no == "$") {
      break;
    }
    num = num + no;
