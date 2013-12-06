import procontroll.*;
import net.java.games.input.*;

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
ControllStick joy2;
ControllButton aButton; //a
ControllButton bButton; //b
ControllButton xButton; //x
ControllButton yButton; //y

int joy1x = 0;
int joy1y = 0;
int joy2x = 0; 
int joy2y = 0;
boolean aButtonValue = false;
boolean bButtonValue = false;
boolean xButtonValue = false;
boolean yButtonValue = false;


int aButtonInt = 0;
int bButtonInt = 0;
int xButtonInt = 0;
int yButtonInt = 0;



void setup(){
  size(1000, 700);
  noStroke();
  textSize(32);
  
  
  //Set up controller
  controllIO = ControllIO.getInstance(this);
  joypad = controllIO.getDevice("Afterglow Gamepad for Xbox 360");
 
  //Set up joystick w/ proper multiplier for x and y values (negative because otehrwise it's inverted)
  joy1 = joypad.getStick(0);
  joy1.setMultiplier(-255);
 
  
  //Set Up 2nd joystick w/ proper multiplier for x and y values (see above for why)
  joy2 = joypad.getStick(1); 
  joy2.setMultiplier(-255); 
  

  //Set up serial
  println(Serial.list());
  //port = new Serial(this, Serial.list()[10], 9600);
}

void draw() {
  background(0.5);
  
  joy1y = (int) joy1.getX();
  joy1x = -(int) joy1.getY(); //y-values need to be inverted, because the controller is weird like that, as I believe I has already noted
  joy2y = (int) joy2.getX();
  joy2x = -(int) joy2.getY();
 
  aButton = joypad.getButton(11);
  bButton = joypad.getButton(12);
  xButton = joypad.getButton(13);
  yButton = joypad.getButton(14); 

 // aButtonValue = (boolean)joypad.getButton(11).pressed();
  //bButtonValue = (boolean)joypad.getButton(12).pressed();
 // xButtonValue = (boolean)joypad.getButton(13).pressed();
  //yButtonValue = (boolean)joypad.getButton(14).pressed();
 
 // all the buttons are set to true for debugging purposes only 
  aButtonValue = true;
  bButtonValue = true;
  xButtonValue = true;
  yButtonValue = true; 


  
  text("x joystick 1: " + str(joy1x), 50, 50);
  text("y joystick 1: " + str(joy1y), 50, 100);
  text("x joystick 2: " + str(joy2x), 50, 150); 
  text("y joystick 2: " + str(joy2y), 50, 200);
  text ("a Button:" + str(aButtonValue), 50, 250);
  text ("b Button:" + str(bButtonValue), 50, 300);
  text ("x Button:" + str(xButtonValue), 50, 350);
  text ("y Button:" + str(yButtonValue), 50, 400);
  // translates boolean values to a 1 or a 0 
  if (aButtonValue == true) {
   aButtonInt = 1; 
  } else {
   aButtonInt = 0;  
  }
  
  if (bButtonValue == true) {
   bButtonInt = 1; 
  } else {
   bButtonInt = 0;  
  }
  
  if (xButtonValue == true) {
   xButtonInt = 1; 
  } else {
   xButtonInt = 0;  
  }
  
  if (yButtonValue == true) {
   yButtonInt = 1; 
  } else {
   yButtonInt = 0;  
  }

  //Draw thrust vectors
   int[] thrustValues = getTranslation(joy1y, -joy1x);
   text ("a:" + str(thrustValues[0]), 50, 450);
   text ("b:" + str(thrustValues[1]), 50, 500);
   text ("c:" + str(thrustValues[2]), 50, 550);
   text ("y:" + str(thrustValues[3]), 50, 600);
   //  Draw Motor A
   line (500, 100, 500-thrustValues[0]*cos(radians(225)), 100-thrustValues[0]*sin(radians(225)));
   //  Draw Motor B
   line (750, 100, 750+thrustValues[1]*cos(radians(315)), 100+thrustValues[1]*sin(radians(315)));
   //  Draw Motor C
   line (500, 350, 500+thrustValues[2]*cos(radians(135)), 350+thrustValues[2]*sin(radians(135)));
   //  Draw Motor D
   line (750, 350, 750-thrustValues[3]*cos(radians(45)), 350-thrustValues[3]*sin(radians(45)));
  
  String toSend = "!";
  toSend += str(joy1x) + "/";
  toSend += str(joy1y) + "/";
  toSend += str(joy2x) + "/";
  toSend += str(joy2y) + "/";
  toSend += str (aButtonInt) + "|";
  toSend += str (bButtonInt) + "|";
  toSend += str (xButtonInt) + "|";
  toSend += str (yButtonInt) + "|";
  
  //port.write(toSend);
  
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
 
// Tuning Constants
double Kx = 1.0000;
double Ky = 1.0000;
//double aConst = 1/(sqrt(2)*2*(Kx + Ky));
//double bConst = 1/(sqrt(2)*2*(Ky - Kx));
double aConst = 1;
double bConst = 1;
 
int[] getTranslation(int x, int y){
 int[] vals = new int[4];
 int a = (int)((x + y)*aConst);
 int b = (int)((y - x)*bConst);
 int d = -a;
 int c = -b;
 vals[0] = a;
 vals[1] = b;
 vals[2] = c;
 vals[3] = d;
 return vals;
}
