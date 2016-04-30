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
//#include "ofApp.h"
//#include "PoissonPoints.hpp"


class Organism {
public:
    
    Organism (int tempIndex, ofVec2f tempLocation, int type, string name);
    
    void drawOrganism();
    
private:
    int index;
    ofVec2f location;
    int type;
    float health;
};

#endif /* organisms_hpp */
