#include "VideoSource.h"

void VideoSource::setup(){
    grabber.setVerbose(true);
    grabber.setDeviceID(0);
    grabber.setUseTexture(true);
    grabber.initGrabber(640, 480);
}

void VideoSource::update(){
    grabber.update();
}

ofTexture& VideoSource::getTexture(){
    return grabber.getTexture();
}