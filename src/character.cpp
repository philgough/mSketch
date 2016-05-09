//
//  character.cpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#include "character.hpp"




Character::Character(string imageLoc, float tempX, float tempY, string textFileName, float textxpos, float textypos, float tempTextWidth) {
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
    
    messageList.load(textFileName, ",");
    
    for (ofxCsvRow row : messageList) {
        Trigger tempTrigger = Trigger(row);
    }
    
    textBox.init("dejaVuSans.ttf", 12);
    textWidth = tempTextWidth;
    textX = textxpos;
    textY = textypos;
}


void Character::drawCharacter() {
    if (drawNow) {
        face.draw(drawPoint, sx, sy);
        ofSetColor(255);
        ofFill();
        ofDrawRectangle(textX - 10 , textY - 5, textWidth + 20, textBox.getHeight() + 10);
        ofSetColor(0);
        textBox.draw(textX, textY);
    }
}


void Character::updateCharacter() {
    drawNow = false;
    
}


void Character::describeOrganism(string name, int type) {
    drawNow = true;
    
    string reaction;
    switch (type) {
        case 0:
            reaction = ", it likes water with a lower pH";
            break;
        case 1:
            reaction = ", it enjoys water with a higher pH";
            break;
        case 2:
            reaction = ", it doesn't survive in pollution";
            break;
        case 3:
            reaction = ", it thrives in pollution";
            break;
        default:
            break;
    }
    displayMessage = "This organism is called " + name + reaction;
    
    textBox.setText(displayMessage);
    textBox.wrapTextX(textWidth);

}