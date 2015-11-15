#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableSmoothing();

    httpURL = "http://ws.cdn.bom.gov.au/radar/";
    bgURL = "http://ws.cdn.bom.gov.au/products/radar_transparencies/";

    currentFrame = 0;
    //IDR021
    //IDR022
    //IDR023
    //IDR024
    id = "IDR021";


    frames = getFrames(id);

    backgrounds = getBackgrounds(id);
    frameTimer = ofGetElapsedTimeMillis();
    pollTimer = frameTimer;
}



//--------------------------------------------------------------
vector<ofImage> ofApp::getBackgrounds(string id)
{
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.background.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.locations.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.range.png
    //http://ws.cdn.bom.gov.au/products/radar_transparencies/IDR021.topography.png

    vector<ofImage> bgs;
    ofImage bg;

    bg.load(bgURL + id + ".background.png");
    bgs.push_back(bg);

    bg.load(bgURL + id + ".topography.png");
    bgs.push_back(bg);

    bg.load(bgURL + id + ".range.png");
    bgs.push_back(bg);

    bg.load(bgURL + id + ".locations.png");
    bgs.push_back(bg);

    return bgs;
}


//--------------------------------------------------------------
vector<ofImage> ofApp::getFrames(string id){

    ofFilePath scriptPath;
    string scriptPathString = scriptPath.getAbsolutePath("scripts/ftpGetter.py", true);
    stringstream scriptOutput;
    scriptOutput << ofSystem("python " + scriptPathString);

    vector<string> scriptLines = ofSplitString(scriptOutput.str(), "\n");

    vector<ofImage> foundFrames;

    for(auto i : scriptLines)
    {
        vector<string> filepath = ofSplitString(i, "/");
        string filename = filepath[filepath.size()-1];

        vector<string> fileparts = ofSplitString(filename, ".");

        if(fileparts[0] == id)
        {
            ofImage foundImage;
            foundImage.loadImage(httpURL + filename );
            foundFrames.push_back(foundImage);
        }
    }

    return foundFrames;
}

//--------------------------------------------------------------
void ofApp::update(){

    if(ofGetElapsedTimeMillis() - frameTimer > 200)
    {
        currentFrame = (currentFrame + 1)%(frames.size()-1);
        frameTimer = ofGetElapsedTimeMillis();
    }

    if(ofGetElapsedTimeMillis() - pollTimer > (30 * 1000))
        frames = getFrames(id);

}

//--------------------------------------------------------------
void ofApp::draw(){

    for(auto bg : backgrounds)
        bg.draw(0.0, 0.0, ofGetWidth(), ofGetHeight());

    if(frames[currentFrame].isAllocated())
        frames[currentFrame].draw(0.0, 0.0, ofGetWidth(), ofGetHeight());

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

