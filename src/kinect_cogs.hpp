//
//  kinect_cogs.hpp
//  phd-installation
//
//  Created by Phil Gough on 16/04/2016.
//
//

#ifndef kinect_cogs_hpp
#define kinect_cogs_hpp

#include <stdio.h>


#include "ofMain.h"
#include "ofApp.h"
//#include "PoissonPoints.hpp"


class Cogs {
public:

    Cogs (int c_width, int c_height);

    
    void drawCogs();

    ofFbo fbo;

private:
    float inner;
    float outer;

    vector<float> variableVar;

    vector<class PoissonPoints> cogPP;
};

#endif /* kinect_cogs_hpp */
