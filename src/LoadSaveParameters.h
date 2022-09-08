//
//  LoadSaveParameters.h
//  SecondSun
//
//  Created by Ralph Kok on 25/08/2021.
//

#ifndef LoadSaveParameters_h
#define LoadSaveParameters_h

#include "ofLog.h"
#include "ofParameter.h"
#include "ofxButton.h"
#include "ofxPanel.h"
#include "ofJson.h"

class LoadSaveParameters {
public:
    void setup(ofParameterGroup* parameters, std::string defaultFilePath) {
        mParameters = parameters;

        mPanel.setName("Load/Save");
        mPanel.add(mFilePath.set("Settings file", defaultFilePath));
        mPanel.add(mLoadButton.setup("Load"));
        mPanel.add(mSaveButton.setup("Save"));
        
        mLoadButton.addListener(this, &LoadSaveParameters::onLoadSettings);
        mSaveButton.addListener(this, &LoadSaveParameters::onSaveSettings);
        
        onLoadSettings();
    }

    ofxPanel& getPanel() {
        return mPanel;
    }
    
    void reload() {
        onLoadSettings();
    }
    
    void loadFromPath(std::string filePath) {
        mFilePath.set(filePath);
        onLoadSettings();
    }
    
    void saveToPath(std::string filePath) {
        mFilePath.set(filePath);
        onSaveSettings();
    }

protected:
    virtual void onLoadSettings() {
        ofLogNotice() << "Loading configuration file from " << mFilePath.get();
        mData = ofLoadJson(mFilePath.get());
        ofDeserialize(mData, *mParameters);
    }
    
    virtual void onSaveSettings() {
        ofSerialize(mData, *mParameters);
        ofSavePrettyJson(mFilePath.get(), mData);
    }
    
private:
    ofParameterGroup* mParameters;
    ofxPanel mPanel;
    ofJson mData;
    ofParameter<std::string> mFilePath;
    ofxButton mSaveButton;
    ofxButton mLoadButton;
};

#endif /* LoadSaveParameters_h */
