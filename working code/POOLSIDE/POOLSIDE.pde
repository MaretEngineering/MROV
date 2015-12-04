import procontroll.*;
import net.java.games.input.*;
import processing.serial.*;

// Notes on the controller:
//   - Axis are inverted, so we used negative multipliers
//   - The "x" axis is the y-axis irl, but the controller's wierd like that

final boolean SERIAL = false;

Serial port;

ControllIO controllIO;
ControllDevice joypad;
ControllStick joy1;
ControllStick joy2;
ControllStick trigs;


ControllButton buttons[];

int[] joy1Vec = {0,0};
int[] joy2Vec = {0,0};

int rTrig = 0;
int lTrig = 0;

//Thrust Values
int[] thrustValues;

boolean xboxButtonValue = false;
final int DEBOUNCE_TIME = 175; //for xbox button toggle
final int JOYSTICK_NOISE = 50; //wiggle room on joysticks. min = 40 with current controller
final int NUM_SERVOS = 6; //Number of servo motors

int[] servoValues = new int[NUM_SERVOS];

void setup() {
    size(1440, 900);
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

    if(SERIAL){
        //Set up serial
        println(Serial.list());
        port = new Serial(this, Serial.list()[Serial.list().length - 1], 9600);
    }
        
    delay(1000);
}

void draw() {
    background(0.5);
    
    //header (for space filler -- it looked empty)
    textSize(48);
    text("Placeholder Text", 50, 75);
    textSize(32);
    
    // Debugging stuff
    noFill();

    stroke(255, 0, 0); //red circle for thrust bound
    ellipse(625, 425, 2*sqrt(2)*255, 2*sqrt(2)*255);

    stroke(0, 255, 0); //green circle for controller bound
    ellipse(625, 425, 2*255, 2*255);

    rectMode(CENTER); //green square for controller bound
    rect(625, 425, 2*255, 2*255);

    stroke(255); //other lines are white
        
    // Joysticks!
    joy1Vec[1] = (int) joy1.getX(); //Why are these inverted x and y??? -Ethan
    joy1Vec[0] = (int) joy1.getY();
    joy2Vec[1] = (int) joy2.getX();
    joy2Vec[0] = (int) joy2.getY();
    
<<<<<<< HEAD
    text("x joystick 1: " + str(joy1Vec[0]), 10, 50);
    text("y joystick 1: " + str(joy1Vec[1]), 10, 100);
    text("x joystick 2: " + str(joy2Vec[0]), 10, 150); 
    text("y joystick 2: " + str(joy2Vec[1]), 10, 200);
=======
    text("x joystick 1: " + str(joy1x), 10, 150);
    text("y joystick 1: " + str(joy1y), 10, 200);
    text("x joystick 2: " + str(joy2x), 10, 250); 
    text("y joystick 2: " + str(joy2y), 10, 300);
>>>>>>> origin/master
  
    // Filter out joystick float/noise by not sending values below JOYSTICK_NOISE
    for (int i = 0; i < joy1Vec; i++) {
        if (abs(joy1Vec[i]) < JOYSTICK_NOISE) { joy1Vec[i] = 0; }
    }
    for (int i = 0; i < joy2Vec; i++) {
        if (abs(joy2Vec[i]) < JOYSTICK_NOISE) { joy2Vec[i] = 0; }
    }
  
    // Triggers and Buttons!
    rTrig = (int) trigs.getX() + 128;
    lTrig = (int) trigs.getY() + 128;
  
    // Toggle PID
    if (buttons[8].pressed()) {
        xboxButtonValue = !xboxButtonValue;
        delay(DEBOUNCE_TIME);
    }
  
    text("a Button: " + str(buttons[0].pressed()), 10, 350);
    text("b Button: " + str(buttons[1].pressed()), 10, 400);
    text("x Button: " + str(buttons[2].pressed()), 10, 450);
    text("y Button: " + str(buttons[3].pressed()), 10, 500);
    text("Dpad Up: " + str(buttons[4].pressed()), 10, 550);
    text("Dpad Down: " + str(buttons[5].pressed()), 10, 600);
    text("PID (XBox): " + str(xboxButtonValue), 10, 650);
  
    // Deal w/ Servos (These calculations mirror those done on the craft) ?? not sure about this
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
  
    text("Camera Pan: " + str(servoValues[0]), 10, 700); //CAREFUL
    text("Camera Tilt: " + str(servoValues[2]), 10, 750); //displayed in a different order than array 
    text("Claw Open (X/Y): " + str(servoValues[1]), 10, 800); 
  
    //Calculate thrust vectors
    if (joy1Vec[0] != 0 || joy1Vec[1] != 0) {
        thrustValues = getTranslation(joy1Vec[1], -joy1Vec[0]);
        // Map thrust vectors to between -256 and +256
        for (int i = 0; i < 4; i++) {
            if (thrustValues[i] > 512 || thrustValues[i] < -512) {
                println("WHAT THE FRACK ARE YOU DOING? Motor #" + str(i) + " is " + str(thrustValues[i]));
            }
            thrustValues[i] = (int) map(thrustValues[i], -512, 512, -256, 256);
        }
<<<<<<< HEAD
    } else {
        thrustValues = getRotation(-joy2Vec[0]);
=======
    }
    else {
        thrustValues = getRotation(-joy2x);
>>>>>>> origin/master
    }
  
  
    text("Right Trigger: " + str(rTrig), 900, 100);
    text("Left Trigger:  " + str(lTrig), 900, 150);
  
    // Reference
    text("^", 614, 300); // Front
    rect(500,300,250,250); //Body
  
    //  Draw Motor A
    line (500, 300, 500+thrustValues[0]*cos(radians(45)), 300-thrustValues[0]*sin(radians(45)));
    //  Draw Motor B
    line (750, 300, 750+thrustValues[1]*cos(radians(135)), 300-thrustValues[1]*sin(radians(135)));
    //  Draw Motor D
    line (500, 550, 500+thrustValues[3]*cos(radians(315)), 550-thrustValues[3]*sin(radians(315)));
    //  Draw Motor C
    line (750, 550, 750+thrustValues[2]*cos(radians(225)), 550-thrustValues[2]*sin(radians(225)));
  
    // Draw control vector for reference (both unscaled and scaled)
    stroke(255, 0, 0); //red line
    line(625, 425, 625+thrustValues[5], 425-thrustValues[4]);

    stroke(0, 255, 0); //green line
<<<<<<< HEAD
    line(625, 325, 625-joy1Vec[0], 325-joy1Vec[1]);
=======
    line(625, 425, 625-joy1x, 425-joy1y);
>>>>>>> origin/master

    stroke(255);
  
    // Draw Depth control lines
    int diff = lTrig - rTrig;
    //line (1050, 425, 1050, 425 - (diff));
    rectMode(CORNERS);
    fill(255);
    rect(1050, 425, 1060, 425 - (diff));
    noFill();
    
    String toSend = "!";

    // makes all thrust value strings going out 3 characters long + the "/"
    for (int counter = 0; counter < 4; counter++) {
        thrustValues[counter] += 256;
        if (thrustValues[counter] >= 100) {
            toSend += str(thrustValues[counter]) + "/";
        }
        else if (thrustValues[counter] >= 10) {
            toSend += "0" + str(thrustValues[counter]) + "/"; 
        }
        else {
            toSend += "00" + str(thrustValues[counter]) + "/";
        }
    }
    
    //makes the diff value 3 characters long + "/"
    diff += 256;
    if (diff == 512) {
        diff -= 1;
    } else if (diff == 0) {
        diff += 1;
    }
    if (diff >= 100) {
        toSend += str(diff) + "/";
        toSend += str(diff) + "/";
    } 
    else if (diff >= 10) {
        toSend += "0" + str(diff) + "/";
        toSend += "0" + str(diff) + "/"; 
    }
    else {
        toSend+= "00" + str(diff)  + "/";
        toSend+= "00" + str(diff)  + "/";
    }

    for (int i=0; i < NUM_SERVOS; i++){
        if (i == NUM_SERVOS - 1){
            toSend += constantLength(servoValues[i]);
        }else {
            toSend += constantLength(servoValues[i]) + "/";
        }
    }
    toSend += "$";
  
    text(toSend, 300, 850);
  
    if(SERIAL){
        port.write(toSend);
    }
        
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

int[] getTranslationCircle(int x, int y){
    //1. cut off values outside radius of 255
    //2. math
    
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
    if (val >= 100) {
        toRet += str(val);
    }
    else if (val >= 10) {
        toRet += "0" + str(val); 
    } 
    else {
        toRet += "00" + str(val);
    }
    return toRet;
}
