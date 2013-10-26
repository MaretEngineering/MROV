int a = 0; // for incoming serial data
int PWMpin = 9; // for outputing PWM signal
int directionPin = 3; // Written high if input >= 0, low otherwise

void setup() {
  //Set up serial
  Serial.begin(9600);  //  opens serial port 
  
  //Set up pins
  pinMode(PWMpin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  
  //Initialize output to off
  a = 0;
 }

void loop () {
  
  if ( Serial.available() > 0 ) {
    a = atoi( readCharArrayFromSerial() );
  }
  
  analogWrite( PWMpin, abs(a) );
  
  if ( a >= 0 ) { digitalWrite( directionPin, HIGH); }
  else { digitalWrite( directionPin, LOW); }
  
  delay(10);
}

//Utility method to read a string (in the form of a char array) from Serial
char* readCharArrayFromSerial() {
  String input = "";
  int delimiter = (int)'/n';
  
  while ( Serial.available() > 0 ) {
    int c = Serial.read();
    
    if ( c == delimiter ) { break; }
    
    input += (char) c;
  }
  
  //Copy string into char buffer
  int inputLength = input.length() + 1; // + 1 to make room for null terminator
  char inputBuffer[inputLength];
  input.toCharArray(inputBuffer, inputLength);
  
  return inputBuffer;
}
