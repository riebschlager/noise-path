#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>
#include "ofMain.h"

class Particle
{

  public:
    Particle(ofVec2f position, ofVec2f velocity, float lifetime, float radius, ofColor color);

    ofVec2f mPosition;
    ofVec2f mVelocity;
    ofVec2f mNoiseVec;

    float mNoiseFloat;
    float mLifetime;
    float mRadius;
    float mAge;

    bool mIsDead;

    ofColor mColor;

    void update(float stepX, float stepY, float stepZ, float multX, float multY, float damp);
    void draw(ofImage *slice);
};

#endif
