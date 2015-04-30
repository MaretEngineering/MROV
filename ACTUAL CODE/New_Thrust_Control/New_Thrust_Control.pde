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


ControllButton buttons[];

int joy1x = 0;
int joy1y = 0;
int joy2x = 0; 
int joy2y = 0;

int r_trig = 0;
int l_trig = 0;

//Thrust Values
int[] thrustValues;

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

//Vars for reading data from arduino (Note: Ethan added these
//please delete if you dislike)
char arduinoInfo;
char tempChar;

int[] servoValues = {
  90, // Cam 1   0
  90, // Cam 2   1
  90, // Claw    2
  90, // Test    3
};

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
  buttons = new ControllButton[] {
    joypad.getButton(11), // a                    0
    joypad.getButton(12), // b                    1
    joypad.getButton(13), // x                    2
    joypad.getButton(14), // y                    3
    joypad.getButton(0), // dpad up               4
    joypad.getButton(1), // dpad down             5
    joypad.getButton(2), // dpad left             6
    joypad.getButton(3), // dpad right            7
    joypad.getButton(10) // xbox button           8
  };

  //Set up serial
  println(Serial.list());
  port = new Serial(this, Serial.list()[5], 115200);
  
  delay(1000);
}

void draw() {
  background(0.5);
  
  // Debugging stuff
  fill(0);
  ellipse(625, 325, 2*sqrt(2)*255, 2*sqrt(2)*255);
  rectMode(CENTER);
  rect(625, 325, 2*255, 2*255);
  fill(255);
  
  // Joysticks!
  joy1y = (int) joy1.getX();
  joy1x = (int) joy1.getY();
  joy2y = (int) joy2.getX();
  joy2x = (int) joy2.getY();
  
  text("x joystick 1: " + str(joy1x), 50, 50);
  text("y joystick 1: " + str(joy1y), 50, 100);
  text("x joystick 2: " + str(joy2x), 50, 150); 
  text("y joystick 2: " + str(joy2y), 50, 200);
  
  // Filter out joystick float/noise by not sending values below 50
  if (abs(joy1y) < 50) { joy1y = 0; }
  if (abs(joy1x) < 50) { joy1x = 0; }
  if (abs(joy2y) < 50) { joy2y = 0; }
  if (abs(joy2x) < 50) { joy2x = 0; }
  
  // Triggers and Buttons!
  r_trig = (int) trigs.getX() + 128;
  l_trig = (int) trigs.getY() + 128;

  
  // Toggle PID
  if (buttons[8].pressed()) {
    xboxButtonValue = !xboxButtonValue;
    delay(DEBOUNCE_TIME);
  }
  
  text("a Toggle: " + str(buttons[0].pressed()), 50, 250);
  text("b Button: " + str(buttons[1].pressed()), 50, 300);
  text("x Button: " + str(buttons[2].pressed()), 50, 350);
  text("y Button: " + str(buttons[3].pressed()), 50, 400);
  text("Dpad Up: " + str(buttons[4].pressed()), 50, 450);
  text("Dpad Down: " + str(buttons[5].pressed()), 50, 500);
  text("PID (XBox): " + str(xboxButtonValue), 50, 550);
  
  // Deal w/ Servos (These calculations mirror those done on the craft)
  for(int i=0; i<(buttons.length-1); i++){
    if (buttons[i].pressed()){
      switch(i){
        case 0: // A
          servoValues[0] -= 5;
          servoValues[0] = constrain(servoValues[0], 65, 140);
          break;
        case 1: // B
          servoValues[0] += 5;
          servoValues[0] = constrain(servoValues[0], 65, 140);
          break;
        case 2: // X
          servoValues[1] -= 5;
          servoValues[1] = constrain(servoValues[1], 55, 110);
          break;
        case 3: // Y
          servoValues[1] += 5;
          servoValues[1] = constrain(servoValues[1], 55, 110);
          break;
        case 4: // Dpad Up
          servoValues[2] += 5;
          servoValues[2] = constrain(servoValues[2], 0, 179);
          break;
        case 5: // Dpad down
          servoValues[2] -= 5;
          servoValues[2] = constrain(servoValues[2], 0, 179);
          break;
        case 6: // Dpad left
          servoValues[3] -= 5;
          servoValues[3] = constrain(servoValues[3], 0, 179);
          break;
        case 7: // Dpad right
          servoValues[3] += 5;
          servoValues[3] = constrain(servoValues[3], 0, 179);
          break;
      }
    }
  }
  
  text("Camera Tilt  : " + str(servoValues[0]), 50, 600);
  text("Camera Roll : " + str(servoValues[1]), 50, 650);
  text("Claw Open   : " + str(servoValues[2]), 50, 700);
  text("Claw Other  : " + str(servoValues[3]), 50, 750);
  
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
  
  // Reference
  text("^", 614, 200); // Front
  line(500, 200, 750, 200); // Body
  line(750, 200, 750, 450);
  line(750, 450, 500, 450);
  line(500, 450, 500, 200);
  
  //  Draw Motor A
  line (500, 200, 500+thrustValues[0]*cos(radians(225)), 200-thrustValues[0]*sin(radians(225)));
  //  Draw Motor B
  line (750, 200, 750+thrustValues[1]*cos(radians(315)), 200-thrustValues[1]*sin(radians(315)));
  //  Draw Motor C
  line (500, 450, 500+thrustValues[2]*cos(radians(135)), 450-thrustValues[2]*sin(radians(135)));
  //  Draw Motor D
  line (750, 450, 750+thrustValues[3]*cos(radians(45)), 450-thrustValues[3]*sin(radians(45)));
  
  // Draw control vector for reference (both unscaled and scaled)
  stroke(255, 0, 0);
  line(625, 325, 625+thrustValues[5], 325-thrustValues[4]);
  stroke(0, 255, 0);
  line(625, 325, 625-joy1x, 325-joy1y);
  stroke(255);
  
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
  if (diff == 512) {
    diff -= 1;
  } else if (diff == 0) {
    diff += 1;
  }
  if (diff < 10) {
    toSend+= "00" + str(diff)  + "/";
    toSend+= "00" + str(diff)  + "/";
  }
  if (diff >= 10 && diff < 100) {
    toSend+= "0" + str(diff) + "/";
    toSend+= "0" + str(diff) + "/"; 
  }
  if (diff >= 100) {
    toSend+= str(diff) + "/";
    toSend+= str(diff) + "/";
  } 

  for (int i=0; i<servoValues.length; i++){
    if (i == servoValues.length - 1){
      toSend += make_constant_servo_value_length(servoValues[i]);
    }else {
      toSend += make_constant_servo_value_length(servoValues[i]) + "/";
    }
  }
  toSend += "$";
  
  text(toSend, 450, 750);
  
  
  port.write(toSend);
//  println(toSend);

//  //Prints arduino info to the consol
//  if (port.available() > 0) {
//    while (port.available() > 0) {
//      arduinoInfo = (char)port.read();
//      if (tempChar != arduinoInfo) {  
//        print(arduinoInfo);
//      }
//      tempChar = arduinoInfo;
//    }  
//  }
//  //Read in data from arduino
//  char val;
//  while(port.available() > 0) {
//    val = (char)port.read();
//    print(val);
//  }
  
  
  delay(250);
}

int[] getTranslation(int x, int y){
  // The code below maps the joystick input vector (constrained by a square of side length 512 centered around (0, 0))
  // . to a joystick "output vector" constrained by a circle.
  // . This ensures that we get maximum possible thrust regardless of the direction we want to move
  // Voodoo magic alert
  float s_input_mag = sqrt(x*x + y*y);
  float s_output_mag = 1.41421 * 255.0; // sqrt(2) * 255
  float dimension_scaling_factor;
  if (abs(y) > abs(x)) {
    // y-constrained regime
    dimension_scaling_factor = float(y) / 255.0;
  } else if (abs(y) < abs(x)) {
    // x-constrained regime
    dimension_scaling_factor = float(x) / 255.0;
  } else {
    // Diagonal regime
    dimension_scaling_factor = 1;
  }
  double scaling_factor = (s_output_mag / s_input_mag) * abs(dimension_scaling_factor);
  x *= scaling_factor;
  y *= scaling_factor;
  
  // The code below takes a single vector of arbitrary direction and magnitude
  // . and finds the magnitudes of four vectors of fixed direction
  // . (basically translating the joystick vector to the four motor vectors)
  int[] vals = new int[6]; // vals[0:3] contain motor values, vals[4:5] contain x and y of the rescaled control vector
  int a = (int)((x + y)*aConst);
  int b = -(int)((y - x)*bConst);
  int d = -a;
  int c = -b;
  vals[0] = a;
  vals[1] = b;
  vals[2] = c;
  vals[3] = d;
  
  vals[4] = x;
  vals[5] = y;
  return vals;
}

int[] getRotation(int x) {
  int[] vals = new int[6]; // vals[0:3] contain motor values, vals[4:5] contain x and y of the rescaled control vector
  int a = (int)(x*aConstR);
  int b = -(int)(x*bConstR);
  int d = a;
  int c = b;
  vals[0] = a;
  vals[1] = b;
  vals[2] = c;
  vals[3] = d;
  
  vals[4] = x;
  vals[5] = 0;
  return vals;
}

String make_constant_servo_value_length(int val) {
  String toRet = "";
  if (val < 10) {
    toRet += "00" + str(val);
  }
  if (val >= 10 && val < 100) {
    toRet += "0" + str(val); 
  }
  if (val >= 100) {
    toRet += str(val);
  } 
  return toRet;
}
