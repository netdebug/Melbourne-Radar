#include "Analyser.h"
#define GRID_X 16
#define GRID_Y 16

void Analyser::setup(){
    videoTexture.allocate(GRID_X, GRID_Y, GL_RGBA);
    videoTexture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    for(int i = 0; i < GRID_X * GRID_X; i++){
        cells.push_back(*new Cell());
    }
}

void Analyser::analyseFrame(ofTexture& tex){
    ofPixels pixels;
    tex.readToPixels(pixels);
    pixels.resize(GRID_X, GRID_Y);
    
    videoTexture.loadData(pixels);
    
    for(int p = 0; p < GRID_X * GRID_Y; p++){
        ofColor c = pixels.getColor(p);
        cells[p].x = p%GRID_X;
        cells[p].y = p/GRID_X;
        cells[p].lastc = cells[p].c;
        cells[p].c = c.getHex();
    }
}

vector<Cell>& Analyser::getCells(){
    return cells;
}

ofTexture& Analyser::getVideoTexture(){
    return videoTexture;
}