//
//  PoissonPoint.hpp
//  mySketch
//
//  Created by Phil Gough on 9/04/2016.
//
//

#ifndef SinglePoint_hpp
#define SinglePoint_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofApp.h"


class SinglePoint {

public:
    SinglePoint(float tx, float ty, float md, float tp, float w);
    
    bool active = true;
    ofVec2f location;
    float minDist;
    
    
    void setVars (float md, float tp);
    
    void get2DVector();
    int imageToGrid();
    

private:
    float x, y;
    int pixelArrayIndex;
    float cellSize;
    float width;
    float testPoints;

};


#endif /* SinglePoint_hpp */

