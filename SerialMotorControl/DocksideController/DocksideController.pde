import procontroll.*;
import net.java.games.input.*;
import processing.serial.*;

// Notes on the controller:
//   - Axis are inverted, so we used negative multipliers
//   - The "x" axis is the y-axis irl, but the controller's wierd like that

Serial port;

ControllIO controllIO;
ControllDevice joypad;
ControllStick joy1;

int joy1x = 0;

void setup(){
  size(600, 600);
  noStroke();
  textSize(32);
  
  //Set up controller
  controllIO = ControllIO.getInstance(this);
  joypad = controllIO.getDevice("Afterglow Gamepad for Xbox 360");
 
  //Set up joystick w/ proper multiplier (negative because otehrwise it's inverted)
  joy1 = joypad.getStick(0);
  joy1.setMultiplier(-255);
  
  //Set up serial
  println(Serial.list());
  port = new Serial(this, Serial.list()[10], 9600);
}

void draw() {
  background(0.5);
  
  joy1x = (int) joy1.getX();
  
  text("x: " + str(joy1x), 50, 50);
  port.write(str(joy1x));
  
  delay(10);
}

//You can also control it with the keyboard (using the up and down arrow keys)
void keyPressed(){
  if (key == CODED) {
    if (keyCode == UP) {
      joy1x ++;
      background(0.5);
    } else if (keyCode == DOWN) {
      joy1x --;
      background(0.5);
    }
    
    joy1x = constrain( joy1x, -255, 255 );
    
  }
}
  
