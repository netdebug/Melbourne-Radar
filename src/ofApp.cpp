#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    ofDisableSmoothing();
//    ofDisableAntiAliasing();
//    ofEnableAlphaBlending();
    oscSender.setup("127.0.0.1", 5050);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    newFramesAvailable = false;

    httpURL = "http://ws.cdn.bom.gov.au/radar/";
    bgURL = "http://ws.cdn.bom.gov.au/products/radar_transparencies/";

    currentFrame = 0;
    getIDs();
    id = ids[idIndex];
    frames = getFrames(id);
    frameTextures = imagesToTextures(frames);
    oscFrames = makeOscFrames(frameTextures);
    backgrounds = getBackgrounds(id);
    backgroundTextures = imagesToTextures(backgrounds);

    frameTimer = ofGetElapsedTimeMillis();
    pollTimer = frameTimer;
    pollInterval = (10 * 1000);
    frameInterval = 300;
    normalizedFrameTimer = 0.0;

    pixBuff.allocate(GRID_X,GRID_Y,GL_RGBA);
    pixBuff.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    pixBuff.begin();
    ofClear(0,0);
    pixBuff.end();
    
    
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
void ofApp::getIDs(){

    cout << "getting ids..." << endl;
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

        if(fileparts[fileparts.size()-1] == "gif")
        {
            cout << "id found: " + fileparts[0] << endl;
            ids.push_back(fileparts[0]);
        }
    }
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
            foundImage.load(httpURL + filename );
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
        cout << "#new frames found = " << ofToString(newFrames.size()) << endl;
        //if images failed to load don't swap buffers
        if(newFrames.size() > 0)
        {
            frameTextures.clear();
            frameTextures = imagesToTextures(newFrames);
            oscFrames.clear();
            oscFrames = makeOscFrames(frameTextures);
            backgroundTextures.clear();
            backgroundTextures = imagesToTextures(newBackgroundFrames);
        }else{
            cout << "frame buffer not swapped" << endl;
            if(!isThreadRunning())
            {
                incrementID();
                startThread();
            }
        }
        newFramesAvailable = false;
        pollTimer = ofGetElapsedTimeMillis();
        cout << endl;
    }

    //display next frame in sequence
    if(ofGetElapsedTimeMillis() - frameTimer > frameInterval)
    {
        currentFrame = (currentFrame + 1);
        if(currentFrame > (frameTextures.size()-1))
            currentFrame = 0;
        
        //SEND OSC
        if(currentFrame < oscFrames.size())
            oscSender.sendBundle(oscFrames[currentFrame]);

        frameTimer = ofGetElapsedTimeMillis();
    }


    //look for new frames
    if(ofGetElapsedTimeMillis() - pollTimer > pollInterval)
    {

        if(!isThreadRunning())
        {
            incrementID();
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
        newFrames.clear();
        newFrames = getFrames(id);
        newBackgroundFrames.clear();
        newBackgroundFrames = getBackgrounds(id);
        newFramesAvailable = true;
        cout<<"new frames available"<<endl;
        stopThread();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	float x = 0.0 - ofGetWidth() * zoom;
	float y = 0.0 - ofGetHeight() * zoom;
	float w = ofGetWidth() + 2.0 * ofGetWidth() * zoom;
	float h = ofGetHeight() + 2.0 * ofGetHeight() * zoom;    
    
    ofClear(0,0);
    ofBackground(0);

    
    //BG and TOPOLOGY
    //layers 0, 1
    for(int i = 0; i < 2; i++)
        if(bLayer[i])
            backgroundTextures[i].draw(x,y,w,h);

    //RADAR
    //layer 2
    if(bLayer[2])
    if(frames[currentFrame].isAllocated() && currentFrame < frameTextures.size())
            frameTextures[currentFrame].draw(x,y,w,h);
        
    //PIXELS
    //layer 3
    if(bLayer[3]){
	    float px = 0.0 - pixBuff.getWidth() * zoom;
	    float py = 0.0 - pixBuff.getHeight() * zoom;
	    float pw = pixBuff.getWidth() + 2.0 * pixBuff.getWidth() * zoom;
	    float ph = pixBuff.getHeight() + 2.0 * pixBuff.getHeight() * zoom;
        
        pixBuff.begin();
        ofClear(0.0,0.0);
        
	    if(frames[currentFrame].isAllocated() && currentFrame < frameTextures.size())
	        frameTextures[currentFrame].draw(px,py,pw,ph);
	    
        pixBuff.end();
        pixBuff.draw(0.0, 0.0, ofGetWidth(), ofGetHeight());
    }

    //LABELS
    //layer 4
    if(bLayer[4])
        backgroundTextures[2].draw(x,y,w,h);
    
    //SCOPE
    //layer 5
    if(bLayer[5])
        backgroundTextures[3].draw(x,y,w,h);
}




//--------------------------------------------------------------
void ofApp::incrementID(){
    idIndex++;
    idIndex %= ids.size();
    id = ids[idIndex];
    cout << "new id = " << id << endl;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == ' ')
        if(!isThreadRunning())
        {
            incrementID();
            startThread();
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
    if(key == '6')
        bLayer[5] = !bLayer[5];
}



//--------------------------------------------------------------
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



vector<ofxOscBundle> ofApp::makeOscFrames(vector<ofTexture>textures){
    vector<ofxOscBundle> bundles;
    
    for(auto t : textures){
        ofPixels pixels;
        t.readToPixels(pixels);
        pixels.resize(GRID_X, GRID_Y);
        
        ofxOscBundle bundle;
        
        for(int p = 0; p < GRID_X * GRID_Y; p++){
            ofColor c = pixels.getColor(p);
            int r = c.r;
            int g = c.g;
            int b = c.b;
            
            if(r+g+b>0)
            {
                ofxOscMessage m;
                m.setAddress("/grid/p/" + ofToString(p));
                int ch = c.getHex();
                m.addIntArg(ch);
                bundle.addMessage(m);

            }
        }
        bundles.push_back(bundle);
    }
    return bundles;
}




