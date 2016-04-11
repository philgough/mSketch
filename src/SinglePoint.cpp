//
//  PoissonPoint.cpp
//  mySketch
//
//  Created by Phil Gough on 9/04/2016.
//
//

#include "SinglePoint.hpp"
#include "ofMain.h"
#include "ofApp.h"

SinglePoint::SinglePoint( float tx, float ty, float md, float tp, float w){
    location.set(tx, ty);
    x = tx;
    y = ty;
    setVars(md, tp);
    width = w;
}

void SinglePoint::setVars (float md, float tp) {
        minDist = md;
        testPoints = tp;
        cellSize = minDist/sqrt(2);
        pixelArrayIndex = imageToGrid();
    }
    
void SinglePoint::get2DVector() {
        return location;
    }
    
int SinglePoint::imageToGrid() {
        return (int) (location.x/cellSize) + (location.y/cellSize) * width;
    }
