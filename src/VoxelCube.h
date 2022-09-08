//
//  VoxelCube.h
//  TextureCube
//
//  Created by Ralph Kok on 05/09/2022.
//

#ifndef VoxelCube_h
#define VoxelCube_h

#include "ofFbo.h"
#include "of3dPrimitives.h"
#include "ofGraphics.h"
#include "ofParameter.h"
#include "ofLog.h"
#include "ofShader.h"
#include "ofVboMesh.h"

class VoxelCube {
public:
    void setup() {
        mCurLayerIndex = 0;
        setupParameters();
        mShader.setGeometryInputType(GL_POINTS);
        mShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
        mShader.setGeometryOutputCount(36);
        mShader.load("shaders/voxelcube.vert", "shaders/voxelcube.frag", "shaders/voxelcube.geom");
    }
    
    void update(ofFbo& f) {
        ofFbo* layer;
        layer = &mLayers.at(mCurLayerIndex);
        layer->begin();
            ofClear(0);
            f.draw(0, 0, layer->getWidth(), layer->getHeight());
        layer->end();
        mCurLayerIndex = (mCurLayerIndex + 1) % mLayers.size();
    }
    
    void draw() {
        glActiveTexture(GL_TEXTURE0 + mTexArrayHandle);
        glClientActiveTexture(GL_TEXTURE0 + mTexArrayHandle);
        glEnable(GL_TEXTURE_2D_ARRAY);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTexArrayHandle);

        mPixels.clear();
        for (int i = 0; i < mLayers.size(); ++i) {
            unsigned int idx = (mCurLayerIndex + i) % int(mParamResolution.get().z);
            mLayers.at(idx).readToPixels(mPixels);
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, mParamResolution.get().x, mParamResolution.get().y, 1, GL_RGBA, GL_UNSIGNED_BYTE, mPixels.getData());
        }
        
        mShader.begin();
        mShader.setUniformTexture("uTextures", GL_TEXTURE_2D_ARRAY, mTexArrayHandle, 0);
        mShader.setUniform3f("uResolution", mParamResolution.get());
        mShader.setUniform3f("uSize", mParamSize.get());
        mShader.setUniform1f("uVoxelSize", mParamVoxelSize.get());
        mShader.setUniform1i("uVariableVoxelSize", mParamVariableVoxelSize.get() ? 1 : 0);
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);
            mMesh.draw();
            glEnd();
            ofDisableAlphaBlending();
            ofPopStyle();
        mShader.end();
        
        glActiveTexture(GL_TEXTURE0 + mTexArrayHandle);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glDisable(GL_TEXTURE_2D_ARRAY);
        glActiveTexture(GL_TEXTURE0);
    }
    
    void clear() {
        for (auto layer : mLayers) {
            layer.begin();
                ofClear(0);
            layer.end();
        }
        mCurLayerIndex = 0;
    }
    
    ofParameterGroup& getParameters() {
        return mParams;
    }
    
private:
    unsigned int mCurLayerIndex;
    ofFboSettings mFboSettings;
    vector<ofFbo> mLayers;
    ofPixels mPixels;
    ofShader mShader;
    GLuint mTexArrayHandle;
    ofVboMesh mMesh;
    
    ofParameter<glm::vec3> mParamSize;
    ofParameter<glm::vec3> mParamResolution;
    ofParameter<float> mParamVoxelSize;
    ofParameter<bool> mParamVariableVoxelSize;
    ofParameterGroup mParams;
    
    void setupParameters() {
        mParamResolution.addListener(this, &VoxelCube::onChangeResolution);
        
        mParams.setName("Voxel Cube");
        mParams.add(mParamSize.set("Size", glm::vec3(1000, 1000, 100), glm::vec3(1, 1, 1), glm::vec3(10000, 10000, 10000)));
        mParams.add(mParamResolution.set("Resolution", glm::vec3(100, 100, 2), glm::vec3(1, 1, 1), glm::vec3(1000, 1000, 1000)));
        mParams.add(mParamVoxelSize.set("Voxel size", 1.0, 0.0, 100.0));
        mParams.add(mParamVariableVoxelSize.set("Variable voxel size", false));
    }
    
    void onChangeResolution(glm::vec3& res) {
        ofFboSettings settings;
        settings.width = res.x;
        settings.height = res.y;
        settings.numSamples = 0;
        settings.internalformat = GL_RGBA;
        
        mLayers.clear();
        mCurLayerIndex = 0;
        mMesh.clear();
        mMesh.setMode(OF_PRIMITIVE_POINTS);
        for (int z = 0; z < res.z; z++) {
            ofFbo fbo;
            fbo.allocate(settings);
            fbo.begin();
                ofClear(0);
            fbo.end();
            mLayers.push_back(fbo);
            for (int x = 0; x < res.x; ++x) {
                for (int y = 0; y < res.y; ++y) {
                    mMesh.addVertex({0, 0, 0}); // position will be set from the texture in the vertex shader
                    mMesh.addTexCoord({x, y + z * res.y});
                }
            }
        }
        ofLogNotice() << mMesh.getVertices().size() << " vertices";
        setupTextureArray();
    }
    
    void setupTextureArray() {
        GLsizei width = mParamResolution.get().x;
        GLsizei height = mParamResolution.get().y;
        GLsizei layerCount = mParamResolution.get().z;
        GLsizei mipLevelCount = 1;

        // GLuint ofApp::texture holds the handle for the texture array
        glGenTextures(1, &mTexArrayHandle);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTexArrayHandle);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGB8, width, height, layerCount);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
};

#endif /* VoxelCube_h */
