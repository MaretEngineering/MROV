
int WIDTH;
int HEIGHT;

final int ROV_WIDTH = 100;
final int ROV_HEIGHT = 100;

ROV rov;
//Controller contr; // Uncomment once XBox control is implemented

void setup() {
  // Boilerplate
  WIDTH = displayWidth;
  HEIGHT = displayHeight - 120;
  size(displayWidth, displayHeight - 120);
  
  // Set up controller
//  contr = new Controller("Afterglow Gamepad for Xbox 360");
  
  // Set up ROV
  rov = new ROV(WIDTH/2, HEIGHT/2, ROV_WIDTH, ROV_HEIGHT, 10, 0.25);
}

void draw() {
  background(100);
  
  // Feed controller input to ROV
//  rov.take_input(contr.get_intput()); // Uncomment once XBox control is implemented
  rov.take_input(get_emulated_stick_values());
  
  // Update ROV
  rov.move();
  rov.display();
}

// Treats the mouse's displacement from the center of the window as a joystick value, emulating a single joystick
float[] get_emulated_stick_values() {
  float dx = WIDTH/2 - mouseX; // Finds x displacement of mouse
  float dy = HEIGHT/2 - mouseY; // Same for y
  
  dx = map(dx, -WIDTH/2, WIDTH/2, -255, 255);
  dy = map(dy, -HEIGHT/2, HEIGHT/2, -255, 255);
  
  float emulated_stick[] = {0, 0, 0};
  if (mousePressed) {
    emulated_stick[0] = dx;
    emulated_stick[1] = dy;
    emulated_stick[2] = 0;
  }
  return emulated_stick;
}
