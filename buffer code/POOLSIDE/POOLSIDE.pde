import procontroll.*;
import net.java.games.input.*;
import processing.serial.*;

// BUFFER CODE!!!!
// Notes on the controller:
//   - Axis are inverted, so we used negative multipliers
//   - The "x" axis is the y-axis irl, but the controller's wierd like that

Serial port;
Console scr;

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

//values for xbox toggle
boolean xboxButtonValue = false;
boolean xboxButtonPressed = false;

final int JOYSTICK_NOISE = 50; //wiggle room on joysticks. min ~= 40 with current controller

final int NUM_SERVOS = 3; //Number of servo motors
final int PAN = 0; //Servo index for camera pan
final int TILT = 1; //Servo index for camera tilt
final int CLAW = 2; //Servo index for claw
final int WRIST = 3; //Servo index for wrist

//Constraints and center for pan
final int PAN_MIN = 65;
final int PAN_CENTER = 90;
final int PAN_MAX = 115;

//Constraints and center for tilt
final int TILT_MIN = 90;
final int TILT_CENTER = 118;
final int TILT_ED = 145;

//Constraints for claw
final int CLAW_CLOSED = 70;
final int CLAW_OPEN = 120;

//Constraints for wrist
final int WRIST_CENTER = 90;
final int WRIST_LEFT = 0;
final int WRIST_RIGHT = 180;

int[] servoValues = new int[NUM_SERVOS];



final boolean serialOn = true;

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

    scr = new Console(10, 1100, 300);
    scr.addLine("Console output");
    
    //Set up serial
    String[] serialList = Serial.list();
    println(serialList);
    for(int i = 0; i < serialList.length; i++){
        scr.addLine(serialList[i]);
    }
        
    if(serialOn){
        port = new Serial(this, Serial.list()[Serial.list().length - 1], 38400);
    }

    //servo start values
    servoValues[PAN] = PAN_CENTER; //Pan starts center
    servoValues[TILT] = TILT_CENTER; //Tilt starts center
    servoValues[CLAW] = CLAW_OPEN; //Claw starts open
    servoValues[WRIST] = WRIST_CENTER; //Wrist starts center

    
    delay(1000);
}

void draw() {
    background(0);

    textSize(12);
    scr.printOutput();
//    scr.println(str(millis())); //uncomment this to test console


    // Debugging stuff
    noFill();
    textAlign(LEFT,BOTTOM);
    
    //header (for space filler -- it looked empty)
    textSize(48);
    text("Placeholder Text", 50, 75);
    textSize(32);

    stroke(255, 0, 0); //red circle for thrust bound
    ellipse(625, 425, 2*sqrt(2)*255, 2*sqrt(2)*255);

    stroke(0, 255, 0); //green circle for controller bound
    ellipse(625, 425, 2*255, 2*255);

    rectMode(CENTER); //green square for controller bound
    rect(625, 425, 2*255, 2*255);

    stroke(255); //other lines are white

    //box around console output
    rectMode(CORNERS);
    rect(1100, 300, 1440, 900);
        
    // Joysticks!
    joy1Vec[1] = (int) joy1.getX(); 
    joy1Vec[0] = (int) joy1.getY();
    joy2Vec[1] = (int) joy2.getX();
    joy2Vec[0] = (int) joy2.getY();
    
    text("x joystick 1: " + str(joy1Vec[0]), 10, 150);
    text("y joystick 1: " + str(joy1Vec[1]), 10, 200);
    text("x joystick 2: " + str(joy2Vec[0]), 10, 250); 
    text("y joystick 2: " + str(joy2Vec[0]), 10, 300);

  
    // Filter out joystick float/noise by not sending values below JOYSTICK_NOISE
    for (int i = 0; i < joy1Vec.length; i++) {
        if (abs(joy1Vec[i]) < JOYSTICK_NOISE) { joy1Vec[i] = 0; }
    }
    for (int i = 0; i < joy2Vec.length; i++) {
        if (abs(joy2Vec[i]) < JOYSTICK_NOISE) { joy2Vec[i] = 0; }
    }
  
    // Triggers and Buttons!
    rTrig = (int) trigs.getX() + 128;
    lTrig = (int) trigs.getY() + 128;
  
    debounce();

    fill(0,255,0); //got bored lol
    text("a Button: " + str(buttons[0].pressed()), 10, 350);
    fill(255,0,0);
    text("b Button: " + str(buttons[1].pressed()), 10, 400);
    fill(0,0,255);
    text("x Button: " + str(buttons[2].pressed()), 10, 450);
    fill(255,255,0);
    text("y Button: " + str(buttons[3].pressed()), 10, 500);
    fill(255);
    noFill();
    text("Dpad Up: " + str(buttons[4].pressed()), 10, 550);
    text("Dpad Down: " + str(buttons[5].pressed()), 10, 600);
    text("PID (XBox): " + str(xboxButtonValue), 10, 650);
  
    // Deal w/ Servos (These calculations mirror those done on the craft) ?? not sure about this
    for(int i=0; i<(buttons.length-1); i++){
        if (buttons[i].pressed()){
            switch(i){

            case 6: // Dpad Left
                servoValues[PAN] -= 1;
                servoValues[PAN] = constrain(servoValues[PAN], 65, 115);
                break;
            case 7: // Dpad Right
                servoValues[PAN] += 1;
                servoValues[PAN] = constrain(servoValues[PAN], 65, 115);

                break;
            case 4: // Dpad Up
                servoValues[TILT] += 1;
                servoValues[TILT] = constrain(servoValues[TILT], TILT_MIN, TILT_ED);
                break;
            case 5: // Dpad down
                servoValues[TILT] -= 1;
                servoValues[TILT] = constrain(servoValues[TILT], TILT_MIN, TILT_ED);
                break;
                
            case 2: // X
                servoValues[WRIST] -= 1;
                servoValues[WRIST] = constrain(servoValues[WRIST], WRIST_LEFT, WRIST_RIGHT);
                break; 
            case 3: // Y
                servoValues[CLAW] += 1;
                servoValues[CLAW] = constrain(servoValues[CLAW], CLAW_CLOSED, CLAW_OPEN);
                break;
            case 0: // A
                servoValues[CLAW] -= 1;
                servoValues[CLAW] = constrain(servoValues[CLAW], CLAW_CLOSED, CLAW_OPEN);
                break;
            case 1: // B
                servoValues[WRIST] += 1;
                servoValues[WRIST] = constrain(servoValues[WRIST], WRIST_LEFT, WRIST_RIGHT);
                break;
            }
        }
    }
  
    text("Camera Pan: " + str(servoValues[PAN]), 10, 700);
    text("Camera Tilt: " + str(servoValues[TILT]), 10, 750);
    text("Claw Open (X/Y): " + str(servoValues[CLAW]), 10, 800);
  
    //Calculate thrust vectors
    if (joy1Vec[0] != 0 || joy1Vec[1] != 0) {
        thrustValues = getTranslationCircle(joy1Vec[1], -joy1Vec[0]);
        // Map thrust vectors to between -256 and +256
        for (int i = 0; i < 4; i++) {
            if (thrustValues[i] > 512 || thrustValues[i] < -512) {
                scr.println("WHAT THE FRACK ARE YOU DOING? Motor #" + str(i) + " is " + str(thrustValues[i]));
            }
            thrustValues[i] = (int) map(thrustValues[i], -512, 512, -256, 256);
        }
    }
    else {
        thrustValues = getRotation(-joy2Vec[0]);
    }
  
  
    text("Right Trigger: " + str(rTrig), 900, 100);
    text("Left Trigger:  " + str(lTrig), 900, 150);

  
    // Reference
    text("^", 614, 300); // Front
    rectMode(CORNERS);
    rect(500,300,750,550); //Body
  
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
    line(625, 425, 625-joy1Vec[0], 425-joy1Vec[1]);


    stroke(255);
  
    // Draw Depth control lines
    int diff = lTrig - rTrig;
    text("Diff: " + str(diff), 950, 200); //write diff
    //line (1050, 425, 1050, 425 - (diff));
    rectMode(CORNERS);
    fill(255);
    rect(1050, 425, 1060, 425 - (diff));
    noFill();
    
    //String toSend = str((char)1); //1=start value
    port.write(1);

    // send the values
    for (int counter = 0; counter < 4; counter++) {
        int sendVal = thrustValues[counter]/2+128;
        
        if(sendVal >= 255) sendVal = 254; //1 and 255 are reserved
        else if(sendVal <= 1) sendVal = 2;
        
        //toSend += (char)sendVal;
        port.write(sendVal);

        scr.println(counter + " " + sendVal);
    }
    diff = diff/2 + 128;
    if(diff >= 255) diff = 254;
    else if(diff <= 1) diff = 2;
    //toSend += (char)diff;
    port.write(diff);

    
    for (int i = 0; i < NUM_SERVOS; i++){
        //toSend += (char)(servoValues[i] + 1);
    }
    //toSend += (char)255;
    port.write(255);

    
    
    //text(toSend, 300, 850);
    
    //write to console as well
    //scr.println("toSend = " + toSend);
  
    if(serialOn){
        //        port.write(toSend);
    }

    //read serial
    String read = port.readString();
    if(read != null){
        scr.println(read);
    }
    
    delay(100);
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


//this version of get translation uses the first (circle) method from this page on the wiki
//https://github.com/MaretEngineering/MROV/wiki/Joystick-Problem
    int[] getTranslationCircle(int x, int y){ 
        //1. cut off values outside radius of 255
        //2. math
    
        float sInputMag = sqrt(x*x + y*y);
        if(sInputMag > 255){ //
            x *= (255 / sInputMag);
            y *= (255 / sInputMag);
        }

        float sOutputMag = 1.41421 * 255.0; //sqrt(2)*255

        //scale joystick vector to thrust vector
        x *= 1.41421;
        y *= 1.41421;    
  
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

    String formatInt(int x){
        String str = "";
        String num = str(x);
    
        if(x < 10){str += "00" + num;}
        else if(x < 100){str += "0" + num;}
        else{str += num;}
    
        return str;
    }

    void debounce() {
        if (!buttons[8].pressed()) {
            xboxButtonPressed = false;
        }
        else if ((buttons[8].pressed())&&(!xboxButtonPressed)) {
            xboxButtonValue = !xboxButtonValue;
            xboxButtonPressed = true;
//      delay(5); //seems to work fine without a delay
        }
        else {
            xboxButtonPressed = true;
        }
    }
