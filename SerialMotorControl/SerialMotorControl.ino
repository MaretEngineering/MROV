
#define NUM_MOTORS 4
#define NUM_BUTTONS 4

int motorPins[NUM_MOTORS] = {9, 3, 5, 6};
int motorPower[NUM_MOTORS];
int buttonPins[NUM_MOTORS] = {13, 12, 11, 8};
int buttonStates[NUM_BUTTONS] = {0, 0, 0, 0};

void setup() {
  //Set up serial
  Serial.begin(9600);  //  opens serial port 
  
  //Set up pins
  motorPins[0] = 9;
  motorPins[1] = 3;
  motorPins[2] = 5;
  motorPins[3] = 6;
  //motorPins[4] = 10;
  //motorPins[5] = 11;
    
  //Set up pins and arrays
  for (int n = 0; n < NUM_MOTORS; n++) {
    pinMode(motorPins[n], OUTPUT);
    pinMode(buttonPins[n], OUTPUT);
    motorPower[n] = 0;
  }
 
 digitalWrite(buttonPins[0], HIGH);
 
 delay(1000);
  
 }

void loop () {
  
  readCharArrayFromSerial();
  
  //Constrain to make sure nothing breaks
  for ( int h = 0; h < NUM_MOTORS; h++ ) {
    if ( motorPower[h] > 0 ) { motorPower[h] = map(motorPower[h], 0, 255, 50, 255); }
    else if (motorPower[h] < 0 ) { motorPower[h] = map(motorPower[h], -255, 0, -255, 50); }
  }
  
  for ( int i = 0; i < NUM_MOTORS; i++) {
    analogWrite( motorPins[i], abs(motorPower[i]) );
  }
  
  for ( int i = 0; i < NUM_BUTTONS; i++) {
    if ( buttonStates[i] == 1 ) {
      digitalWrite( buttonPins[i], HIGH);
    } else {
      digitalWrite( buttonPins[i], LOW);
    }
  }
  
  
  delay(10);
}

//Utility method to read a string (in the form of a char array) from Serial
void readCharArrayFromSerial() {
  int motorDelimiter = (int)'/';
  int buttonDelimiter = (int)'|';

  int switchDelimeter = (int)'{';


  
  //Scan for motor values first
  int index = 0;
  String input = "";
  
  if ( Serial.available() > 0 ) {
    if ( Serial.read() != (int)'!' ) { return; }
  }
  
  while ( Serial.available() > 0 && index < 4 ) {
    int c = Serial.read();
    
    
    //Handle moving on to the next value if motor delimeter is recieved
    if ( c == motorDelimiter ) {
      //Copy string into char buffer
      int motorInputLength = input.length() + 1; // + 1 to make room for null terminator
      char motorInputBuffer[motorInputLength];
      input.toCharArray(motorInputBuffer, motorInputLength);
      motorPower[index] = atoi(motorInputBuffer);
      
      index ++;
      input = "";
    } else {
      
      //Handle other delimeters
      if ( c == switchDelimeter) { break; }
    
      //If no delimeter found, add the char to the input string
      input += (char) c;
    }
  }
  
  
  //Then scan for button values
  index = 0;
  input = "";
  while ( Serial.available() > 0 && index < 4) {
    int b = Serial.read();
    
    //Handle button delimeters
    if ( b == buttonDelimiter ) {
      int buttonInputLength = input.length() + 1;
      char buttonInputBuffer[buttonInputLength];
      input.toCharArray(buttonInputBuffer, buttonInputLength);
      buttonStates[index] = atoi(buttonInputBuffer);
      
      index ++;
    } else {
      input += (char) b;
    }
    
  }
  
}
