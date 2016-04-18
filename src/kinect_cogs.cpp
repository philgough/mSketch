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

    PoissonPoints cogPP = PoissonPoints(5000, 60, 40, 500, 500);
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
    for (int i = 0; i < cogLocations.size(); i++) {
        ofPushMatrix();
//        drawOneCog();
        ofSetColor(255);
        ofFill();
        ofDrawCircle(0, 0, 2);
        ofTranslate(cogLocations.at(i).x, cogLocations.at(i).y);
        ofPopMatrix();
    }
    fbo.end();
    fbo.draw(x, y);
}


void Cogs::drawOneCog () {

    // start the shape
    ofSetColor(255);
    ofFill();
    ofSetColor(ofColor::fromHsb(40, 100, 80));
    // start the outer contour
    for (int i = 0.0; i < 360; i += 10)
    {
        float ifVal = (i % 4)/1.0;
        if (ifVal < 2)
        {
            float x = inner * sin(ofDegToRad(i));
            float y = inner * cos(ofDegToRad(i));
            ofVertex( x, y );
            float u = outer * sin(ofDegToRad(i));
            float v = outer * cos(ofDegToRad(i));
            ofVertex( u, v );
        } else
        {
            float x = outer * sin(ofDegToRad(i));
            float y = outer * cos(ofDegToRad(i));
            ofVertex( x, y );
            float u = inner * sin(ofDegToRad(i));
            float v = inner * cos(ofDegToRad(i));
            ofVertex( u, v );
        }
    }
    
    // start the inner cutouts
    for (float i = 1; i < 5; i++)
    {
        ofNextContour();
        float startValue = 130 + 90 * i;
        float endValue = 50 + 90 * i;
        for (float j = startValue; j > endValue; j-= 2) {
            float x = (inner - 10) * sin(ofDegToRad(j));
            float y = (inner - 10) * cos(ofDegToRad(j));
            ofVertex(x, y);
        }
        
        for (float k = startValue; k > endValue; k -= 10)
        {
            float x = 8 * sin(ofDegToRad(k));
            float y = 8 * cos(ofDegToRad(k));
            ofVertex(x, y);
        }
            
    }
    // finish off the shape
    

}