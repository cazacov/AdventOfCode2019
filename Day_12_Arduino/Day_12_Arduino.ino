struct SpaceBody {
    double position[3];
    int velocity[3];

    SpaceBody(int x, int y, int z)  {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        velocity[0] = velocity[1] = velocity[2] = 0;
    }
};

int sign(int i);

SpaceBody io (4,1,1);
SpaceBody europa(11, -18, -1);
SpaceBody ganymede(-2, -10, -4);
SpaceBody calisto(-7, -2, 14);

SpaceBody moons[4] {io, europa, ganymede, calisto};


void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(8);
}

void loop() {

  const int animation_steps = 10;
  
  // apply gravity
  for (int i = 0; i < 4; i++) {
      for (int j = i + 1; j < 4; j++) {
          SpaceBody &moon1 = moons[i];
          SpaceBody &moon2 = moons[j];
          for (int axis = 0; axis < 3; axis++) {
              if (moon1.position[axis] != moon2.position[axis]) {
                  moon1.velocity[axis] += sign(moon2.position[axis] - moon1.position[axis]);
                  moon2.velocity[axis] += sign(moon1.position[axis] - moon2.position[axis]);
              }
          }
      }
  }
  
  // apply velocity
  for (int i = 0; i < animation_steps; i++) {
    for (auto &moon : moons) {
      for (int i = 0; i < 3; i++) {
        moon.position[i] += (double)moon.velocity[i] / animation_steps;
      }
    }
    // show moons
    for (int j = 0; j < 4; j++) {
      int x = 128 + moons[j].position[0];
      int y = 128 + + moons[j].position[1];

      if (x >= 0 && x < 256 && y >=0 && y < 256) {
        analogWrite(DAC0, x);
        analogWrite(DAC1, y );
        delayMicroseconds(1000);
      }
    }
  }
  
 analogWrite(DAC0, 0);
 analogWrite(DAC1, 0);
}

int sign(int arg) {
    if (arg < 0) {
        return -1;
    } else if (arg > 0) {
        return 1;
    }
    else {
        return 0;
    }
}
