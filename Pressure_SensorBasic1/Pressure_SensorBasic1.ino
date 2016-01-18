int sensorPort = A0;
double sensorVal = 0;
double kPa = 0;
double height = 0;

/*
 * Pa = (rho*g*h) + P0
 * Pa = (N/m^2) = (kg/m^3)(m/s^2)(m)
 * N = (kg)(m)/(s^2)
 */


void setup() {
  Serial.begin(115200); //Open serial port
  Serial.println("Initializing sensor...");
  delay(2000);
  Serial.println("Ready");
}

void loop() {
  sensorVal = analogRead(sensorPort);
  kPa = (sensorVal*(0.00488)/(0.022) + 20);
  height = 1000*((kPa-101.85)/(9800));
  //Serial.print("Raw/kPa/height \t");
  //Serial.println(sensorVal);
  //Serial.println(kPa);
  Serial.println((int)(height*100));
  delay(300);
}
