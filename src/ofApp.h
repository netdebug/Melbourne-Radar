#pragma once
#include "ofMain.h"
#include "VideoSource.h"
#include "Analyser.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    VideoSource videoSource;
    Analyser analyser;
};

