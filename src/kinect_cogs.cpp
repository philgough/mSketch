//
//  kinect_cogs.cpp
//  phd-installation
//
//  Created by Phil Gough on 16/04/2016.
//
//

#include "kinect_cogs.hpp"
#include "PoissonPoints.hpp"

Cogs::Cogs (int c_width, int c_height) {
    fbo.allocate(c_width, c_height);

    inner = 60.0;
    outer = 62.0;

//    drawCogs();

    PoissonPoints cogPP = PoissonPoints(100, 60, 40, 500, 500);
//    cout << cogPP.numLocations() << endl;
    
    for (int i = 0; i < cogPP.pp.size(); i++)
    {
//        variableVar.push_back(sin(i));
        cogLocations.push_back(cogPP.pp.at(i).location);
        
//        cout << cogPP.getPPLocation(i).x << cogPP.getPPLocation(i).y << endl;
    }

}


void Cogs::drawCogs(float x, float y) {
//    cout << "drawing cogs" << endl;
    
    fbo.begin();
//    ofBackground(0, 0, 0, 0);
    
    ofBackgroundGradient(ofColor::fromHsb(165, 201, 117), ofColor::fromHsb(20, 255, 207), OF_GRADIENT_LINEAR);
    ofSeedRandom(500);
    for (int i = 0; i < cogLocations.size(); i++) {
        ofPushMatrix();
        ofTranslate(cogLocations.at(i).x, cogLocations.at(i).y);
        drawOneCog(ofRandom(0.25, 1.1));
        ofPopMatrix();
    }
    fbo.end();
    fbo.draw(x, y);
}

void Cogs::drawOneCog (float r) {

    ofSetColor(255);
    ofFill();
    vector<ofPoint> outerPoints;
    for (float i = 0.0; i < 360; i += 10.0)
        
    {
        float ifVal = (int(i) % int(4));
        if (ifVal < 2)
        {
            float x = inner * sin(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            float y = inner * cos(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            outerPoints.push_back(ofPoint( x, y ));
            float u = outer * sin(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            float v = outer * cos(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            outerPoints.push_back(ofPoint( u, v ));
        } else
        {
            float x = outer * sin(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            float y = outer * cos(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            outerPoints.push_back(ofPoint( x, y ));
            float u = inner * sin(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            float v = inner * cos(ofDegToRad(i + ofGetFrameNum()*sin(r))) * r;
            outerPoints.push_back(ofPoint( u, v ));
        }
    }
    
    // start the inner cutouts
    vector<ofPoint> innerPoints;
    for (float i = 1; i < 5; i++)
    {
//        ofNextContour();
        float startValue = 130 + 90 * i;
        float endValue = 50 + 90 * i;
        for (float j = startValue; j > endValue; j-= 2) {
            float x = (inner - 10) * sin(ofDegToRad(j + ofGetFrameNum()*sin(r))) * r;
            float y = (inner - 10) * cos(ofDegToRad(j + ofGetFrameNum()*sin(r))) * r;
            innerPoints.push_back(ofPoint(x, y));
        }
        
    }
    
    ofMesh mesh;
    ofSetColor(ofColor::fromHsb(40, 255, 200, 200));
    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    mesh.addVertices(outerPoints);
    mesh.draw();
    mesh.clear();
    mesh.addVertices(innerPoints);
    mesh.draw();
    

}