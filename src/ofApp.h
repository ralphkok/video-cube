#pragma once

#include "ofMain.h"
#include "ofShader.h"
#include "ofFbo.h"
#include "ofVideoPlayer.h"
#include "ofGraphics.h"
#include "LoadSaveParameters.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "ofLog.h"
#include "VoxelCube.h"

using namespace std;

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void dragEvent(ofDragInfo dragInfo);
    
private:
    ofShader mShader;
    ofFbo mFbo;
    ofVideoPlayer mVideoPlayer;
    VoxelCube mVoxelCube;
    ofEasyCam mCamera;
    
    void loadShader(string fragPath);
    void loadVideo(string path);
    
    // GUI
    ofParameter<string> mShaderPath;
    ofParameter<string> mVideoPath;
    ofParameter<float> mVideoSpeed;
    ofParameter<float> mThreshold;
    
    ofxPanel mGui;
    ofParameterGroup mGuiParams;
    ofParameterGroup mProcessingParams;
    LoadSaveParameters mGuiLoadSave;
    bool mDoShowGui = false;
    void setupGui();
    void onChangeShader(string& path);
    void onChangeVideo(string& path);
    void onChangeVideoSpeed(float& speed);
};
