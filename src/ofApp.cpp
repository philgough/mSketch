#include "ofApp.h"


////--------------------------------------------------------------
//
//
//ofImage imgRiver;

//ofMesh mesh;

void ofApp::setup(){
//    ofBackground(0, 0, 0);
    cout << "starting application" << endl;

    
    ppSetup();
    
    ofBackground(0);

    
    
    // frameBuffer object for kinect blobs
     kinectCogs.push_back(Cogs(500, 500));
}

//--------------------------------------------------------------
void ofApp::update(){
//    updateRiver();
}


//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofDrawBitmapString(ofGetFrameRate(), 20, 20);
//    imgRiver.draw(0, 0);
//    mesh.draw();
    drawRiver();
    drawBenthic();

    
    
    
    ofSetColor(255, 255, 255);
    ofFill();
    ofDrawBitmapString(ofGetFrameRate(), 20, 20);

    
//    kinectCogs.drawCogs();
//    kinectCogs.fbo.draw(0, 0);
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
//void ofApp::mousePressed(int x, int y, int button){
//    int xyToPixelArray = x + y * 1280;
//    cout << "pixelList: " << voronoiPixelAssignments.at(xyToPixelArray) << ", distLIst: " << distList.at(xyToPixelArray);
//    cout << ", satList: " << satList.at(xyToPixelArray) << ", hueLIst: " << hueList.at(xyToPixelArray) << endl;
//    
//}

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
    int riv_w = 1920;
    int riv_h = 360;
    PoissonPoints pp = PoissonPoints(5000, 10, 40, riv_w, riv_h);
    
    widthScale = 9;
    
    numPoints = pp.pp.size();
    for (int i = 0; i < numPoints; i++) {
        pointList.push_back(pp.pp.at(i).location);
    }
    
    cout << "setup river background" << endl;
    

    
    vector<ofPoint> riverPoints;
    vector<ofPoint> benthicPoints;
    
    for (int i = 0; i < numPoints; i++) {
        float mapx = ofMap(pointList.at(i).x, 0, riv_w, 0, riv_w * widthScale);
        

        
        ofPoint pv = ofPoint(mapx, pointList.at(i).y);
        pv.set(mapx, pointList.at(i).y);
        if (mapx < ofGetWidth()) {
            riverPoints.push_back(pv);
        }

        ofPoint bv = ofPoint(pointList.at(i).x, pointList.at(i).y);
        benthicPoints.push_back(bv);
        
        
    }
    
    
    // set up the river voronoi mesh
    ofRectangle bounds = ofRectangle(0, 0, ofGetWidth(), riv_h);
    
    voronoiRiver.setBounds(bounds);
    voronoiRiver.setPoints(riverPoints);
    
    voronoiRiver.generate();

    riverPoints.clear();
    
    
    
    for (unsigned int i = 0; i < voronoiRiver.getCells().size(); i++ ) {
        auto cell = voronoiRiver.getCells().at(i);
        riverPoints.push_back(cell.pt);
    }

    cout << "river pionts:" << riverPoints.size() << endl;
    
    
    // setup the riverbed (benthic) mesh

    voronoiBenthic.setBounds(bounds);
    voronoiBenthic.setPoints(benthicPoints);
    
    voronoiBenthic.generate();
    
    benthicPoints.clear();
    
    
    cout << "finish pp setup" << endl;
}



void ofApp::updateRiver() {

    
}


void ofApp::drawRiver() {
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight() - 720);
    ofRectangle bounds = voronoiRiver.getBounds();
    ofSetLineWidth(0);
    ofNoFill();
    ofSetColor(220);
    ofDrawRectangle(bounds);
    
    
    vector <ofxVoronoiCell> cells = voronoiRiver.getCells();
    for(int i=0; i<cells.size(); i++) {
        float sinVar = sin(((i * 1.5) + ofGetFrameNum())*0.032);
        float h = 140 + sinVar * 5;
        float s = 170 + sinVar * 8;
        float b = 255;
        ofSetColor(ofColor::fromHsb(h, s, b));
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.addVertices(cells[i].pts);
        mesh.draw(OF_MESH_FILL);
        
//        mesh.clear();
//        for(int j = 0; j < cells[i].pts.size(); j++) {
//            mesh.addVertex(cells[i].pt);
//            mesh.addVertex(cells[i].pts[j]);
//        }
//        ofSetColor(120);
//        mesh.draw();
        
//        // Draw cell points
//        ofSetColor(ofColor::fromHsb(0, 0, 255.));
//        ofFill();
//        ofDrawCircle(cells[i].pt, 2);
    }
    ofPopMatrix();
    
}

void ofApp::drawBenthic() {
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight() - 360);
    ofRectangle bounds = voronoiBenthic.getBounds();
    ofSetLineWidth(0);
    ofNoFill();
    ofSetColor(220);
    ofDrawRectangle(bounds);
    
    vector <ofxVoronoiCell> cells = voronoiBenthic.getCells();
    for(int i=0; i<cells.size(); i++) {
        float h = 20 + sin(i) * 3;
        float s = 200;
        float b = 130;
        ofSetColor(ofColor::fromHsb(h, s, b));

        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.addVertices(cells[i].pts);
        mesh.draw(OF_MESH_FILL);

//        // Draw cell points
//        ofSetColor(ofColor::fromHsb(0, 0, 255.));
//        ofFill();
//        ofDrawCircle(cells[i].pt, 2);
    }
    ofPopMatrix();
}


