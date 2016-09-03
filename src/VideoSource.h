#pragma once
#include "ofMain.h"

class VideoSource{
public:
    void setup();
    void update();
    ofTexture& getTexture();
    
private:
    ofVideoGrabber grabber;
};

