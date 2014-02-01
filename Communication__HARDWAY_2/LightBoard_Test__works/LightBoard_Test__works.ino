//all of the stuff together


/* !motor/motor/{Button|Button|$ */

// !XXX/XXX/XXX/XXX/XXX/{X|X|X|X|$
int led1 = 5;
int led2 = 6;
int led3 = 10;
int led4 = 11;
int led5 = 9;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  Serial.println("Hello, world");
  Serial.begin(115200);
  //pinMode(led, OUTPUT);
}

void loop() {
  String input = "blah!";
  Serial.println("Reading input...");
  input = read_input();
  Serial.print("Input read: ");
  Serial.println(input);
  int motor1 = abs(motorValue(input, 1) - 256);
  int motor2 = abs(motorValue(input, 2) - 256);
  int motor3 = abs(motorValue(input, 3) - 256);
  int motor4 = abs(motorValue(input, 4) - 256);
  int motor5 = abs(motorValue(input, 5) - 256);
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
  delayMicroseconds(80);
  while (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '$') { break; }
    data[index] = input;
    index ++;
    delayMicroseconds(80);
 }
 data[index] = '\0';
 return String(data);
}


int motorValue(String inputData, int motorNumber) {
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

  String val = inputData.substring(placeHolder, placeHolder + 3);
  return val.toInt();
  //Serial.println(val.toInt());
}

int buttonValue(String inputData, int buttonNumber) {
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

  String val = inputData.substring(placeHolder, placeHolder+1);
  return val.toInt();
}


  
