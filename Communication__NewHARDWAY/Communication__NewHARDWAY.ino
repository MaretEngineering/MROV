 //all of the stuff together


/* !motor/motor/{Button|Button|$ */


void setup() {
  Serial.begin(9600);
}

void loop() {
  String input = read_input(31);
}

char* read_input (int max_length) {
  int index = 0;
  char data[max_length];
  
  while (Serial.available() <= 0 || Serial.read() != '!') {
 }
 while (Serial.available() > 0 && index < max_length) {
   char input = Serial.read();
   if (input == '$') {break; }
   data[index] = input;
   index ++;
 }
 return data;
}

int* motorValue(char inputData[], int whichMotor) {
  placeHolder = 0;
  
  if (whichMotor == 1) {
    placeHolder = 1;
  }
  else if (whichMotor == 2) {
    placeHolder = 5;
  }
  else if (whichMotor == 3) {
    placeHolder = 9;
  }
  else if (whichMotor == 4) {
    placeHolder = 13;
  }
  else if (whichMotor == 5) {
    placeHolder = 17;
  }
  return int motorValue = atoi(inputData.substring(placeholder, placeholder+3)
  
   
   
