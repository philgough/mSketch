//
//  character.cpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#include "character.hpp"
//#include <int>



Character::Character(string imageLoc, float tempX, float tempY, string textFileName, int characterIndex, float textxpos, float textypos, float tempTextWidth)
{
    face.load(imageLoc);
    x = tempX;
    y = tempY;
    sx = face.getWidth()*.4;
    sy = face.getHeight()*.4;
    welcome.load("welcomeA_small.png");
//    drawNow = true;
    drawPoint.set(x, y);
    timer = 0;
    welcomeString = "Hi, my name is Dr. Anthony. I am a scientist, and my job is to see the effect of pollution and natural changes on the tiny organisms that live in the top layer of soil in an esturary. Would you like to help? Wave your hand to let me know when you're ready to get started.";
    
    // text data file 
    messageList.load(textFileName, ",", "#");
    
    // in the text, get the data we want
    for (ofxCsvRow row : messageList) 
    {
//        cout << row << endl;
        if (characterIndex == stoi(row[0])) 
        {
            cout << "matched character to comment!" << endl;

//            autoTriggers.push_back(new Trigger(row));
            autoTriggers.push_back(Trigger(row));
        }
    }
    
    textBox.init("comic.ttf", 24);
    textWidth = tempTextWidth;
    textX = textxpos;
    textY = textypos;
    waveX = 320;
    waveY = 140;
}


void Character::drawCharacter(int location, int lastUI) 
{
    int numSeconds = (ofGetElapsedTimeMillis() / 1000) % 110;
    cout << numSeconds << endl;
//    cout << lastUI + noUserWaitTime << " : " << ofGetElapsedTimeMillis() << endl;
    if (lastUI + noUserWaitTime > ofGetElapsedTimeMillis())
    {
        // check the timer to see if they're saying something.
        for (int i = 0; i < autoTriggers.size(); ++i)
    	{
            
    		if (autoTriggers[i].location == location)
    		{
    			int start = timer + autoTriggers[i].wait;

    			int end = start + autoTriggers[i].duration;
                // cout << start << ": " << end <<endl;
    			if (start < numSeconds && numSeconds < end)
                {
    				drawNow(autoTriggers[i].text);
    			}
    		} 
    	}
    }
    else 
    {
        isAnyoneThere() ;
    }
}

void Character::drawNow(string text) 
{
        ofSetColor(255);
        face.draw(drawPoint, sx, sy);
        ofFill();
        // ofDrawRectangle(textX - 10 , textY - 5, textWidth + 20, textBox.getHeight() + 10);
        ofDrawRectangle(textX - 10, textY - 5, textWidth + 20, textBox.getHeight() + 30);
        ofSetColor(0);
        ofNoFill();
        ofDrawRectangle(textX - 8, textY - 3, textWidth + 16, textBox.getHeight() + 26);
        textBox.setText(text);
        textBox.wrapTextX(textWidth);
        textBox.setColor(20, 20, 20, 255);
        textBox.draw(textX, textY);
}	

void Character::updateCharacter() 
{
    
}


void Character::describeOrganism(string name, int type) 
{
    // drawNow = true;
    
    string reaction;
    switch (type) 
    {
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
    
    // textBox.setText(displayMessage);

    // textBox.wrapTextX(textWidth);
    drawNow(displayMessage);
}


void Character::isAnyoneThere()
{
    ofSetColor(255);
    ofFill();
    welcome.draw(ofGetWidth()/2, 90);
    // anthony invites to interact
    // string msg = "Hi! My name is Dr. Anthony, would you like to explore life in estuaries with me?";
    ofDrawRectangle(waveX - 10 , waveY - 5, textWidth + 20, textBox.getHeight() + 30);
    ofSetColor(0);
    ofNoFill();
    ofDrawRectangle(waveX - 5 , waveY, textWidth + 10, textBox.getHeight() + 10);
    textBox.setText(welcomeString);
    textBox.wrapTextX(textWidth);
    textBox.setColor(20, 20, 20, 255);
    textBox.draw(waveX, waveY);
    // cout << "hello, world" << endl;
    
}



