#include "ofApp.h"


////--------------------------------------------------------------
//
//
ofImage imgRiver;



void ofApp::setup(){
//    ofBackground(0, 0, 0);
    cout << "starting application" << endl;
    
    ppSetup();
}

//--------------------------------------------------------------
void ofApp::update(){
    updateRiver();
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
//    ofDrawBitmapString(ofGetFrameRate(), 20, 20);
    ofSetColor(255, 255, 255);
    ofFill();
    ofDrawBitmapString(ofGetFrameRate(), 20, 20);
    imgRiver.draw(0, 0);
    for (int i = 0; i < numPoints; i++) {
        ofDrawEllipse(riverLocations.at(i).x, riverLocations.at(i).y, 2, 2);
    }
//    
//    ofSetColor(255, 255, 255);
//    ofFill();
    
    for (int i = 0; i < numPoints; i++) {
        ofDrawEllipse(pointList.at(i).x, pointList.at(i).y + 500, 4, 4);
    }
    
//    ofBackground(255, 255, 255);
//    ofSetColor( 255, 255, 255 );
//    imgRiver.draw( 0, 0 );

}

////--------------------------------------------------------------
//void ofApp::keyPressed(int key){
//
//}
//
////--------------------------------------------------------------
//void ofApp::keyReleased(int key){
//
//}
//
////--------------------------------------------------------------
//
////--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button){
//
//}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    int xyToPixelArray = x + y * 1280;
    cout << "pixelList: " << voronoiPixelAssignments.at(xyToPixelArray) << ", distLIst: " << distList.at(xyToPixelArray);
    cout << ", satList: " << satList.at(xyToPixelArray) << ", hueLIst: " << hueList.at(xyToPixelArray) << endl;
    
}

////--------------------------------------------------------------
//void ofApp::mouseReleased(int x, int y, int button){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseEntered(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseExited(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::windowResized(int w, int h){
//
//}
//
////--------------------------------------------------------------
//void ofApp::gotMessage(ofMessage msg){
//
//}
//
////--------------------------------------------------------------
//void ofApp::dragEvent(ofDragInfo dragInfo){ 
//
//}
//
//
//
//
////--------------------------------------------------------------

void ofApp::ppSetup() {
    int riv_w = 1280;
    int riv_h = 220;
    PoissonPoints pp = PoissonPoints(10, 200, 40, riv_w, riv_h);
    
    widthScale = 1;
    
    numPoints = pp.pp.size();
    for (int i = 0; i < numPoints; i++) {
        pointList.push_back(pp.pp.at(i).location);
    }
    
    cout << "setup river background" << endl;
    
    
    for (int i = 0; i < numPoints; i++) {
        float mapx = ofMap(pointList.at(i).x, 0, riv_w, 0, riv_w * widthScale);
        
        ofVec2f pv;
        pv.set(mapx, pointList.at(i).y);
        
        riverLocations.push_back(pv);
    }
    
    cout << "assign pixels to closest location" << endl;
    
    
    for (int x = 0; x < riv_w; x++) {
        for (int y = 0; y < riv_h; y++) {
            int closestLocation = 0;
            float distToClosestLocation = 9001;
            for (int i = 0; i < pointList.size(); i++) {
                float distToThisLocation = ofDist(riverLocations.at(i).x, riverLocations.at(i).y, x, y);
                if (distToThisLocation < distToClosestLocation) {
                    distToClosestLocation = distToThisLocation;
                    closestLocation = i;
                }// end if
            }// end for i

            int xyToPixelArray = x + y * int(riv_w);
            // colour to draw to image
            voronoiPixelAssignments.push_back(closestLocation);
            distList.push_back(distToClosestLocation);
            hueList.push_back((closestLocation % 15)*2.5);
            satList.push_back((closestLocation % 20)*2.5);
        }
    }
    
//    cout << "pixelList size" << pixelList.size() << endl;
    
    cout << "finish pixel assignments" << endl;
}



void ofApp::updateRiver() {
    //Creating image
    int w = 1280;  //river width
    int h = 220;  //river height
    //Allocate array for filling pixels data
    unsigned char *imgRiverData = new unsigned char[w * h * 3];
    
    //Fill array for each pixel (x,y)
    float thisFrame = ofGetFrameNum()*0.015;
    // cout << thisFrame << endl0;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            // get the current pixel location
            int xyToPixelArray = x + y * w;
            ofColor c;
            // set the hue according to the closest location
            float hue = hueList.at(xyToPixelArray);
            // set the saturation according to the closest location
            float sat = satList.at(xyToPixelArray) * 3;
            // set the colour using HSB
            c.setHsb(170, 250, 100 + sat, 255);
            
            //set the rgba values to the array
            int index = 3 * ( x + y * w );
            // set red pixel colour
            imgRiverData[ index ] = c.r;
            // set green
            imgRiverData[ index + 1 ] = c.g;
            // set blue
            imgRiverData[ index + 2 ] = c.b;
            // imgRiverData[ index + 3 ] = 255;
            // float grey = 100 + hueList.at(xyToPixelArray) * 20;
            // imgRiverData[xyToPixelArray] = grey;

        }
    }

    // Load array to image
    imgRiver.setFromPixels( imgRiverData, w, h, OF_IMAGE_COLOR );
//    imgRiver.setFromPixels( imgRiverData, w, h, OF_IMAGE_GRAYSCALE );
//    imgRiver.setFromPixels(imgRiverData);
//    imgRiver.setFromPixels(const ofPixels_<unsigned char> &pixels)
    // Array is not needed anymore, so clear memory
    delete[] imgRiverData;
}




