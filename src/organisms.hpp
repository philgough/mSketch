//
//  organisms.hpp
//  phd-installation
//
//  Created by Phil Gough on 30/04/2016.
//
//

#ifndef organisms_hpp
#define organisms_hpp

#include <stdio.h>

#include "ofMain.h"
#include "ofxVoronoi.h"

class Organism {
public:
    
    Organism (int tempIndex, ofVec2f tempLocation, int tempType, map<string, string> tempData);
    void updateOrganism(float* healthIndex, float *phVal);
    void drawOrganism();
    ofVec2f location;
    
private:
    int index;
    ofVec2f startLocation;
    int type;
    float health;
    void addColours();
    ofMesh shape;
    
    float noisex, noisey, noiser;
    ofVec2f movementArea;
    float startAngle;
    map<string, string> data;
    // env.cp value;
    float zVal;
    float brightness;
    
    // calculate log values
    float py(float i, float h, float k);
    float qy(float i, float h, float k);
    float ry(float i, float h, float k);
    float sy(float i, float h, float k);

    
    
};

#endif /* organisms_hpp */
