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
ControllStick trigs;
ControllButton aButton; //a
ControllButton bButton; //b
ControllButton xButton; //x
ControllButton yButton; //y
ControllButton dpadUp;
ControllButton dpadDown;
ControllButton xboxButton;

int joy1x = 0;
int joy1y = 0;
int joy2x = 0; 
int joy2y = 0;

int r_trig = 0;
int l_trig = 0;

//Thrust Values
int[] thrustValues;

boolean aButtonValue     = false;
boolean bButtonValue     = false;
boolean xButtonValue     = false;
boolean yButtonValue     = false;
boolean xboxButtonValue  = false;

// Tuning Constants
double Kx = 1.0000;
double Ky = 1.0000;
//double aConst = 1/(sqrt(2)*2*(Kx + Ky));
//double bConst = 1/(sqrt(2)*2*(Ky - Kx));
double aConst = 1;
double bConst = 1;
double aConstR = 1;
double bConstR = 1;
int DEBOUNCE_TIME = 175;

int camServo1Val = 0;
int camServo2Val = 0;
int clawServoVal = 0;



void setup(){
  size(1500, 900);
  stroke(150);
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
  
  // Set up triggers
  trigs = joypad.getStick(2);
  trigs.setMultiplier(-128);
  
  //Setup buttons
  aButton = joypad.getButton(11);
  bButton = joypad.getButton(12);
  xButton = joypad.getButton(13);
  yButton = joypad.getButton(14);
  xboxButton = joypad.getButton(10);
  dpadUp = joypad.getButton(0);
  dpadDown = joypad.getButton(1);

  //Set up serial
  println(Serial.list());
  //port = new Serial(this, Serial.list()[10], 115200);

}

void draw() {
  background(0.5);
  
  // Joysticks!
  joy1y = (int) joy1.getX();
  joy1x = (int) joy1.getY();
  joy2y = (int) joy2.getX();
  joy2x = (int) joy2.getY(); 
  
  text("x joystick 1: " + str(joy1x), 50, 50);
  text("y joystick 1: " + str(joy1y), 50, 100);
  text("x joystick 2: " + str(joy2x), 50, 150); 
  text("y joystick 2: " + str(joy2y), 50, 200);
  
  // Triggers and Buttons!
  r_trig = (int) trigs.getX() + 128;
  l_trig = (int) trigs.getY() + 128;
  
  aButtonValue = aButton.pressed();
  bButtonValue = bButton.pressed();
  xButtonValue = xButton.pressed();
  yButtonValue = yButton.pressed();
  
  // Toggle PID
  if (xboxButton.pressed()) {
    xboxButtonValue = !xboxButtonValue;
    delay(DEBOUNCE_TIME);
  }
  
  text("a Toggle: " + str(aButtonValue), 50, 250);
  text("b Button: " + str(bButtonValue), 50, 300);
  text("x Button: " + str(xButtonValue), 50, 350);
  text("y Button: " + str(yButtonValue), 50, 400);
  text("Dpad Up: " + str(dpadUp.pressed()), 50, 450);
  text("Dpad Down: " + str(dpadDown.pressed()), 50, 500);
  text("PID (XBox): " + str(xboxButtonValue), 50, 550);
  
  // Deal w/ Servos (These calculations mirror those done on the craft)
  //  Camera Servo 1
  if (aButtonValue) {
    camServo1Val -= 1;
    camServo1Val = constrain(camServo1Val, 0, 179);
  }
  if (bButtonValue) {
    camServo1Val += 1;
    camServo1Val = constrain(camServo1Val, 0, 179);
  }
  
  // Camera Servo 2
  if (xButtonValue) {
    camServo2Val -= 1;
    camServo2Val = constrain(camServo2Val, 0, 179);
  }
  if (yButtonValue) {
    camServo2Val += 1;
    camServo2Val = constrain(camServo2Val, 0, 179);
  }
  // Claw Servo
  if (dpadDown.pressed()) {
    clawServoVal -= 1;
    clawServoVal = constrain(clawServoVal, 0, 179);
  }
  if (dpadUp.pressed()) {
    clawServoVal += 1;
    clawServoVal = constrain(clawServoVal, 0, 179);
  }
  
  text("Camera Tilt  : " + str(camServo1Val), 50, 600);
  text("Camera Roll : " + str(camServo2Val), 50, 650);
  text("Claw Open   : " + str(clawServoVal), 50, 700);
  
  //Calculate thrust vectors
  if (joy1y != 0 || joy1x != 0) {
    thrustValues = getTranslation(joy1y, -joy1x);
    // Map thrust vectors to between -256 and +256
    for (int i = 0; i < 4; i++) {
      if (thrustValues[i] > 512 || thrustValues[i] < -512) { println("WHAT THE FRACK ARE YOU DOING? Motor #" + str(i) + " is " + str(thrustValues[i])); }
      thrustValues[i] = (int) map(thrustValues[i], -512, 512, -256, 256);
    
  }
  } else {
    thrustValues = getRotation(-joy2x);
  }
  
  
  text("Right Trigger: " + str(r_trig), 900, 50);
  text("Left Trigger:  " + str(l_trig), 900, 100);
  
  //  Draw Motor A
  line (500, 100, 500+thrustValues[0]*cos(radians(225)), 100-thrustValues[0]*sin(radians(225)));
  //  Draw Motor B
  line (750, 100, 750+thrustValues[1]*cos(radians(315)), 100-thrustValues[1]*sin(radians(315)));
  //  Draw Motor C
  line (500, 350, 500+thrustValues[2]*cos(radians(135)), 350-thrustValues[2]*sin(radians(135)));
  //  Draw Motor D
  line (750, 350, 750+thrustValues[3]*cos(radians(45)), 350-thrustValues[3]*sin(radians(45)));
  
  // Draw Depth control lines
  int diff = l_trig - r_trig;
  line (1300, 350, 1300, 350 - (diff));
  
  String toSend = "!";

  // makes all thrust value strings going out 3 characters long + the "/"
  for (int counter = 0; counter < 4; counter++) {
   thrustValues[counter] += 256;
   if (thrustValues[counter] < 10) {
        toSend+= "00" + str(thrustValues[counter]) + "/";
   }
   if (thrustValues[counter] >=10 && thrustValues[counter] < 100) {
        toSend+= "0" + str(thrustValues[counter]) + "/"; 
   }
   if (thrustValues[counter] >= 100) {
      toSend+= str(thrustValues[counter]) + "/";
   }
  }
    
  //makes the diff value 3 characters long + "/"
  diff += 256;
  if (diff < 10) {
    toSend+= "00" + str(diff)  + "/";
  }
  if (diff >= 10 && diff < 100) {
    toSend+= "0" + str(diff) + "/"; 
  }
  if (diff >= 100) {
    toSend+= str(diff) + "/";
  } 

  toSend += "{";
  toSend += str(int(aButtonValue)) + "|";
  toSend += str(int(bButtonValue)) + "|";
  toSend += str(int(xButtonValue)) + "|";
  toSend += str(int(yButtonValue)) + "|";
  toSend += str(int(dpadDown.pressed())) + "|";
  toSend += str(int(dpadUp.pressed())) + "|";
  toSend += str(int(xboxButtonValue)) + "|";
  toSend += "$";
  
  text(toSend, 50, 850);
  
  /*
  port.write(toSend);
  
  //Read in data from arduino
  String val;
  if(port.available() > 0) {
    val = port.readStringUntil('\n');
    println(val);
  }
  */
  
  delay(10);
}

int[] getTranslation(int x, int y){
  int[] vals = new int[4];
  int a = (int)((x + y)*aConst);
  int b = -(int)((y - x)*bConst);
  int d = -a;
  int c = -b;
  vals[0] = a;
  vals[1] = b;
  vals[2] = c;
  vals[3] = d;
  return vals;
}

int[] getRotation(int x) {
  int[] vals = new int[4];
  int a = (int)(x*aConstR);
  int b = -(int)(x*bConstR);
  int d = a;
  int c = b;
  vals[0] = a;
  vals[1] = b;
  vals[2] = c;
  vals[3] = d;
  return vals;
}
