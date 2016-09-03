#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    videoSource.setup();
    analyser.setup();
}


//--------------------------------------------------------------
void ofApp::update(){
    videoSource.update();
    analyser.analyseFrame(videoSource.getTexture());
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    videoSource.getTexture().draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 100);
    analyser.getVideoTexture().draw(0, 0, ofGetWidth(), ofGetHeight());
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    cout << "#cells = " << analyser.getCells().size() << endl;
    
    for(auto c : analyser.getCells())
        cout << c.x << " " << c.y << " " << c.c << endl;
}





