//
//  character.cpp
//  phd-installation
//
//  Created by Phil Gough on 4/05/2016.
//
//

#include "character.hpp"
//#include <int>



Character::Character(string imageLoc, float tempX, float tempY, string textFileName, int characterIndex, float textxpos, float textypos, float tempTextWidth) {
    face.load(imageLoc);
    x = tempX;
    y = tempY;
    sx = face.getWidth()*.4;
    sy = face.getHeight()*.4;
    
    drawNow = true;
    
    drawPoint.set(x, y);
    timer = 0;
    
    // text data file
    messageList.load(textFileName, ",", "#");
    
    // in the text, get the data we want
    for (ofxCsvRow row : messageList) {
//        cout << row << endl;
        if (characterIndex == stoi(row[0])) {
            cout << "matched character to comment!" << endl;

//            autoTriggers.push_back(new Trigger(row));
            autoTriggers.push_back(Trigger(row));
        }
    }
    
    textBox.init("dejaVuSans.ttf", 12);
    textWidth = tempTextWidth;
    textX = textxpos;
    textY = textypos;
}


void Character::drawCharacter(int timer, int location) {
	// for (std::vector<Trigger>::iterator it = autoTriggers.begin(); it != autoTriggers.end(); ++it)
	// {
	// 	if (it->location == location)
	// 	int start = timer + it->wait;
	// 	int end = timer + it->wait + it->duration;
	// 	if (start < timer < end) {
	// 		drawNow = true;
	// 		cout << it->text << endl;
	// 	}
	// }

	for (int i = 0; i < autoTriggers.size(); ++i)
	{
//		cout << autoTriggers[i].location << ": " << location  << endl;
		if (autoTriggers[i].location == location)
		{
			int start = timer + autoTriggers[i].wait * 1000;

			int end = start + autoTriggers[i].duration * 1000;
            cout << start << ": " << end <<endl;
			if (start < ofGetElapsedTimeMillis() && ofGetElapsedTimeMillis() < end) {
				drawNow = true;
			}
		} 
	}


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