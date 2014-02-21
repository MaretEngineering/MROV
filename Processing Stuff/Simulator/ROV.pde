class ROV {
  float w, h; // Dimensions
  
  float thrust[] = {0, 0, 0, 0}; // Thrust for each of the 4 horizontal motors
                                 //    Arranged like so (dots indicate props):
                                 //                        ./.    .\.
                                 //                        . .     . .
                                 //                         \      /

  // Translational stuff
  float pos[] = {0, 0}; // Position in x and y
  float speed[] = {0, 0}; // Speed in x and y directions
  float acceleration[] = {0, 0}; // Acceleration in x and y directions
  float force[] = {0, 0};
  
  // Rotational stuff
  float theta = 0; // Rotation
  float omega = 0; // Angular velocity
  float alpha = 0; // Angular acceleration
  float torque = 0;
  
  float mass;
  float radius;
  
  // Tuning constants to convert between input and thrust values
  final float aConst = 0.005;  // For translation
  final float bConst = 0.005;  //    same
  final float aConstR = 1; // For rotation
  final float bConstR = 1; //    same
  
  ROV(float x, float y, float new_width, float new_height, float weight, float new_radius) {
    pos[0] = x;
    pos[1] = y;
    
    w = new_width;
    h = new_height;
    
    mass = weight; // Not technically accurate, I know, but you know what I mean
    radius = new_radius;
  }
  
  /*
  * Take input from the controller in the form:
  *   [stick1_x, stick1_y, stick2_x]
  */
  void take_input(float input[]) {
    // Unpack input into thrust values
    if (input[0] == 0 && input[1] == 0) {
      thrust = getRotation(input[2]);
    } else {
      thrust = getTranslation(input[0], input[1]);
    }
    
    // Use trig to find thrust along the x and y axis
    force[0] = (-thrust[0]*sin(PI/4)) + (thrust[1]*sin(PI/4)) + (thrust[2]*sin(PI/4)) + (-thrust[3]*sin(PI/4));
    force[1] = (-thrust[0]*cos(PI/4)) + (-thrust[1]*cos(PI/4)) + (thrust[2]*cos(PI/4)) + (thrust[3]*cos(PI/4));
    
    // Use F=ma to find the acceleration along x and y
    acceleration[0] = force[0]/mass;
    acceleration[1] = force[1]/mass;
    
    // Calculate the torque on the craft (assuming motors are at right angles to radii, so no trig)
    torque = (thrust[0]) + (-thrust[1]) + (thrust[2]) + (-thrust[3]); // Not actually torque, as I've left out the radius term, as it will cancel later
    
    // Use T=I*alpha, modeling the craft as a flat disc
    //    More specifically, alpha = T/(M*R), once some Rs have cancelled out
    alpha = torque/(mass*radius);
  }
  
  void move() {
    // Update speed with acceleration
    speed[0] += acceleration[0];
    speed[1] += acceleration[1];
    
    // Update position with speed
    pos[0] += speed[0];
    pos[1] += speed[1];
    
    //Constrain
    pos[0] = constrain(pos[0], 0, width);
    pos[1] = constrain(pos[1], 0, height);
    if (pos[0] == 0 || pos[0] == width) { speed[0] = 0; }
    if (pos[1] == 0 || pos[1] == height) { speed[1] = 0; }
    
    // Update omega with alpha
    omega += alpha;
    
    // Update angle with omega
    theta += omega;
  }
  
  void display() {
    pushMatrix(); // Save current transformation
    
    // Center coordinate system
    translate(pos[0], pos[1]);
    rotate(theta);
    
    // Draw craft body
    fill(0);
    beginShape();
    rect(-w/2, -h/2, w, h, 7);
    endShape();
    
    // Restore previous transformation
    popMatrix();
  }
  
  // Helper method to get motor values from translational stick input
  float[] getTranslation(float x, float y){
    float[] vals = new float[4];
    float a = (float)((x + y)*aConst);
    float b = -(float)((y - x)*bConst);
    float d = -a;
    float c = -b;
    vals[0] = a;
    vals[1] = b;
    vals[2] = c;
    vals[3] = d;
    return vals;
  }
  
  // Helper method to get motor values from rotational stick input
  float[] getRotation(float x) {
    float[] vals = new float[4];
    float a = (float)(x*aConstR);
    float b = -(float)(x*bConstR);
    float d = a;
    float c = b;
    vals[0] = a;
    vals[1] = b;
    vals[2] = c;
    vals[3] = d;
    return vals;
  }
}
