#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableSmoothing();

    newFramesAvailable = false;

    httpURL = "http://ws.cdn.bom.gov.au/radar/";
    bgURL = "http://ws.cdn.bom.gov.au/products/radar_transparencies/";

    currentFrame = 0;
    //IDR021
    //IDR022
    //IDR023
    //IDR024
    idIndex = 3;
    id = ids[idIndex];
    frames = getFrames(id);
    backgrounds = getBackgrounds(id);

    frameTimer = ofGetElapsedTimeMillis();
    pollTimer = frameTimer;
    pollInterval = (20 * 1000);
    frameInterval = 200;
    normalizedFrameTimer = 0.0;

    barBuffer = true;
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
vector<ofImage> ofApp::getFrames(string _id){

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

        if(fileparts[0] == _id && fileparts[fileparts.size()-1] == "png")
        {
            cout << "adding " + filename << endl;
            ofImage foundImage;
            foundImage.setUseTexture(false);
            foundImage.loadImage(httpURL + filename );
            foundFrames.push_back(foundImage);
        }
    }

    return foundFrames;
}

//--------------------------------------------------------------
void ofApp::update(){


    if(newFramesAvailable)
    {
        cout << "swapping frames" << endl;
        cout << "new frame size = " << ofToString(newFrames.size()) << endl;
        frames = newFrames;
        newFramesAvailable = false;
    }


    if(ofGetElapsedTimeMillis() - frameTimer > frameInterval)
    {

        currentFrame = (currentFrame + 1)%(frames.size()-1);
        if(currentFrame == 0)
            barBuffer = !barBuffer;
        frameTimer = ofGetElapsedTimeMillis();

    }

    if(ofGetElapsedTimeMillis() - pollTimer > pollInterval)
    {
        startThread();
        pollTimer = ofGetElapsedTimeMillis();
    }




    //calculate normalized time for driving animations
    int totalTime = (frames.size()-1) * frameInterval;
    normalizedFrameTimer = ((float)(ofGetElapsedTimeMillis() - frameTimer + currentFrame * frameInterval) / (float)totalTime );

}

//--------------------------------------------------------------
void ofApp::threadedFunction(){

    while(isThreadRunning())
    {
        newFrames = getFrames(id);
        newFramesAvailable = true;

        cout<<"new frames available"<<endl;
        stopThread();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){



    //background and topology
    for(int i = 0; i < 2; i++)
    {
        if(bLayer[i])
            backgrounds[i].draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
    }

    //radar
    if(frames[currentFrame].isAllocated())
    {
        ofTexture frameTexture;
        frameTexture.loadData(frames[currentFrame]);
        //frames[currentFrame].draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
        frameTexture.draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
    }


    //labels and scope
    for(int i = 3; i > 1; i--)
    {
        if(bLayer[i])
            backgrounds[i].draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
    }

    if(bLayer[4])
    {
        float barHeight = 120;

        ofColor color1(225, 213, 174);
        ofColor color2(182, 207, 231);
        color1.setSaturation(200);
        color2.setSaturation(200);

        if(barBuffer)
        {
            ofColor tempColor = color1;
            color1 = color2;
            color2 = tempColor;
        }

        ofPushStyle();
        ofSetColor(color1);
        ofRect(0.0, ofGetHeight() - barHeight*0.9,ofGetWidth(), barHeight/10);
        ofSetColor(color2);
        ofRect(0.0, ofGetHeight() - barHeight*0.9, normalizedFrameTimer * ofGetWidth(), barHeight/10);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ')
    {
        idIndex++;
        idIndex %= 4;
        id = ids[idIndex];
        frames = getFrames(id);
        backgrounds = getBackgrounds(id);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key == '1')
        bLayer[0] = !bLayer[0];
    if(key == '2')
        bLayer[1] = !bLayer[1];
    if(key == '3')
        bLayer[2] = !bLayer[2];
    if(key == '4')
        bLayer[3] = !bLayer[3];
    if(key == '5')
        bLayer[4] = !bLayer[4];
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

