#include "particle.hpp"

Particle::Particle(ofVec2f position, ofVec2f velocity, float lifetime, float radius, ofColor color)
{
    mPosition = position;
    mVelocity = velocity;
    mLifetime = lifetime;
    mRadius = radius;
    mColor = color;
    mAge = 0.0f;
    mIsDead = false;
}

void Particle::update(float stepX, float stepY, float stepZ, float multX, float multY, float damp)
{

    if (1.0f - mAge / mLifetime <= 0.0f)
    {
        mIsDead = true;
    }

    if (mPosition.x < 0 || mPosition.x > ofGetWidth())
    {
        mIsDead = true;
    }

    if (mPosition.y < 0 || mPosition.y > ofGetHeight())
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

void Particle::draw(ofImage *slice)
{
    slice->draw(mPosition.x, mPosition.y);
}
