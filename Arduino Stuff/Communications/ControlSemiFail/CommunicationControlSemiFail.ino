 //all of the stuff together


/* !motor/motor/{Button|Button|$*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  
}



String read_from_serial() {
  String singleCharFromSerial = "";
  String allCharsTogetherAsAString = "";
  while (Serial.available() > 0) {
    singleCharFromSerial = (char) Serial.read();
      if (singleCharFromSerial == "!") {
        while (allCharsTogetherAsAString != "$") {
          allCharsTogetherAsAString = allCharsTogetherAsAString + singleCharFromSerial;
        }
      }
  }
  return allCharsTogetherAsAString;
}
      
    



    
