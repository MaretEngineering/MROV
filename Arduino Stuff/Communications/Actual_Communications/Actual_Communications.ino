 //all of the stuff together


// !motor/motor/{Button|Button|$

<<<<<<< HEAD:Communication__NewHARDWAY/Communication__NewHARDWAY.ino
// !XXX/XXX/XXX/XXX/XXX/{X|X|X|X|$
int led = 10;

void setup() {
  Serial.begin(9600);
  //pinMode(led, OUTPUT);
}

void loop() {
  String input;
  input = read_input();
  motorValue(input, 2);
  //analogWrite(led, motorValue(input, 1));
  //delay(500);
  
=======
#define LEDPIN 9

void setup() {
  Serial.begin(9600);
  
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  char* input = read_input(31);
  
  int motor1 = motorValue(input, 1);
  analogWrite(LEDPIN, motor1);
>>>>>>> 90f5b63649947e92d9acb12766d8d580ce4c44cb:Arduino Stuff/Communications/Actual_Communications/Actual_Communications.ino
}

String read_input () {
  int index = 0;
  char data[31];
  
  while (Serial.available() <= 0 || Serial.read() != '!') {
  }
 while (Serial.available() > 0) {
   char input = Serial.read();
   Serial.println(input);
   if (input == '$') {Serial.println("End of string"); break; }
   data[index] = input;
   index ++;
 }
 Serial.println(data);
 return String(data);
}

<<<<<<< HEAD:Communication__NewHARDWAY/Communication__NewHARDWAY.ino

int motorValue(String inputData, int motorNumber) {
  Serial.println("Hello");
=======
int motorValue(char* inputData, int whichMotor) {
>>>>>>> 90f5b63649947e92d9acb12766d8d580ce4c44cb:Arduino Stuff/Communications/Actual_Communications/Actual_Communications.ino
  int placeHolder = 0;
  
  switch (motorNumber) {
    case 1:
      placeHolder = 1;
      break;
    case 2:
      placeHolder = 5;
      break;
    case 3:
      placeHolder = 9;
      break;
    case 4:
      placeHolder = 13;
      break;
    case 5:
      placeHolder = 17;
      break;
  }
<<<<<<< HEAD:Communication__NewHARDWAY/Communication__NewHARDWAY.ino
  Serial.println(placeHolder); //debugging
  Serial.println(inputData);
  String val = inputData.substring(0, 3);
  Serial.println(val); //debugging
  return val.toInt();
  //Serial.println(val.toInt());
}

   
   
=======
  else if (whichMotor == 4) {
    placeHolder = 13;
  }
  else if (whichMotor == 5) {
    placeHolder = 17;
  }
  
  char val[3];
  for (int i = 0; i < 3; i++) {
    val[i] = inputData[placeHolder + i];
  }
  
  return atoi(val);
}
>>>>>>> 90f5b63649947e92d9acb12766d8d580ce4c44cb:Arduino Stuff/Communications/Actual_Communications/Actual_Communications.ino
