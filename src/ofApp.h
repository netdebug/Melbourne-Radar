#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp, ofThread{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void threadedFunction();

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
    vector<ofImage> getFrames(string id);

    vector<ofImage> backgrounds;
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.background.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.locations.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.range.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.topography.png

    vector<ofImage> getBackgrounds(string id);

    bool newFramesAvailable;
    vector<ofImage> newFrames;

    int currentFrame;
    int frameTimer;
    int frameInterval;
    float normalizedFrameTimer;

    bool barBuffer;

    int pollTimer;
    int pollInterval;

};

