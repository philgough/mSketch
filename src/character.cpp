//
//  character.cpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#include "character.hpp"




Character::Character(string imageLoc, float tempX, float tempY, string textLoc) {
    face.load(imageLoc);
    x = tempX;
    y = tempY;
    sx = face.getWidth()*.4;
    sy = face.getHeight()*.4;
    
    drawNow = true;
    
    drawPoint.set(x, y);
    timer = 0;
    
    // text data files
    // int trigger type, time to trigger (if applicable), string, duration
    // trigger conditions
        // 0 = timed
        // 1 = user triggered comment
    
    messageList.load(textLoc, ",");
    
    for (ofxCsvRow row : messageList) {
        Trigger tempTrigger = Trigger(row);
    }
    
    
    
    
}


void Character::drawCharacter() {
    if (drawNow) {
        face.draw(drawPoint, sx, sy);
    }
}


void Character::updateCharacter() {
    
}
