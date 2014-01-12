// from example code on arduino about Serial Read
int a = 0; // for incoming serial data
int pin9 = 9; 
int pin3 = 3;

void setup() {
 Serial.begin(9600);  //  opens serial port 
 pinMode(pin9, OUTPUT);
 pinMode(pin3, OUTPUT);
 a = 100;
 }

void loop () {
 /*
  if (Serial.available() > 0) {
   // reads the incoming byte
  a = Serial.read();
  } */
 
 
 
 analogWrite(pin9, abs(a));
 
 
 analogWrite(pin9, abs(a));
 if(a > 0) { digitalWrite(pin3, LOW); }
 else { digitalWrite(pin3, HIGH); }
    
}
