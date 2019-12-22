#define MOONS 4
#define DIMENSIONS 3

struct SpaceBody {
    int position[DIMENSIONS];
    int velocity[DIMENSIONS];
    float position_f[DIMENSIONS];  // Float for smooth animation

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
SpaceBody callisto(-7, -2, 14);

SpaceBody moons[MOONS] {io, europa, ganymede, callisto};


void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(10);
}

void loop() {

  const int animation_steps = 10;
  
  // apply gravity
  for (int i = 0; i < MOONS; i++) {
      for (int j = i + 1; j < MOONS; j++) {
          SpaceBody &moon1 = moons[i];
          SpaceBody &moon2 = moons[j];
          for (int axis = 0; axis < DIMENSIONS; axis++) {
              if (moon1.position[axis] != moon2.position[axis]) {
                  moon1.velocity[axis] += sign(moon2.position[axis] - moon1.position[axis]);
                  moon2.velocity[axis] += sign(moon1.position[axis] - moon2.position[axis]);
              }
          }
      }
  }
  
  // apply velocity
  for (auto &moon : moons) {
    for (int i = 0; i < DIMENSIONS; i++) {
      moon.position_f[i] = moon.position[i];
      moon.position[i] += moon.velocity[i];
    }
  }
  
  // Animation
  for (int i = 0; i < animation_steps; i++) {
    for (auto &moon : moons) {
      for (int i = 0; i < DIMENSIONS-1; i++) {
        moon.position_f[i] += (float)moon.velocity[i] / animation_steps;
      }
    }
    // show moons
    for (int j = 0; j < MOONS; j++) {
      int x = 512 + moons[j].position_f[0]*4.0;
      int y = 512 + + moons[j].position_f[1]*4.0;

      if (x >= 0 && x < 1024 && y >=0 && y < 1024) {
        analogWrite(DAC0, x);
        analogWrite(DAC1, y);
        delayMicroseconds(2000);
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
    } else {
        return 0;
    }
}
