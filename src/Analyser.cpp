#include "Analyser.h"
#define GRID_X 16
#define GRID_Y 16


void Analyser::setup(){
    threshold = 100.0;
    videoTexture.allocate(GRID_X, GRID_Y, GL_LUMINANCE);
    videoTexture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    for(int i = 0; i < GRID_X * GRID_X; i++){
        cells.push_back(*new Cell());
    }
}


void Analyser::analyseFrame(ofTexture& tex){
    ofPixels pixels;
    tex.readToPixels(pixels);
    pixels.resize(GRID_X, GRID_Y);
    
    for(int p = 0; p < GRID_X * GRID_Y; p++){
        ofColor c = pixels.getColor(p%GRID_X, (int)p/GRID_X);
        cells[p].x = p%GRID_X;
        cells[p].y = (int)p/GRID_X;
        cells[p].lastc = cells[p].c;
        cells[p].c = c.getHex();

        if(c.getLightness() > threshold)
            cells[p].activeness = 255.0;
        else
            cells[p].activeness = 0.0;
    }
    createVideoTexture();
}


void Analyser::createVideoTexture(){
    ofPixels pixels;
    pixels.allocate(GRID_X, GRID_Y, OF_PIXELS_MONO);
    
    for(auto c : cells)
        pixels.setColor(c.x, c.y, ofColor(c.activeness));
    
    videoTexture.loadData(pixels);
}



vector<Cell>& Analyser::getCells(){
    return cells;
}


ofTexture& Analyser::getVideoTexture(){
    return videoTexture;
}

