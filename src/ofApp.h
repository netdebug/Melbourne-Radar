#pragma once

#include "ofMain.h"
#include "ofxOSC.h"
#define GRID_X 16
#define GRID_Y 16

class ofApp : public ofBaseApp, ofThread{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

    void threadedFunction();
    vector<ofTexture> imagesToTextures(vector<ofImage> imageVector);
    vector<string> arguments;
    vector<string>  ids;
    string id = "IDR022";
    int idIndex = 0;
    
    bool bLayer[6] = {true, true, true, true, false, false};

    string httpURL;
    string bgURL;

    vector<ofImage> frames;
    vector<ofTexture> frameTextures;
    bool newFramesAvailable;
    vector<ofImage> newFrames;
    vector<ofImage> getFrames(string id);
    ofFbo pixBuff;
    float zoom =  0.04;
    
    void getIDs();
    void incrementID();

    vector<ofImage> backgrounds;
    vector<ofTexture> backgroundTextures;
    vector<ofImage> newBackgroundFrames;
    vector<ofImage> getBackgrounds(string id);
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.background.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.locations.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.range.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.topography.png

    vector<string> frameTimes;
    vector<string> newFrameTimes;
    int currentFrame;
    int frameTimer;
    int frameInterval;
    float normalizedFrameTimer;
    int pollTimer;
    int pollInterval;
    
    
    ofxOscSender oscSender;
    vector<ofxOscBundle> oscFrames;
    vector<ofxOscBundle> makeOscFrames(vector<ofTexture> textures);
    

};

