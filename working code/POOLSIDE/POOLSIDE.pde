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

int rTrig = 0;
int lTrig = 0;

//Thrust Values
int[] thrustValues;

boolean xboxButtonValue = false;
const int DEBOUNCE_TIME = 175; //for xbox button toggle

int[] servoValues = {
    90, // Cam 1   0
    90, // Cam 2   1
    90 // Claw    2
};

void setup(){
    size(1500, 900);
    stroke(150);
    textSize(32);
  
    //Set up controller
    controllIO = ControllIO.getInstance(this);
    joypad = controllIO.getDevice("Wireless 360 Controller");
 
    //Set up joystick w/ proper multiplier for x and y values (negative because otehrwise it's inverted)
    joy1 = joypad.getStick(0);
    joy1.setMultiplier(-255);
    //Set Up 2nd joystick w/ proper multiplier for x and y values (see above for why)
    joy2 = joypad.getStick(1); 
    joy2.setMultiplier(255); 
  
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
    port = new Serial(this, Serial.list()[Serial.list().length - 1], 9600);
  
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
    rTrig = (int) trigs.getX() + 128;
    lTrig = (int) trigs.getY() + 128;

  
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
            case 6: // Dpad Down
                servoValues[0] -= 1;
                servoValues[0] = constrain(servoValues[0], 65, 115);
                break;
            case 7: // Dpad Up
                servoValues[0] += 1;
                servoValues[0] = constrain(servoValues[0], 65, 115);
                break;
            case 2: // X
                servoValues[1] -= 1;
                servoValues[1] = constrain(servoValues[1], 70, 120);
                break;
            case 3: // Y
                servoValues[1] += 1;
                servoValues[1] = constrain(servoValues[1], 70, 120);
                break;
            case 4: // Dpad Up
                servoValues[2] += 1;
                servoValues[2] = constrain(servoValues[2], 90, 145);
                break;
            case 5: // Dpad down
                servoValues[2] -= 1;
                servoValues[2] = constrain(servoValues[2], 90, 145);
                break;
            }
        }
    }
  
    text("Camera Pan  : " + str(servoValues[0]), 50, 600);
    text("Claw Open : " + str(servoValues[1]), 50, 650);
    text("Camera Tilt   : " + str(servoValues[2]), 50, 700);
  
    //Calculate thrust vectors
    if (joy1y != 0 || joy1x != 0) {
        thrustValues = getTranslation(joy1y, -joy1x);
        // Map thrust vectors to between -256 and +256
        for (int i = 0; i < 4; i++) {
            if (thrustValues[i] > 512 || thrustValues[i] < -512) {
                println("WHAT THE FRACK ARE YOU DOING? Motor #" + str(i) + " is " + str(thrustValues[i]));
            }
            thrustValues[i] = (int) map(thrustValues[i], -512, 512, -256, 256);
        }
    } else {
        thrustValues = getRotation(-joy2x);
    }
  
  
    text("Right Trigger: " + str(rTrig), 900, 50);
    text("Left Trigger:  " + str(lTrig), 900, 100);
  
    // Reference
    text("^", 614, 200); // Front
    line(500, 200, 750, 200); // Body
    line(750, 200, 750, 450);
    line(750, 450, 500, 450);
    line(500, 450, 500, 200);
  
    //  Draw Motor A
    line (500, 200, 500+thrustValues[0]*cos(radians(45)), 200-thrustValues[0]*sin(radians(45)));
    //  Draw Motor B
    line (750, 200, 750+thrustValues[1]*cos(radians(135)), 200-thrustValues[1]*sin(radians(135)));
    //  Draw Motor D
    line (500, 450, 500+thrustValues[3]*cos(radians(315)), 450-thrustValues[3]*sin(radians(315)));
    //  Draw Motor C
    line (750, 450, 750+thrustValues[2]*cos(radians(225)), 450-thrustValues[2]*sin(radians(225)));
  
    // Draw control vector for reference (both unscaled and scaled)
    stroke(255, 0, 0);
    line(625, 325, 625+thrustValues[5], 325-thrustValues[4]);
    stroke(0, 255, 0);
    line(625, 325, 625-joy1x, 325-joy1y);
    stroke(255);
  
    // Draw Depth control lines
    int diff = lTrig - rTrig;
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
            toSend += constantLength(servoValues[i]);
        }else {
            toSend += constantLength(servoValues[i]) + "/";
        }
    }
    toSend += "$";
  
    text(toSend, 450, 750);
  
  
    port.write(toSend);
  
    delay(30);
}

int[] getTranslation(int x, int y){
    // The code below maps the joystick input vector (constrained by a square of side lengths 512 centered around (0, 0))
    // . to a joystick "output vector" constrained by a circle.
    // . This ensures that we get maximum possible thrust regardless of the direction we want to move
    // Voodoo magic alert
    float sInputMag = sqrt(x*x + y*y);
    float sOutputMag = 1.41421 * 255.0; // sqrt(2) * 255
    float dimensionScalingFactor;
    if (abs(y) > abs(x)) {
        // y-constrained regime
        dimensionScalingFactor = float(y) / 255.0;
    } else if (abs(y) < abs(x)) {
        // x-constrained regime
        dimensionScalingFactor = float(x) / 255.0;
    } else {
        // Diagonal regime
        dimensionScalingFactor = 1;
    }
    double scalingFactor = (sOutputMag / sInputMag) * abs(dimensionScalingFactor);
    x *= scalingFactor;
    y *= scalingFactor;
  
    // The code below takes a single vector of arbitrary direction and magnitude
    // . and finds the magnitudes of four vectors of fixed direction
    // . (basically translating the joystick vector to the four motor vectors)
    int[] vals = new int[6]; // vals[0:3] contain motor values, vals[4:5] contain x and y of the rescaled control vector
    int a = (int)(x + y);
    int b = -(int)(y - x);
    int c = -a;
    int d = -b;
    vals[0] = -a;
    vals[1] = -b;
    vals[2] = -c;
    vals[3] = -d;
  
    vals[4] = x;
    vals[5] = y;
    return vals;
}

int[] getRotation(int x) {
    int[] vals = new int[6]; // vals[0:3] contain motor values, vals[4:5] contain x and y of the rescaled control vector
    int a = (int)(x);
    int b = -(int)(x);
    int c = a;
    int d = b;
    vals[0] = a;
    vals[1] = b;
    vals[2] = c;
    vals[3] = d;
  
    vals[4] = 0;
    vals[5] = x;
    return vals;
}

String constantLength(int val) {
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
