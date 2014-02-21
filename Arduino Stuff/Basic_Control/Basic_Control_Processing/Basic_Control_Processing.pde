import processing.serial.*;

Serial botInstruction;
int basicDir = 0;

void setup() {
  size(640, 360);
  botInstruction = new Serial(this, "/dev/tty.usbmodem1411", 9600);
}

void draw() {
  background(0);
  fill(255);
  textAlign(CENTER);
  text("Hit spacebar to stop", width/2, height/2);
  text("Hit up to go forward", width/2, height/2 + 20);
  text("Hit back to go backwards", width/2, height/2 + 40);
  text("Hit left to go left", width/2, height/2 + 60);
  text("Hit right to go right", width/2, height/2 + 80);
  text("Type 'w' to go up", width/2, height/2 +100);
  text("Type 's' to go down", width/2, height/2 +120);
  
  // Sends a value to serial
  botInstruction.write(basicDir);
}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP) {
      basicDir = 1;
    }
    else if (keyCode == DOWN) {
      basicDir = 2;
    }
    else if (keyCode == LEFT) {
      basicDir = 3;
    }
    else if (keyCode == RIGHT) {
      basicDir = 4;
    }
  }
  else if (key == ' ') {
    basicDir = 0;
  }
  else if (key == 'w') {
    basicDir = 5;
  }
  else if (key == 's') {
    basicDir = 6;
  }
}
  
  
  
  
  
