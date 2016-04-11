//
//  PoissonPoints.hpp
//  mySketch
//
//  Created by Phil Gough on 9/04/2016.
//
//

#ifndef PoissonPoints_hpp
#define PoissonPoints_hpp

#include <stdio.h>
#include "SinglePoint.hpp"
#include "ofMain.h"
#include "ofApp.h"



class PoissonPoints {
public:
    PoissonPoints(float tempMaxNumPoints, float tempMinDist, float tempTestPoints, float width, float height);
    
    
    ofVec2f getPPLocation (int i);
    
    int numLocations();

    
    vector<ofVec2f> ppLocations;
    vector<class SinglePoint> pp;
    vector<int> checklist;

private:
    float maxNumPoints, minDist, testPoints, pointSize;
    

    
    vector<ofVec2f> getPoints();
    
    bool activePoints();
    
    ofVec2f newRandomPoint (ofVec2f point, float minDist);
    
    bool findNewPoint (ofVec2f newPoint, float w, float h);
    
    bool inBounds(ofVec2f p, float w, float h);
    
    bool tooClose(ofVec2f newPoint);
    
    bool checkPoints (ofVec2f newPoint, float w, float h);
    
};

#endif /* PoissonPoints_hpp */
