//
//  character.hpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#ifndef character_hpp
#define character_hpp

#include <stdio.h>

#include "ofMain.h"
#include "triggers.hpp"
#include "ofxCsv.h"
#include "ofxTextBlock.h"

class Character {
public:
    Character(string imageLoc, float tempX, float tempY, string textFileName, int characterIndex, float textxpos, float textypos, float tempTextWidth);
    void updateCharacter();
    void drawCharacter(int location, int lastUI);
    void describeOrganism(string name, int type);
    int timer;
//
private:
    ofImage face;
    float x, y;
    ofPoint drawPoint;
    // scales
    float sx, sy;
//    int id;
    // bool drawNow;
    
    string displayMessage;
//    vector <string> messageList;
//    vector<map <int, string>> messageList;
    
    ofxTextBlock textBox;
    float textWidth, textX, textY;
    ofxCsv messageList;
    

    vector <Trigger> autoTriggers;
    vector <Trigger> otherTriggers;
    

    // draw the character saying something

    void drawNow(string text);

    bool alreadyTalking = false;
    void isAnyoneThere();
    ofImage welcome;
    int noUserWaitTime = 20;
    int waveX, waveY;
    string welcomeString;
};  


#endif /* characters_hpp */
