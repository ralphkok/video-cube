#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setSize(1024, 1024);
    settings.setGLVersion(3, 2);
    settings.windowMode = OF_WINDOW;
    auto window = ofCreateWindow(settings);
    
    ofRunApp(new ofApp());
}
