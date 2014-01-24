 //all of the stuff together


/* !motor/motor/{Button|Button|$ */

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


int motorValue(String inputData, int motorNumber) {
  Serial.println("Hello");
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
  Serial.println(placeHolder); //debugging
  Serial.println(inputData);
  String val = inputData.substring(0, 3);
  Serial.println(val); //debugging
  return val.toInt();
  //Serial.println(val.toInt());
}

   
   
