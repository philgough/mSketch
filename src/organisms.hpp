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
#include "character.hpp"

class Organism {
public:
    
    Organism (int tempIndex, ofVec2f tempLocation, int tempType, map<string, string> tempData, float* phValue);
    void updateOrganism(float* pollutionVal, float *phVal);
    void drawOrganism();
    ofVec2f location;
    void callAnthony(Character* Anthony);
    bool isBeingInspected;
    ofColor organismColor;
    float organismYPos;
    
private:
    int index;
    ofVec2f startLocation;
    int type;
    float health;
    void addColours();
    ofMesh shape;
    float* phValue;
    string name;
    
    float noisex, noisey, noiser;
    ofVec2f movementArea;
    float startAngle;
    map<string, string> data;
    // env.cp value;
    float zVal;
    float brightness;

    float chance;
};

#endif /* organisms_hpp */
