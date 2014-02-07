 //all of the stuff together


/* !motor/motor/{Button|Button|$ */
// 0123456789012345678901234567890
// !XXX/XXX/XXX/XXX/XXX/{X|X|X|X|$
int led = 10;

void setup() {
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
  motorValue(input, 1);
  motorValue(input, 2);
  motorValue(input, 3);
  motorValue(input, 4);
  motorValue(input, 5);
  buttonValue(input, 1);
  buttonValue(input, 2);
  buttonValue(input, 3);
  buttonValue(input, 4);
  //analogWrite(led, motorValue(input, 1));
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
  int placeHolder = 0;
  
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
  Serial.println("placeholder=" + placeHolder); //debugging
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
  Serial.println("placeholder=" + placeHolder); //debugging
  Serial.println(inputData);
  String val = inputData.substring(placeHolder, placeHolder+1);
  Serial.println(val); //debugging
  return val.toInt();
}


