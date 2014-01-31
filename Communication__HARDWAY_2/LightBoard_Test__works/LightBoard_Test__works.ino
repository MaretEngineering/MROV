//all of the stuff together


/* !motor/motor/{Button|Button|$ */

// !XXX/XXX/XXX/XXX/XXX/{X|X|X|X|$
int led1 = 10;
int led2 = 11;
int led3 = 12;
int led4 = 13;
int led5 = 8;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  Serial.println("Hello, world");
  Serial.begin(9600);
  //pinMode(led, OUTPUT);
}

void loop() {
  String input = "blah!";
  Serial.println("Reading input...");
  input = read_input();
  Serial.print("Input read: ");
  Serial.println(input);
  int motor1 = motorValue(input, 1);
  int motor2 = motorValue(input, 2);
  int motor3 = motorValue(input, 3);
  int motor4 = motorValue(input, 4);
  int motor5 = motorValue(input, 5);
  int button1 = buttonValue(input, 1);
  int button2 = buttonValue(input, 2);
  int button3 = buttonValue(input, 3);
  int button4 = buttonValue(input, 4);
  analogWrite(led5, motor2);
  analogWrite(led4, motor1);
  analogWrite(led3, motor3);
  analogWrite(led2, motor4);
  analogWrite(led1, motor5);
  //delay(500);
  
}

String read_input () {
  int index = 0;
  char data[32];
  
  while (Serial.available() <= 0 || Serial.read() != '!') {}
  Serial.println("Data incoming!");
  delay(1);
  while (Serial.available() > 0) {
    char input = Serial.read();
    Serial.print("input");
    if (input == '$') {Serial.println("End of string"); break; }
    data[index] = input;
    index ++;
    delay(1);
 }
 data[index] = '\0';
 Serial.println(data);
 Serial.println("------------------------- DATA: " + String(data));
 return String(data);
}


int motorValue(String inputData, int motorNumber) {
  Serial.println("Hello");
  int placeHolder = -1;
  
  switch (motorNumber) {
    case 1:
      placeHolder = 0;
      break;
    case 2:
      placeHolder = 4;
      break;
    case 3:
      placeHolder = 8;
      break;
    case 4:
      placeHolder = 12;
      break;
    case 5:
      placeHolder = 16;
      break;
  }
  Serial.println(placeHolder); //debugging
  Serial.println(inputData);
  String val = inputData.substring(placeHolder, placeHolder + 3);
  Serial.println(val); //debugging
  return val.toInt();
  //Serial.println(val.toInt());
}

int buttonValue(String inputData, int buttonNumber) {
  Serial.println("Hello Button");
  int placeHolder = 0;
  
  switch (buttonNumber) {
    case 1:
      placeHolder = 21;
      break;
    case 2:
      placeHolder = 23;
      break;
    case 3:
      placeHolder = 25;
      break;
    case 4:
      placeHolder = 27;
      break;
  }
  Serial.println(placeHolder); //debugging
  Serial.println(inputData);
  String val = inputData.substring(placeHolder, placeHolder+1);
  Serial.println(val); //debugging
  return val.toInt();
}


  
