#include "ofApp.h"

void ofApp::setup(){
    ofEnableArbTex();
    ofEnableAlphaBlending();
    
    ofFboSettings fs;
    fs.width = 1024;
    fs.height = 1024;
    fs.useDepth = false;
    fs.useStencil = false;
    fs.numSamples = 0;
    fs.internalformat = GL_RGBA;
    
    mFbo.allocate(fs);
    mFbo.begin();
    ofClear(0);
    mFbo.end();
    
    mVoxelCube.setup();
    
    setupGui();
}

void ofApp::update(){
    mVideoPlayer.update();
    if (mVideoPlayer.isFrameNew()) {
        mFbo.begin();
            ofClear(0);
            mShader.begin();
                mShader.setUniformTexture("uTexture", mVideoPlayer.getTexture(), 0);
                mShader.setUniform2f("uTextureSize", glm::vec2(mVideoPlayer.getWidth(), mVideoPlayer.getHeight()));
                mShader.setUniform2f("uOutputResolution", mFbo.getWidth(), mFbo.getHeight());
                mShader.setUniform1f("uThreshold", mThreshold);
                ofDrawRectangle(0, 0, mFbo.getWidth(), mFbo.getHeight());
            mShader.end();
        mFbo.end();
        
        mVoxelCube.update(mFbo);
    }
}

void ofApp::draw(){
    ofBackground(0);

    mCamera.begin();
        mVoxelCube.draw();
    mCamera.end();
    
    if (mDoShowGui) {
        mGui.draw();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()) + " FPS");
}

void ofApp::keyPressed(int key){
    if (key == 'g') {
        mDoShowGui = !mDoShowGui;
    }
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size() > 0) {
        string file = dragInfo.files[0];
        string ext = file.substr(file.size() - 4);
        if (ext == ".mov" || ext == ".mp4") {
            mVideoPath.set(file);
        } else if (ext == "frag") {
            mShaderPath.set(file);
        }
    }
}

void ofApp::loadShader(string fragPath) {
    mShader.load("shaders/passthrough.vert", fragPath);
}

void ofApp::loadVideo(string path) {
    mVoxelCube.clear();
    mVideoPlayer.load(path);
    mVideoPlayer.setUseTexture(true);
    mVideoPlayer.setLoopState(OF_LOOP_NORMAL);
    mVideoPlayer.setSpeed(mVideoSpeed.get());
    mVideoPlayer.play();
    ofLog(OF_LOG_NOTICE, "Playing video from file " + path);
}

void ofApp::setupGui() {
    // suppress warning messages when drawing GUI
    ofSetLogLevel("ofGLProgrammableRenderer", OF_LOG_ERROR);
    
    mShaderPath.addListener(this, &ofApp::onChangeShader);
    mVideoPath.addListener(this, &ofApp::onChangeVideo);
    mVideoSpeed.addListener(this, &ofApp::onChangeVideoSpeed);
    
    mProcessingParams.setName("Processing");
    mProcessingParams.add(mShaderPath.set("Frag shader", "shaders/default.frag"));
    mProcessingParams.add(mVideoPath.set("Video", "video/explosion.mp4"));
    mProcessingParams.add(mVideoSpeed.set("Video speed", 1.0, -10.0, 10.0));
    mProcessingParams.add(mThreshold.set("Threshold", 0.0, 0.0, 1.0));

    mGuiParams.setName("Settings");
    mGuiParams.add(mProcessingParams);
    mGuiParams.add(mVoxelCube.getParameters());
    mGuiLoadSave.setup(&mGuiParams, "settings/default.json");
    
    mGui.setup();
    mGui.add(mGuiParams);
    mGui.add(&mGuiLoadSave.getPanel());
    
    mGuiLoadSave.reload();
}

void ofApp::onChangeShader(string& path) {
    loadShader(path);
}

void ofApp::onChangeVideo(string& path) {
    loadVideo(path);
}

void ofApp::onChangeVideoSpeed(float& speed) {
    mVideoPlayer.setSpeed(speed);
}
