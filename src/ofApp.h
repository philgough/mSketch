#pragma once

#include "ofMain.h"
#include "PoissonPoints.hpp"
#include "ofxVoronoi.h"
#include "organisms.hpp"

#include "ofxOpenNI.h"

#include "ofxBox2d.h"

class ofApp : public ofBaseApp{
	public:
//        ofApp();
		void setup();
		void update();
		void draw();

//		void keyPressed(int key);
//		void keyReleased(int key);
//		void mouseMoved(int x, int y );
//		void mouseDragged(int x, int y, int button);
//		void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void mouseEntered(int x, int y);
//		void mouseExited(int x, int y);
//		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);
    


    
    
//private:
    
//    PoissonPoints pp = PoissonPoints(float tempMaxNumPoints, float tempMinDist, float tempTestPoints, float width, float height);
    
    int numPoints;
    vector<ofVec2f> pointList;
    vector<ofVec2f> riverLocations;
    vector<int> voronoiPixelAssignments;
    vector<float> hueList;
    vector<float> satList;
    vector<float> distList;
    
    
    
    void ppSetup();
    void setupRiverbed();
    void updateRiver();
    void drawRiver();
    void drawBenthic();
    
    float widthScale;

    // river
    ofxVoronoi voronoiRiver;
    
    ofxVoronoi voronoiBenthic;

    vector <ofPolyline> benthicPoly;

    vector<ofPoint> riverPoints;
    bool isBorder(ofPoint _pt);
    
    vector <float> pollutionOffset;
    int riv_w = 1920;
    int riv_h = 360;
    
    
    
    
    // land layer
    ofImage topBgA;
    ofImage topBgB;
    
    void drawLand();
    
    // colours for changing background gradient on top level
//    float sh1, ss1, sb1, sh2, ss2, sb2, eh1, es1, eb1, eh2, es2, eb2;
    ofImage landBG1, landBG2;
    ofImage landFG1, landFG2;
    ofImage sky1, sky2, sky3, sky4;
    
    // kinect
    void openniSetup();
    void drawSkele();
    void drawHands();
    void userEvent(ofxOpenNIUserEvent & event);
    
    ofxOpenNI openNIDevice;


    // particles
    ofxBox2d                               box2d;   // the box2d world
    vector   <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles

    // line for the circles to interact with
    vector <ofPolyline>                    lines;
    vector <shared_ptr<ofxBox2dEdge> >      edges;

    void drawBox2d();
    
//    bool shouldRemove(ofxBox2dCircle c);
    
    
    
    
    // organisms
    void organismSetup();
    vector <Organism> organisms;
//    vector <float> organismTypes;
    
};
