//
//  organisms.cpp
//  phd-installation
//
//  Created by Phil Gough on 30/04/2016.
//
//

#include "organisms.hpp"


Organism::Organism(int tempIndex, ofVec2f tempLocation, int tempType, map<string, string> tempData, float* phVal) {
    index = tempIndex;
    health = 1.0;
    location.set(tempLocation);
    startLocation.set(tempLocation);
    type = tempType;
    
    // draw the shape
    for (int i = 0; i < 360; i += 18) {
        float radius = 12;
        float r = ofDegToRad(i);
        
        float x = sin(r) * radius;
        float y = sin(2*r) * radius/3 * sin(r) + cos(r) * radius/4;
        
        shape.addVertex(ofVec2f(x,y));
    }
    
    // movement and rotation
    noisex = ofRandom(.3);
    noisey = ofRandom(.3);
    noiser = ofRandom(.3);
    movementArea.set(ofRandom(40), ofRandom(40));
    startAngle = ofRandom(720);
    
    data = tempData;
    
    string firstname = data["PHYLUM"];
    string lastname = data["KINGDOM"];
    if (data["CLASS"] != "") {
        firstname = lastname;
        lastname = data["CLASS"];
    }
    if (data["ORDER"] != "") {
        firstname = lastname;
        lastname = data["ORDER"];
    }
    if (data["FAMILY"] != "") {
        firstname = lastname;
        lastname = data["FAMILY"];
    }
    if (data["GENUS"] != "") {
        firstname = lastname;
        lastname = data["GENUS"];
    }
    if (data["FINAL"] != "") {
        firstname = lastname;
        lastname = data["FINAL"];
    }
    
    name = firstname + " " + lastname;
//     cout << name << endl;
    zVal = stof(data["env.cp"]);
    if (type < 2) {
        zVal = ofMap(zVal, 6.8, 8.4, 0, 1);
    }
    else {
        zVal = ofMap(zVal, 0.0, 0.3, 0.0, 1.0);
    }
    brightness = 0;

    
    
    isBeingInspected = false;
}



void Organism::drawOrganism() {
    shape.enableColors();
    shape.clearColors();
    addColours();
    ofPushMatrix();
    ofTranslate(location);
    ofRotate(startAngle + 720 * ofNoise(noiser));
    shape.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    shape.draw();
    shape.clearColors();
    for (int i=0; i<shape.getNumVertices(); i++) {
        float h = (type+1.1 + index/100.0)/9.5+0.0125*sin(ofDegToRad(7.5 + (1.0+type)  * 12.5));
        shape.addColor(ofFloatColor::fromHsb(h, 0.75, .6));
    }
    
//    if (isBeingInspected) {
////        cout << "inspecting this organism:" << index << endl;
//        for (int i=0; i<shape.getNumVertices(); i++) {
//            shape.addColor(ofFloatColor::fromHsb(0, 1, 1));
////            shape.addColor(ofFloatColor(0));
//        }
//    }
//    else {
//        for (int i=0; i<shape.getNumVertices(); i++) {
////            shape.addColor(ofFloatColor(ofGetFrameNum()%255)/25.5);
//            float h = (type+1.1 + index/100.0)/9.5+0.0125*sin(ofDegToRad(7.5 + type * 12.5));
//            shape.addColor(ofFloatColor::fromHsb(h, 0.75, .6));
//        }
//    }
    
    shape.setMode(OF_PRIMITIVE_LINE_LOOP);
    shape.draw();
    ofPopMatrix();
    isBeingInspected = false;
    
}


void Organism::addColours() {
    for (int i=0; i<shape.getNumVertices(); i++) {
        float h = (type+1.1 + index/100.02)/9.5+0.0125*sin(ofDegToRad(7.5 * i  + (1.0+type)  * 12.5 * ofGetFrameNum()/20.0));
        shape.addColor(ofFloatColor::fromHsb(h, 0.75 * health, .6));
    }
}


void Organism::updateOrganism(float* healthIndex, float *phVal) {
    
    // update health
    health = ofMap(*healthIndex, -175.0, 0.0, 0.0, 1.0);
    
    
    // update position and rotation
    location.set(startLocation.x + movementArea.x * ofNoise(noisex), startLocation.y + movementArea.y * ofNoise(noisey));
    
    

    switch (type) {
        case 0:
            // ph, negative reaction low pH
            if (zVal < *phVal) {
//                health = py(phVal, .5, zVal);
                health = 1-log(*phVal - zVal);
//                cout<< "negative pH, z < phVal, " << zVal << ":" << *phVal << ":" << health << endl;
            }
            else {
//                health = qy(*phVal, .5, zVal);
                health = 1-log(zVal - *phVal);
//                cout<< "negative pH, z > phVal, " << zVal << ":" << phVal << ":" << health << endl;
            }
            
//            health = 0;
            break;
            
            
        case 1:
            // ph, positive reaction high pH
            if (zVal < *phVal) {
//                health = 0.5 + log(zVal - *phVal);
                health = log(*phVal - zVal) * 2;

//                health = ry(phVal, .5, zVal);
//                cout<< "positive pH, z < phVal, " << zVal << " : " << *phVal << " : " << health << endl;
                
            }
            else {
//                health = sy(phVal, .5, zVal);
                health = log(zVal - *phVal) * 2;

//                health = 0.5 - log(*phVal - zVal);
//                cout<< "positive pH, z > phVal, " << zVal << " : " << *phVal << " : " << health << endl;
            }
//            health = 0;
            break;
            
//
        case 2:
            // phosphorus, negative reaction
            if (zVal < health) {
//                health = py(health, .5, zVal);
//                cout<< "negative pollution, z < health, " << zVal << " : " << health << " : " << *healthIndex << endl;
                health = 0.95 + log(health - zVal);
            }
            else {
//                health = qy(health, .5, zVal);
                health = 0.6 - log(zVal - health);
                
//                cout<< "negative pollution, z > health, " << zVal << " : " << health << " : " << *healthIndex << endl;
//                health = 0;
            }

            break;
            
        case 3:
//            cout << "case : 3, health:" << health << endl;
            // phosphorus, positive reaction
            if (zVal < health) {
//                health = 0.0;
//                health = ry(health, .5, zVal);
                health = 0.5+log(health - zVal)/3;
//                cout<< "positive pollution, z < health, " << zVal << " : " << health << endl;
            }
            else {
//                health = 0.0;
//                health = sy(health, .5, zVal);
                health = 0.5-log(zVal - health)/3;
//                cout<< "positive pollution, z > health, " << zVal <<  " : " << health << endl;
                
            }

            break;
            
            
            
        default:
            break;
    }
    if (health < 0) {health = 0;}

    noisex += 0.005 * health;
    noisey += 0.005 * health;
    noiser += 0.0005 * health;
}



void Organism::callAnthony(Character* anthony) {
    anthony->describeOrganism(name, type);
    isBeingInspected = true;
}

