import processing.serial.*;

Serial s;

void setup() {
  s = new Serial(this, Serial.list()[7], 115200);
  delay(2000);
}

void draw() {
  String toSend = "!123/123/123/123/123/123/000/000$";
  s.write(toSend);
  println(toSend);
  delay(100);
}
