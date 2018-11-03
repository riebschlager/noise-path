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

	list<Particle> mParticles;

	bool mIsDrawing;
    int mCurrentSourceIndex;

	ofVec2f mMousePos;
	ofFbo mCanvas;
    ofxPanel mGui;

	ofxFloatSlider mStepX;
	ofxFloatSlider mStepY;
	ofxFloatSlider mStepZ;
    ofxFloatSlider mMinLifetime;
    ofxFloatSlider mMaxLifetime;
    ofxFloatSlider mMinScale;
    ofxFloatSlider mMaxScale;
    ofxFloatSlider mMinAlpha;
    ofxFloatSlider mMaxAlpha;
    ofxFloatSlider mMultX;
    ofxFloatSlider mMultY;
    ofxFloatSlider mDamp;
    ofxFloatSlider mSourceChangeFrequency;
    ofxButton mSaveImage;
    ofxButton mClear;


	ofVec2f resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight);
    
	void loadSlices();
	void loadSlicesFromSource();
	void loadSources();

	vector<ofImage> mSlices;
	vector<ofImage> mSources;
};
