//author: Ethan Seide
//version: 1.0
//date: 17-10-2015

//Calculate the displacement of the MPU 6050 accelerometer
double displacement = 0;
double velocity = 0;
double acceleration = 0;

int currentTime = 0;
int prevTime = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  int dt = currentTime - prevTime;
  prevTime = currentTime;
  velocity += integral(acceleration, dt); //Velocity is the integral of acceleartion
  displacement += integral(velocity, dt); //Displacement is the integral of velocity
}

double integral(double f, double dx) {
  return f * dx;
}

