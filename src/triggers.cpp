//
//  triggers.cpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#include "triggers.hpp"


Trigger::Trigger(ofxCsvRow row) {
    type = stoi(row[0]);
    wait = stoi(row[1]);
    duration = stoi(row[3]);
    text = row[2];
}