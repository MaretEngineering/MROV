import processing.serial.*;

Serial port_data;
Serial port_motors;

float read_data = 0;
int motorVal = 80;
String input = "";

void setup() {
    size(640, 360);
    stroke(150);
    textSize(32);
    
    port_data = new Serial(this, Serial.list()[Serial.list().length - 2], 115200);
    port_motors = new Serial(this, Serial.list()[Serial.list().length - 1], 115200);
    delay(1000);
}

void draw() {
    if (port_data.available() > 0) {
        input = port_data.readString();
      //  read_data = Float.parseFloat(input);
    }
    println(input + ", " + motorVal);
    port_motors.write(motorVal);
}

void keyPressed() {
    if (key == CODED) {
        if (keyCode == UP) {
            motorVal += 5;
        }
        else if (keyCode == DOWN) {
            motorVal -= 5;
        }
    }
    else if (key == ' ') {
        motorVal = 80;
    }
}
