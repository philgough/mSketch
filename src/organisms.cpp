//
//  organisms.cpp
//  phd-installation
//
//  Created by Phil Gough on 30/04/2016.
//
//

#include "organisms.hpp"


Organism::Organism(int tempIndex, ofVec2f tempLocation, int tempType, map<string, string> tempData) {
    index = tempIndex;
    health = 1.0;
    location.set(tempLocation);
    startLocation.set(tempLocation);
    type = tempType;
    
    // draw the shape
    for (int i = 0; i < 360; i += 18) {
        float radius = 25;
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
    
    zVal = stof(data["env.cp"]);
    if (type < 2) {
        zVal = ofMap(zVal, 6.8, 8.4, 0, 1);
    }
    else {
        zVal = ofMap(zVal, 0.0, 0.2, 0.0, 1.0);
    }
    brightness = 0;
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
        float h = (type+1.1 + index/100.0)/9.5+0.0125*sin(ofDegToRad(7.5 * i  + (1.0+type)  * 12.5 * ofGetFrameNum()/20.0));
        shape.addColor(ofFloatColor::fromHsb(h, 0.75, .6));
    }
//    shape.disableColors();
    shape.setMode(OF_PRIMITIVE_LINE_LOOP);
    shape.draw();
    ofPopMatrix();
    
    
    
    
}


void Organism::addColours() {
    
    for (int i=0; i<shape.getNumVertices(); i++) {
//        if (type > 1) {shape.addColor(ofColor(0));}
//        else {
        float h = (type+1.1 + index/100.02)/9.5+0.0125*sin(ofDegToRad(7.5 * i  + (1.0+type)  * 12.5 * ofGetFrameNum()/20.0));
        shape.addColor(ofFloatColor::fromHsb(h, 0.75 * health, .6));
//        }
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
            
            
        case 2:
            // phosphorus, negative reaction
            if (zVal < health) {
//                health = py(health, .5, zVal);
//                cout<< "negative pollution, z < health, " << zVal << " : " << health << " : " << *healthIndex << endl;
                health = 0.95 + log(health - zVal);
//                health = 0;
            }
            else {
//                health = qy(health, .5, zVal);
                health = 0.6 - log(zVal - health);
                if (health > 1) {health = 0;}
                
//                cout<< "negative pollution, z > health, " << zVal << " : " << health << " : " << *healthIndex << endl;
//                health = 0;
            }

            break;
            
        case 3:
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
//            if (health < 0) {health = 0;}

            break;
            
            
            
        default:
            break;
    }
    if (health < 0) {health = 0;}

    noisex += 0.005 * health;
    noisey += 0.005 * health;
    noiser += 0.0005 * health;
}
//float x = 200;
//float y = 300;
//float l = 100;
//float h = 40*mouseY/height;
//float k = l*mouseX/width; // env.cp (zVal)
// i = x value


float Organism::py(float j, float h, float k) {
//    if (k == j) {cout << "derp" << endl;}
    cout << k << " : " << j << endl;
    float  ny = 0.5 - h -  logf(k - j);// - k * log(k-j);
    return ny;
}


float Organism::qy(float j, float h, float k) {
//    if (k == j) {cout << "derp" << endl;}
    float  ny =  0.5 - h + logf(j - k);// + k * log(j-k);
    return ny;
}

float Organism::ry(float j, float h, float k) {
//    if (k == j) {cout << "derp" << endl;}
    float  ny = 0.5 - h + logf(k - j);// + k * log(k-j);
//    cout << ny << endl;
    return ny;
}


float Organism::sy(float j, float h, float k) {
//    if (k == j) {cout << "derp" << endl;}
    float  ny = 0.5 - h - logf(j - k);// - k * log(j-k);
//    cout << ny << endl;
    return ny;
}







