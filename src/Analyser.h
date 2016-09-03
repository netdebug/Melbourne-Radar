#pragma once
#include "ofMain.h"
#include "Cell.h"

class Analyser{
public:
    void setup();
    void analyseFrame(ofTexture& tex);
    vector<Cell>& getCells();
    ofTexture& getVideoTexture();
private:
    vector<Cell> cells;
    ofTexture videoTexture;
    void createVideoTexture();
    float threshold;
};