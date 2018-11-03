#include "ofApp.h"
#include "particle.hpp"

void ofApp::setup()
{
    mSaveImage.addListener(this, &ofApp::onSaveImagePressed);
    mClear.addListener(this, &ofApp::onClearPressed);

    mGui.setup();
    mGui.add(mNoiseScaleX.setup("Noise Scale X", 0.005f, 0, 0.1f));
    mGui.add(mNoiseScaleY.setup("Noise Scale Y", 0.005f, 0, 0.1f));
    mGui.add(mNoiseScaleZ.setup("Noise Scale Z", 0.005f, 0, 0.1f));
    mGui.add(mMinLifetime.setup("Lifetime Min", 20.0f, 0.0f, 2000.0f));
    mGui.add(mMaxLifetime.setup("Lifetime Max", 1000.0f, 0.0f, 2000.0f));
    mGui.add(mMinScale.setup("Scale Min", 0.1f, -5.0f, 5.0f));
    mGui.add(mMaxScale.setup("Scale Max", 1.0f, -5.0f, 5.0f));
    mGui.add(mMinRotation.setup("Rotation Min", -10.0f, -360.0f, 360.0f));
    mGui.add(mMaxRotation.setup("Rotation Max", 10.0f, -360.0f, 360.0f));
    mGui.add(mMinAlpha.setup("Alpha Min", 0.0f, 0.0f, 255.0f));
    mGui.add(mMaxAlpha.setup("Alpha Max", 255.0f, 0.0f, 255.0f));
    mGui.add(mMultX.setup("mMultX", 5.0f, 0.0f, 255.0f));
    mGui.add(mMultY.setup("mMultY", 5.0f, 0.0f, 255.0f));
    mGui.add(mFriction.setup("Friction", 1.0f, 0.0f, 2.0f));
    mGui.add(mSourceChangeFrequency.setup("Source Change Frequency", 0, 0, 0.1));
    mGui.add(mDrawsPerFrame.setup("Draws Per Frame", 1, 1, 1000));
    mGui.add(mClear.setup("Clear"));
    mGui.add(mSaveImage.setup("Save Image"));

    mCurrentSourceIndex = 0;

    mCanvas.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    mCanvas.begin();
    ofBackground(0, 0, 0);
    mCanvas.end();

    //    loadSlices("flora");
    loadSlicesFromSource("vangogh", 100, 300, 300);
    loadSources("face");
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

void ofApp::loadSlices(string folderName)
{
    string path = "./slices/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("png");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        mSlices.push_back(img);
    }
}

void ofApp::loadSlices(string folderName, uint maxWidth, uint maxHeight)
{
    string path = "./slices/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("png");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        ofVec2f r = resizeProportionally(img.getWidth(), img.getHeight(), maxWidth, maxHeight);
        img.resize(r.x, r.y);
        mSlices.push_back(img);
    }
}

void ofApp::loadSlicesFromSource(string folderName, uint numberOfSlices, uint maxWidth, uint maxHeight)
{
    string path = "./sources/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        for (uint j = 0; j < numberOfSlices; j++)
        {
            ofImage slice;
            uint w = ofRandom(maxWidth);
            uint h = ofRandom(maxHeight);
            uint x = ofRandom(0, img.getWidth() - w);
            uint y = ofRandom(0, img.getHeight() - h);
            slice.cropFrom(img, x, y, w, h);
            mSlices.push_back(slice);
        }
    }
}

void ofApp::loadSources(string folderName)
{
    string path = "./sources/" + folderName;
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
        mParticles.push_back(Particle(mMousePos, lifetime));
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
            for (uint i = 0; i < mDrawsPerFrame; i++)
            {
                uint sliceIndex = ofMap(p->mNoiseFloat, 0, 1, 0, mSlices.size());
                p->update(mNoiseScaleX, mNoiseScaleY, mNoiseScaleZ, mMultX, mMultY, mFriction);
                ofPushMatrix();
                ofTranslate(p->mPosition.x, p->mPosition.y);
                uint x = ofClamp(p->mPosition.x, 0, mCanvas.getWidth());
                uint y = ofClamp(p->mPosition.y, 0, mCanvas.getHeight());
                ofColor color = mSources[mCurrentSourceIndex].getColor(x, y);
                ofScale(ofMap(p->mNoiseFloat, 0, 1, mMinScale, mMaxScale, true));
                ofRotateDeg(ofMap(p->mNoiseFloat, 0, 1, mMinRotation, mMaxRotation));
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
