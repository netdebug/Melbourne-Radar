#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp, ofThread{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);

    void threadedFunction();
    vector<ofTexture> imagesToTextures(vector<ofImage> imageVector);

    vector<string> arguments;

    //IDR021
    //IDR022
    //IDR023
    //IDR024
    string ids[4] = { "IDR021", "IDR022", "IDR023", "IDR024" };
    string id;
    int idIndex;

    bool bLayer[5] = {true, true, false, true, true};

    string httpURL;
    string bgURL;

    vector<ofImage> frames;
    vector<ofTexture> frameTextures;
    bool newFramesAvailable;
    vector<ofImage> newFrames;
    vector<ofImage> getFrames(string id);

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

    bool barBuffer;

    int pollTimer;
    int pollInterval;

};

