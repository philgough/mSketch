//
//  triggers.hpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#ifndef triggers_hpp
#define triggers_hpp

#include <stdio.h>

#include "ofMain.h"
#include "ofxCsv.h"

class Trigger {
public:
    Trigger(ofxCsvRow row);
    int type;
    int wait;
    int duration;
    int location;
    string text;
};


#endif /* triggers_hpp */
