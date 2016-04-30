//
//  organisms.cpp
//  phd-installation
//
//  Created by Phil Gough on 30/04/2016.
//
//

#include "organisms.hpp"


Organism::Organism(int tempIndex, ofVec2f tempLocation, int type, string name) {
    index = tempIndex;
    health = 1.0;
    location = tempLocation;
}



void Organism::drawOrganism() {
//    cout << "drawing organism #" << index << endl;
    ofDrawCircle(location.x, location.y, 5);
}