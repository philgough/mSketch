//
//  PoissonPoints.cpp
//  mySketch
//
//  Created by Phil Gough on 9/04/2016.
//
//

#include "PoissonPoints.hpp"
#include "ofMain.h"
#include "ofApp.h"

PoissonPoints::PoissonPoints(float tempMaxNumPoints, float tempMinDist, float tempTestPoints, float width, float height) {
        maxNumPoints = tempMaxNumPoints;
        minDist = tempMinDist;
        testPoints = tempTestPoints;
        
        ofVec2f firstPoint(ofRandom(width), ofRandom(height));
    
        
        SinglePoint starter(firstPoint.x, firstPoint.y, minDist, testPoints, width);
        pp.push_back(starter);
    
    
        checklist.push_back(pp.size());
        
        bool found = false;
    cout << "distributing points" << endl;
        while (activePoints() && pp.size() < maxNumPoints && checklist.size() > 0) {
            //cout << "nextLoop: " << pp.size() << " : " << checklist.size() << endl;
            
            int r = (int) abs( floor( ofRandom(checklist.size()) ) );
            int s = checklist.at(r);
            SinglePoint tpp = pp.at(s-1);
            
            if(tpp.active) {
                ofVec2f point = tpp.location;
                found = false;
                for (int i = 0; i < testPoints; i++) {
                    ofVec2f newPoint = newRandomPoint(tpp.location, minDist);
                    if (checkPoints(newPoint, width, height)) {
                        pp.push_back(SinglePoint(newPoint.x, newPoint.y, minDist, testPoints, width));
                        checklist.push_back(pp.size());
                        found = true;
                        // cout << newPoint << endl;
                        break;
                    }
                }
                if (!found) {
                    tpp.active = false;
                    checklist.erase(checklist.begin() + r);
                }
                
            }
        }
        cout << "ended Poisson Point distribution" << endl;
    cout << "distributed " << pp.size() << " points" << endl;
    
    }




    
    vector<ofVec2f> PoissonPoints::getPoints() {
        return ppLocations;
        
    }
    
bool PoissonPoints::activePoints() {
        for (int i = 0; i < pp.size (); i++) {
            SinglePoint tpp = pp.at(i);
            if (tpp.active) {
                return true;
            }
        }
        return false;
    }
    
    ofVec2f PoissonPoints::newRandomPoint (ofVec2f point, float minDist) {
        float r1 = ofRandom(1)+1;
        float r2 = ofRandom(1);
        
        float radius = minDist * r1;
        
        float angle = PI * 2 * r2;
        
        float newX = point.x + radius * cos(angle);
        float newY = point.y + radius * sin(angle);
        
        return ofVec2f(newX, newY);
    }
    
    bool PoissonPoints::findNewPoint (ofVec2f newPoint, float w, float h) {
        if (inBounds(newPoint, w, h) && !tooClose(newPoint)) {
            return true;
        }
        return false;
    }
    
    bool PoissonPoints::inBounds(ofVec2f p, float w, float h) {
        return p.x >= 0 && p.y >= 0 && p.x <= w && p.y <= h;
    }
    
    bool PoissonPoints::tooClose(ofVec2f newPoint) {
        for (int i = 0; i < pp.size(); i++) {
            SinglePoint tpp = pp.at(i);
            if (ofDist(newPoint.x, newPoint.y, tpp.location.x, tpp.location.y) < tpp.minDist) {
                return true;
            }
        }
        return false;
    }
    
    bool PoissonPoints::checkPoints (ofVec2f newPoint, float w, float h) {
        if (findNewPoint(newPoint, w, h)) {
            return true;
        }
        return false;
    }
    
    ofVec2f PoissonPoints::getPPLocation (int i) {
        return ppLocations.at(i);
    }
    
    int PoissonPoints::numLocations () {
        return ppLocations.size();
    }
