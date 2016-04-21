#pragma once

#include "ofMain.h"
#include "PoissonPoints.hpp"
#include "ofxVoronoi.h"
//#include "kinect_cogs.hpp"

#include "ofxOpenNI.h"

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

    ofxVoronoi voronoiRiver;
    
    ofxVoronoi voronoiBenthic;
    
    vector<ofPoint> riverPoints;
    bool isBorder(ofPoint _pt);
    
    
    
//    vector <class Cogs> kinectCogs;// = Cogs(500, 500);
    
    
    
    
    
    // land layer
    ofImage topBgA;
    ofImage topBgB;
    
    void drawLand();
    
    // colours for changing background gradient on top level
//    float sh1, ss1, sb1, sh2, ss2, sb2, eh1, es1, eb1, eh2, es2, eb2;
    ofImage landBG1, landBG2;
    ofImage sky1, sky2, sky3, sky4;
    
    
    void openniSetup();
    void drawSkele();
    void drawHands();
    void userEvent(ofxOpenNIUserEvent & event);
    
    ofxOpenNI openNIDevice;

    
};
