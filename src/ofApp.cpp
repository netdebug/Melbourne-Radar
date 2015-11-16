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
    frameTextures = imagesToTextures(frames);
    backgrounds = getBackgrounds(id);
    backgroundTextures = imagesToTextures(backgrounds);

    frameTimer = ofGetElapsedTimeMillis();
    pollTimer = frameTimer;
    pollInterval = (40 * 1000);
    frameInterval = 300;
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
    bg.setUseTexture(false);

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

    cout << "getting frames..." << endl;
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
            if(foundImage.isAllocated())
            {
                foundFrames.push_back(foundImage);
                //something like this....
                //201511151200
                //newFrameTimes.push_back(fileParts[2]);

            }
        }
    }

    return foundFrames;
}

//--------------------------------------------------------------
void ofApp::update(){


    //refresh frames when thread has finished loading
    if(newFramesAvailable)
    {
        cout << "swapping frames" << endl;
        cout << "new frame size = " << ofToString(newFrames.size()) << endl;
        frameTextures = imagesToTextures(newFrames);
        backgroundTextures = imagesToTextures(newBackgroundFrames);
        newFramesAvailable = false;
        pollTimer = ofGetElapsedTimeMillis();
    }


    //display next frame in sequence
    if(ofGetElapsedTimeMillis() - frameTimer > frameInterval)
    {
        currentFrame = (currentFrame + 1)%(frames.size()-1);

        if(currentFrame == 0)
            barBuffer = !barBuffer;

        frameTimer = ofGetElapsedTimeMillis();
    }


    //look for new frames
    if(ofGetElapsedTimeMillis() - pollTimer > pollInterval)
    {

        if(!isThreadRunning())
        {
            idIndex++;
            idIndex %= 4;
            id = ids[idIndex];
            startThread();
        }

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

        newBackgroundFrames = getBackgrounds(id);

        newFramesAvailable = true;
        cout<<"new frames available"<<endl;
        stopThread();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    int topStretch = ofGetHeight()/30.72;

    //background and topology
    for(int i = 0; i < 2; i++)
    {
        if(bLayer[i])
            backgroundTextures[i].draw(0.0, 0.0 - topStretch, ofGetWidth(), ofGetHeight() + topStretch);
    }

    //radar
    if(frames[currentFrame].isAllocated())
        frameTextures[currentFrame].draw(0.0, 0.0 - topStretch, 0.0, ofGetWidth(), ofGetHeight() + topStretch);

    //labels and scope
    for(int i = 3; i > 1; i--)
    {
        if(bLayer[i])
            backgroundTextures[i].draw(0.0, 0.0 - topStretch, 0.0, ofGetWidth(), ofGetHeight() + topStretch);
    }


    //progress bar
    if(bLayer[4])
    {
        float barHeight = 120;

        color1.set(225, 213, 174, 200);
        color2.set(182, 207, 231, 200);
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
        ofRect(normalizedFrameTimer * ofGetWidth(), ofGetHeight() - barHeight*0.9,ofGetWidth(), barHeight);
        //ofRect(0.0, ofGetHeight() - barHeight*0.9,ofGetWidth(), barHeight/10);
        ofSetColor(color2);
        ofRect(0.0, ofGetHeight() - barHeight*0.9, normalizedFrameTimer * ofGetWidth(), barHeight);
        //ofRect(0.0, ofGetHeight() - barHeight*0.9, normalizedFrameTimer * ofGetWidth(), barHeight/10);
        ofPopStyle();
    }


    //TODO
    //put bars top and bottom
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == ' ')
    {
        if(!isThreadRunning())
        {
            idIndex++;
            idIndex %= 4;
            id = ids[idIndex];
            startThread();
        }
    }

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
void ofApp::keyReleased(int key){
}



vector<ofTexture> ofApp::imagesToTextures(vector<ofImage> imageVector)
{
    vector<ofTexture> textureVector;

    for(int  i = 0; i < imageVector.size(); i++)
    {
        ofTexture tex;
        tex.loadData(imageVector[i]);
        textureVector.push_back(tex);
    }

    return textureVector;
}






