import procontroll.*;
import net.java.games.input.*;

// Notes on the controller:
//   - Axis are inverted, so we used negative multipliers
//   - The "x" axis is the y-axis irl, but the controller's wierd like that


ControllIO controllIO;
ControllDevice joypad;

ControllStick joy1; // Rotation Control
ControllStick joy2; // Translation Control

ControllButton dpadUp; //up D
ControllButton dpadDown; //down D
ControllButton dpadLeft; //left D
ControllButton dpadRight; //right D
ControllButton startButton; //start
ControllButton selectButton; //select
ControllButton leftStickButton; //left stick button
ControllButton rightStickButton; //right stick button
ControllButton leftBumper; //left bumper
ControllButton rightBumper; //right bumper
ControllButton xboxButton; //xbox button
ControllButton aButton; //a
ControllButton bButton; //b
ControllButton xButton; //x
ControllButton yButton; //y

//Button Values
boolean aButtonValue = false;
boolean bButtonValue = false;
boolean xButtonValue = false;
boolean yButtonValue = false;

int joy1x = 0;
int joy1y = 0;
int joy2x = 0; 
int joy2y = 0;

int debounceTime = 175;

void setup(){
  size(600, 600);
  noStroke();
  textSize(16);
  
  
  //Set up controller
  controllIO = ControllIO.getInstance(this);
  joypad = controllIO.getDevice("Afterglow Gamepad for Xbox 360");
 
  //Set up joysticks w/ proper multiplier for x and y values (negative because otehrwise it's inverted)
  joy1 = joypad.getStick(0);
  joy1.setMultiplier(-255);
  joy2 = joypad.getStick(1); 
  joy2.setMultiplier(-255);
  
  //Get ALL the buttons
  aButton = joypad.getButton(11);
  bButton = joypad.getButton(12);
  xButton = joypad.getButton(13);
  yButton = joypad.getButton(14);
}

void draw() {
  //Clear Previous text
  background(0.5);
  
  //Get button values
  if (aButton.pressed()) { aButtonValue = ! aButtonValue; delay(debounceTime); }
  if (bButton.pressed()) { bButtonValue = ! bButtonValue; delay(debounceTime); }
  if (xButton.pressed()) { xButtonValue = ! xButtonValue; delay(debounceTime); }
  if (yButton.pressed()) { yButtonValue = ! yButtonValue; delay(debounceTime); }
  
  //Write ALL the values!
  text("joystick 1 x: " + str((int) joy1.getY()), 50, 50);
  text("joystick 1 y: " + str((int) joy1.getX()), 50, 75);
  text("joystick 2 x: " + str((int) joy2.getY()), 50, 100); 
  text("joystick 2 y: " + str((int) joy2.getX()), 50, 125);
  text("A Button: " + str(aButtonValue), 50, 225);
  text("B Button: " + str(bButtonValue), 50, 250);
  text("X Button: " + str(xButtonValue), 50, 275);
  text("Y Button: " + str(yButtonValue), 50, 300);
  
  delay(10);
}
