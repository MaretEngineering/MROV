

void setup() {
  Serial.begin(9600);
}

void loop() {
  String input = read_input(5);
}

// Reads in max_length characters from serial
//   Waits for start signal ('!') and reads until either max_length or end signal ('$')
char* read_input(int max_length) {
  int index = 0;
  char data[max_length];
  
  // Wait for "1"
  while (Serial.available() == 0 || Serial.read() != '!') {
  }
  while (Serial.available() > 0 && index < max_length) {
    char input = Serial.read();
    if (input == '$') { break; }
    data[index] = input;
    index ++;
  }
  return data;
}
