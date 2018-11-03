#include "ofApp.h"
#include "particle.hpp"

void ofApp::setup()
{
    mSaveImage.addListener(this, &ofApp::onSaveImagePressed);
    mClear.addListener(this, &ofApp::onClearPressed);

    mGui.setup();
    mGui.add(mStepX.setup("mStepX", 0.005f, 0, 0.1f));
    mGui.add(mStepY.setup("mStepY", 0.005f, 0, 0.1f));
    mGui.add(mStepZ.setup("mStepZ", 0.005f, 0, 0.1f));
    mGui.add(mMinLifetime.setup("mMinLifetime", 20.0f, 0.0f, 2000.0f));
    mGui.add(mMaxLifetime.setup("mMaxLifetime", 1000.0f, 0.0f, 2000.0f));
    mGui.add(mMinScale.setup("mMinScale", 0.1f, -5.0f, 5.0f));
    mGui.add(mMaxScale.setup("mMaxScale", 1.0f, -5.0f, 5.0f));
    mGui.add(mMinAlpha.setup("mMinAlpha", 0.0f, 0.0f, 255.0f));
    mGui.add(mMaxAlpha.setup("mMaxAlpha", 255.0f, 0.0f, 255.0f));
    mGui.add(mMultX.setup("mMultX", 5.0f, 0.0f, 255.0f));
    mGui.add(mMultY.setup("mMultY", 5.0f, 0.0f, 255.0f));
    mGui.add(mDamp.setup("mDamp", 1.0f, 0.0f, 2.0f));
    mGui.add(mSourceChangeFrequency.setup("mSourceChangeFrequency", 0, 0, 0.1));
    mGui.add(mClear.setup("mClear"));
    mGui.add(mSaveImage.setup("mSaveImage"));

    mCurrentSourceIndex = 0;

    mCanvas.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    mCanvas.begin();
    ofBackground(0, 0, 0);
    mCanvas.end();

    loadSlices();
    loadSources();
}

void ofApp::onClearPressed()
{
    mCanvas.begin();
    ofBackground(0, 0, 0);
    mCanvas.end();
}

void ofApp::onSaveImagePressed()
{
    saveFbo();
}

ofVec2f ofApp::resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight)
{
    float ratio = min(maxWidth / srcWidth, maxHeight / srcHeight);
    return ofVec2f(srcWidth * ratio, srcHeight * ratio);
}

void ofApp::loadSlices()
{
    string path = "./slices/flora";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        ofVec2f r = resizeProportionally(img.getWidth(), img.getHeight(), 300, 300);
        img.resize(r.x, r.y);
        mSlices.push_back(img);
    }
}

void ofApp::loadSlicesFromSource()
{
    string path = "./sources/face";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        for (int j = 0; j < 10; j++)
        {
            ofImage slice;
            slice.cropFrom(img, ofRandom(img.getWidth()), ofRandom(img.getHeight()), ofRandom(300), ofRandom(300));
            mSlices.push_back(slice);
        }
    }
}

void ofApp::loadSources()
{
    string path = "./sources/one";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        img.resize(mCanvas.getWidth(), mCanvas.getHeight());
        mSources.push_back(img);
    }
}

void ofApp::update()
{
    if (mIsDrawing)
    {
        if (ofRandom(1) < mSourceChangeFrequency)
        {
            mCurrentSourceIndex = floor(ofRandom(mSources.size()));
        }
        ofVec2f velocity(0, 0);
        float lifetime = ofRandom(mMinLifetime, mMaxLifetime);
        float radius = ofRandom(1.0f, 5.0f);
        ofColor color = ofColor(255, 255, 255);
        mParticles.push_back(Particle(mMousePos, velocity, lifetime, radius, color));
    }
}

void ofApp::draw()
{
    mCanvas.begin();
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    for (list<Particle>::iterator p = mParticles.begin(); p != mParticles.end();)
    {
        if (p->mIsDead)
        {
            p = mParticles.erase(p);
        }
        else
        {
            for (int i = 0; i < 10; i++)
            {
                int sliceIndex = floor(ofRandom(mSlices.size()));
                p->update(mStepX, mStepY, mStepZ, mMultX, mMultY, mDamp);
                ofPushMatrix();
                ofTranslate(p->mPosition.x, p->mPosition.y);
                int x = ofClamp(p->mPosition.x, 0, mCanvas.getWidth());
                int y = ofClamp(p->mPosition.y, 0, mCanvas.getHeight());
                //ofColor color = mSources[mCurrentSourceIndex].getColor(mMousePos.x, mMousePos.y);
                ofColor color = mSources[mCurrentSourceIndex].getColor(x, y);
                ofScale(ofMap(p->mNoiseFloat, 0, 1, mMinScale, mMaxScale, true));
                ofRotateDeg(ofMap(p->mNoiseFloat, 0, 1, -360, 360));
                float alpha = ofMap(p->mNoiseFloat, 0, 1, mMinAlpha, mMaxAlpha, true);
                ofSetColor(color, alpha);
                mSlices[sliceIndex].draw(0, 0);
                ofPopMatrix();
            }
            ++p;
        }
    }
    mCanvas.end();
    mCanvas.draw(0, 0, ofGetWidth(), ofGetHeight());
    mGui.draw();
}

void ofApp::saveFbo()
{
    ofFbo img;
    ofPixels pixels;
    img.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    img.begin();
    ofBackground(0, 0, 0);
    mCanvas.draw(0, 0);
    img.end();
    img.readToPixels(pixels);
    ofSaveImage(pixels, "output/image" + ofToString(ofGetUnixTime()) + ".tif");
}

void ofApp::keyPressed(int key)
{
}

void ofApp::keyReleased(int key)
{
}

void ofApp::mouseMoved(int x, int y)
{
    float mX = ofMap(x, 0, ofGetWidth(), 0, mCanvas.getWidth());
    float mY = ofMap(y, 0, ofGetHeight(), 0, mCanvas.getHeight());
    mMousePos.set(mX, mY);
}

void ofApp::mouseDragged(int x, int y, int button)
{
    float mX = ofMap(x, 0, ofGetWidth(), 0, mCanvas.getWidth());
    float mY = ofMap(y, 0, ofGetHeight(), 0, mCanvas.getHeight());
    mMousePos.set(mX, mY);
}

void ofApp::mousePressed(int x, int y, int button)
{
    mIsDrawing = true;
}

void ofApp::mouseReleased(int x, int y, int button)
{
    mIsDrawing = false;
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
