#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "particle.hpp"

class ofApp : public ofBaseApp
{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void saveFbo();
	void onSaveImagePressed();
	void onClearPressed();
	void loadSlices(string folderName);
	void loadSlices(string folderName, uint maxWidth, uint maxHeight);
	void loadSlicesFromSource(string folderName, uint numberOfSlices, uint maxWidth, uint maxHeight);
	void loadSources(string folderName);
	ofVec2f resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight);

	list<Particle> mParticles;
	vector<ofImage> mSlices;
	vector<ofImage> mSources;

	bool mIsDrawing;
	int mCurrentSourceIndex;

	ofVec2f mMousePos;
	ofFbo mCanvas;
	ofxPanel mGui;
	ofxFloatSlider mNoiseScaleX;
	ofxFloatSlider mNoiseScaleY;
	ofxFloatSlider mNoiseScaleZ;
	ofxFloatSlider mMinLifetime;
	ofxFloatSlider mMaxLifetime;
	ofxFloatSlider mMinRotation;
	ofxFloatSlider mMaxRotation;
	ofxFloatSlider mMinScale;
	ofxFloatSlider mMaxScale;
	ofxFloatSlider mMinAlpha;
	ofxFloatSlider mMaxAlpha;
	ofxFloatSlider mMultX;
	ofxFloatSlider mMultY;
	ofxFloatSlider mFriction;
	ofxFloatSlider mSourceChangeFrequency;
	ofxIntSlider mDrawsPerFrame;
	ofxButton mSaveImage;
	ofxButton mClear;
};
