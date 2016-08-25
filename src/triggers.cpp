//
//  triggers.cpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#include "triggers.hpp"


Trigger::Trigger(ofxCsvRow row) {
    // type = stoi(row[0]);
    // character, location, wait, duration
    location = stoi(row[1]);
    wait = stoi(row[2]);
    duration = stoi(row[3]);
    text = row[4];
}