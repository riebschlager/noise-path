#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle
{

public:
  Particle(ofVec2f position, float lifetime);

  ofVec2f mPosition;
  ofVec2f mVelocity;
  ofVec2f mNoiseVec;

  float mNoiseFloat;
  float mLifetime;

  uint mAge;

  bool mIsDead;

  void update(float stepX, float stepY, float stepZ, float multX, float multY, float damp);
};

#endif
