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

int joy1x = 0;
int joy1y = 0;
int joy2x = 0; 
int joy2y = 0;

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
  dpadUp = joypad.getButton(0);
  dpadDown = joypad.getButton(1);
  dpadLeft = joypad.getButton(2);
  dpadRight = joypad.getButton(3);
  startButton = joypad.getButton(4);
  selectButton = joypad.getButton(5);
  leftStickButton = joypad.getButton(6);
  rightStickButton = joypad.getButton(7);
  leftBumper = joypad.getButton(8);
  rightBumper = joypad.getButton(9);
  xboxButton = joypad.getButton(10);
  aButton = joypad.getButton(11);
  bButton = joypad.getButton(12);
  xButton = joypad.getButton(13);
  yButton = joypad.getButton(14);
}

void draw() {
  //Clear Previous text
  background(0.5);
  
  //Write ALL the values!
  text("joystick 1 x: " + str((int) joy1.getX()), 50, 50);
  text("joystick 1 y: " + str((int) joy1.getY()), 50, 100);
  text("joystick 2 x: " + str((int) joy2.getX()), 50, 150); 
  text("joystick 2 y: " + str((int) joy2.getY()), 50, 200);
  text("Button 0: " + str(dpadUp.pressed()), 50, 225);
  text("Button 1: " + str(dpadDown.pressed()), 50, 250);
  text("Button 2: " + str(dpadLeft.pressed()), 50, 275);
  text("Button 3: " + str(dpadRight.pressed()), 50, 300);
  text("Button 4: " + str(startButton.pressed()), 50, 325);
  text("Button 5: " + str(selectButton.pressed()), 50, 350);
  text("Button 6: " + str(leftStickButton.pressed()), 50, 375);
  text("Button 7: " + str(rightStickButton.pressed()), 50, 400);
  text("Button 8: " + str(leftBumper.pressed()), 50, 425);
  text("Button 9: " + str(rightBumper.pressed()), 50, 450);
  text("Button 10: " + str(xboxButton.pressed()), 50, 475);
  text("Button 11: " + str(aButton.pressed()), 50, 500);
  text("Button 12: " + str(bButton.pressed()), 50, 525);
  text("Button 13: " + str(xButton.pressed()), 50, 550);
  
  delay(10);
}
