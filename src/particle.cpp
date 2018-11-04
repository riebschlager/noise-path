#include "particle.hpp"

Particle::Particle(ofVec2f position, float lifetime, uint sliceIndex, ofColor color)
{
    mPosition = position;
    mVelocity = ofVec2f(0, 0);
    mLifetime = lifetime;
    mAge = 0;
    mIsDead = false;
    mSliceIndex = sliceIndex;
    mColor = color;
}

void Particle::update(float stepX, float stepY, float stepZ, float multX, float multY, float damp)
{

    if (mAge > mLifetime)
    {
        mIsDead = true;
    }

    mNoiseFloat = ofNoise(mPosition.x * stepX, mPosition.y * stepY, ofGetFrameNum() * stepZ);
    mNoiseVec.x = cos(mNoiseFloat * M_PI_2 * multX);
    mNoiseVec.y = sin(mNoiseFloat * M_PI_2 * multY);
    mVelocity += mNoiseVec;
    mVelocity *= damp;
    mPosition += mVelocity;
    mAge++;
}
