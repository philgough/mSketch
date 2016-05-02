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
    
    Organism (int tempIndex, ofVec2f tempLocation, int tempType, string name);
    void updateOrganism(float* healthIndex);
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
};

#endif /* organisms_hpp */
