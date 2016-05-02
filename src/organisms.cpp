//
//  organisms.cpp
//  phd-installation
//
//  Created by Phil Gough on 30/04/2016.
//
//

#include "organisms.hpp"


Organism::Organism(int tempIndex, ofVec2f tempLocation, int tempType, string name) {
    index = tempIndex;
    health = 1.0;
    location.set(tempLocation);
    startLocation.set(tempLocation);
    type = tempType;
    
    // draw the shape
    for (int i = 0; i < 360; i += 18) {
        float radius = 25;
        float r = ofDegToRad(i);
        
        float x = sin(r) * radius;
        float y = sin(2*r) * radius/3 * sin(r) + cos(r) * radius/4;
        
        shape.addVertex(ofVec2f(x,y));
    }
    
    // movement and rotation
    noisex = ofRandom(.3);
    noisey = ofRandom(.3);
    noiser = ofRandom(.3);
    movementArea.set(ofRandom(40), ofRandom(40));
    startAngle = ofRandom(720);
}



void Organism::drawOrganism() {
    shape.enableColors();
    shape.clearColors();
    addColours();
    ofPushMatrix();
    ofTranslate(location);
    ofRotate(startAngle + 720 * ofNoise(noiser));
    shape.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    shape.draw();
    shape.disableColors();
    shape.setMode(OF_PRIMITIVE_LINE_LOOP);
    shape.draw();
    ofPopMatrix();
    
    
    
    
}


void Organism::addColours() {
    for (int i=0; i<shape.getNumVertices(); i++) {
        shape.addColor(ofFloatColor::fromHsb((type+1.1 + index/100.02)/9.5+0.0125*sin(ofDegToRad(7.5 * i  + (1.0+type)  * 12.5 * ofGetFrameNum()/20.0)), 0.75 * health, .6));
    }
}


void Organism::updateOrganism(float* healthIndex) {
    // update position and rotation
    location.set(startLocation.x + movementArea.x * ofNoise(noisex), startLocation.y + movementArea.y * ofNoise(noisey));
    noisex += 0.005 * health;
    noisey += 0.005 * health;
    noiser += 0.001 * health;
    
    // update health
    health = ofMap(*healthIndex, -175, 0, 0, 1);
}







