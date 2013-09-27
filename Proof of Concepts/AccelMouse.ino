/**
*
* Maret Engineering Team
* CC BY-SA
*
*/

#include <Wire.h> // Used for I2C
#include <math.h>

// The SparkFun breakout board defaults to 1, set to 0 if SA0 jumper on the bottom of the board is set
#define MMA8452_ADDRESS 0x1D  // 0x1D if SA0 is high, 0x1C if low

//Define a few of the registers that we will be accessing on the MMA8452
#define OUT_X_MSB 0x01
#define XYZ_DATA_CFG  0x0E
#define WHO_AM_I   0x0D
#define CTRL_REG1  0x2A

#define GSCALE 2 // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.

#define timeInterval 10

//Variables
int acceleration[] = {0, 0, 0};
float velocity[] = {0, 0, 0};
float velocityOld[] = {0, 0, 0};

void setup()
{
  
  Serial.begin(9600);
  Wire.begin();
  Mouse.begin();
  initMMA8452();
  pinMode(9, INPUT);
  
  
}

void loop()
{  
  if(digitalRead(9) == HIGH){ Serial.println("Switch off"); return; }
  
  readAccelData(acceleration);  // Read the x/y/z adc values

  // Now we'll calculate the accleration value into actual g's
  double accelActual[3];  // Stores the real accel value in g's
  for (int i = 0 ; i < 3 ; i++)
  {
    accelActual[i] = (double) acceleration[i] / ((1<<12)/(2*GSCALE)) * 9.8;  // get actual m/s^2 value, this depends on scale being set
  }
  
  accelActual[2] -= 9.8;
  
  Serial.print("X Acc: "); Serial.println(accelActual[0]);
  Serial.print("Z Acc: "); Serial.println(accelActual[2]);
  
  
  for(int i = 0; i < 3; i++) {
    velocityOld[i] = velocity[i];
    velocity[i] =+ ((float)accelActual[i]*timeInterval*0.001)*100;
  }
  
  Serial.print("Vx: "); Serial.println(velocity[0]);
  Serial.print("Vy: "); Serial.println(-velocity[2]);
  
  float deltaX = 100*(velocityOld[0]*(timeInterval*0.001)+0.5*accelActual[0]*pow((timeInterval*0.001), 2));
  float deltaY = 100*(-velocityOld[2]*(timeInterval*0.001) - 0.5*accelActual[2]*pow((timeInterval*0.001), 2));
  
  Serial.print("DeltaX: "); Serial.println(deltaX);
  Serial.print("DeltaY: "); Serial.println(deltaY);
  
  Mouse.move(deltaX, deltaY, 0);
  
  delay(timeInterval);
}





















//------------------------------EVERYTHING BELOW THIS POINT ARE FUNCTIONS TO DEAL WIRTH ACCELEROMETER-----------

void readAccelData(int *destination)
{
  byte rawData[6];  // x/y/z accel register data stored here

  readRegisters(OUT_X_MSB, 6, rawData);  // Read the six raw data registers into data array

  // Loop to calculate 12-bit ADC and g value for each axis
  for(int i = 0; i < 3 ; i++)
  {
    int gCount = (rawData[i*2] << 8) | rawData[(i*2)+1];  //Combine the two 8 bit registers into one 12-bit number
    gCount >>= 4; //The registers are left align, here we right align the 12-bit integer

    // If the number is negative, we have to make it so manually (no 12-bit data type)
    if (rawData[i*2] > 0x7F)
    {  
      gCount = ~gCount + 1;
      gCount *= -1;  // Transform into negative 2's complement #
    }

    destination[i] = gCount; //Record this gCount into the 3 int array
  }
}

// Initialize the MMA8452 registers 
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8452Q
void initMMA8452()
{
  byte c = readRegister(WHO_AM_I);  // Read WHO_AM_I register
  if (c == 0x2A) // WHO_AM_I should always be 0x2A
  {  
    Serial.println("MMA8452Q is online...");
  }
  else
  {
    Serial.print("Could not connect to MMA8452Q: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
  }

  MMA8452Standby();  // Must be in standby to change registers

  // Set up the full scale range to 2, 4, or 8g.
  byte fsr = GSCALE;
  if(fsr > 8) fsr = 8; //Easy error check
  fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
  writeRegister(XYZ_DATA_CFG, fsr);

  //The default data rate is 800Hz and we don't modify it in this example code

  MMA8452Active();  // Set to active to start reading
}

// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby()
{
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active()
{
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c | 0x01); //Set the active bit to begin detection
}

// Read bytesToRead sequentially, starting at addressToRead into the dest byte array
void readRegisters(byte addressToRead, int bytesToRead, byte * dest)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active

  Wire.requestFrom(MMA8452_ADDRESS, bytesToRead); //Ask for bytes, once done, bus is released by default

  while(Wire.available() < bytesToRead); //Hang out until we get the # of bytes we expect

  for(int x = 0 ; x < bytesToRead ; x++)
    dest[x] = Wire.read();    
}

// Read a single byte from addressToRead and return it as a byte
byte readRegister(byte addressToRead)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active

  Wire.requestFrom(MMA8452_ADDRESS, 1); //Ask for 1 byte, once done, bus is released by default

  while(!Wire.available()) ; //Wait for the data to come back
  return Wire.read(); //Return this one byte
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister(byte addressToWrite, byte dataToWrite)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToWrite);
  Wire.write(dataToWrite);
  Wire.endTransmission(); //Stop transmitting
}
