#pragma once

#include "ofMain.h"
#include "PoissonPoints.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

//		void keyPressed(int key);
//		void keyReleased(int key);
//		void mouseMoved(int x, int y );
//		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
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
    
    float widthScale;
    
    
    
};
