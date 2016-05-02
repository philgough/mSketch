#include "ofApp.h"


////--------------------------------------------------------------


void ofApp::setup(){
    cout << "starting application" << endl;

    // river/benthic layers
    ppSetup();
    organismSetup();
    
    // land layer
    landBG1.load("LandBG_1.png");
    landBG2.load("LandBG_1.png");
    landFG1.load("LandFG_1.png");
    landFG2.load("LandFG_2.png");
    sky1.load("Sky_1.png");
    sky2.load("Sky_2.png");
    sky3.load("Sky_3.png");
    

    // kinect
    openniSetup();
    
    // pollution particles
    box2d.init();
    box2d.setGravity(0, 3);
    box2d.createGround(ofPoint(0, ofGetHeight()), ofPoint(1920, ofGetHeight()));
    
//    ofSetFrameRate(1);
}








// _____      setup the river backgound      __________ \\


void ofApp::ppSetup() {

    PoissonPoints pp = PoissonPoints(5000, 12, 40, riv_w, riv_h);
    
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
        pollutionOffset.push_back(0.0);
        
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
    
    
    cout << "set up land layer" << endl;
    
    topBgA.load("LandBG_1.png");
    topBgB.load("LandBg_2.png");
    

    
    
    // make a polyline for each cell
    ofPolyline tempPoints;
    
    vector <ofxVoronoiCell> cells = voronoiBenthic.getCells();
    for(int i=0; i<pointList.size(); i++) {
        for (int j = 0; j < cells[i].pts.size(); j++) {
            // I have no idea why you need to take 860 off x here??
            tempPoints.addVertex(ofPoint(cells[i].pts[j].x - 860, cells[i].pts[j].y) + 860);
        }
        tempPoints.close();
        benthicPoly.push_back(tempPoints);
        tempPoints.clear();
    }
}








// _____      setup openni __________ \\



void ofApp::openniSetup() {
    openNIDevice.setup();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);

    
    
    openNIDevice.addHandsGenerator();
    openNIDevice.addAllHandFocusGestures();
    openNIDevice.setMaxNumHands(6);
    
    
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();

    

}






// _____      setup the organisms __________ \\


void ofApp::organismSetup() {
    cout << "starting organism setup" << endl;
    PoissonPoints tempPP = PoissonPoints(1000, 80, 40, riv_w, 180);
    cout << "number of organisms: " << tempPP.pp.size() << endl;
    for (int i = 0; i < tempPP.pp.size(); i++) {
        ofVec2f loc = ofVec2f(tempPP.pp.at(i).location.x, tempPP.pp.at(i).location.y + 880);
        Organism tempO = Organism(i, loc, i % 4, "steve");
        organisms.push_back(tempO);
    }
//    for (int i = 0; i < organisms.size(); i++) {
//        organisms.at(i).drawOrganism();
//    }
    
    cout << "end organism setup" << endl;
}






















//--------------------------------------------------------------
void ofApp::update(){
    
    // update the kinect position every second frame
//    if (ofGetFrameNum() % 2 == 0) {
        openNIDevice.update();
//    }
    
    // add some new circles
    if((int)ofRandom(0, 40) == 0 && circles.size() < 25) {
        shared_ptr<ofxBox2dCircle> c = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
        c.get()->setPhysics(0.3, 0.6, 0.012);
        c.get()->setup(box2d.getWorld(), ofRandom(20, ofGetWidth()-20), 501, 10);
        c.get()->setVelocity(ofRandom(7)-3.5, ofRandom(3)); // shoot them down!
        circles.push_back(c);
    }
    
    
    vector   <shared_ptr<ofxBox2dCircle> > tempCircles; // default box2d circles

    for (int i  = 0; i < circles.size(); i++) {
        float x =circles[i].get() -> getPosition().x;
        float y =circles[i].get() -> getPosition().y;
        if (y > 500 && x > -10 && x < ofGetWidth() + 10 && y < ofGetHeight()) {
            tempCircles.push_back(circles.at(i));
            if (y > 860) {
                for(int j=0; j<voronoiBenthic.getPoints().size(); j++) {
                    float x2 = benthicPoly[j].getCentroid2D().x;
                    float y2 = benthicPoly[j].getCentroid2D().y;

                    float dist = ofDist(x, y, x2, y2);
                    if (dist < 40 && pollutionOffset.at(j) > -175) {
                        pollutionOffset.at(j) -= 1;
                    }
                }
            }
        }
    }
    circles = tempCircles;
    tempCircles.clear();
    
    // update the physics engine
    box2d.update();

}























//--------------------------------------------------------------
void ofApp::draw(){

    drawRiver();
    drawBenthic();
    
    ofSetColor(255, 255, 255);
    ofFill();
    for (int i = 0; i < organisms.size(); i++ ) {
        organisms.at(i).drawOrganism();
    }
    
    //
    ofSetColor(255, 255, 255);
    drawLand();
    
    
    // kinect functions
    drawHands();
 
    
    // draw particle elements
    drawBox2d();
    
    
    
    ofSetColor(255, 255, 255);
    ofFill();
//    ofDrawBitmapString(ofGetFrameRate(), 20, 20);
    
    
    landFG1.draw(0, 0);
    
    string msg = " Runtime: " + ofToString(ofGetElapsedTimeMillis()/1000) + "s FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate()) + " circles.size(): " + ofToString(circles.size()) + " Pollution: " + ofToString(-1.0 * accumulate(pollutionOffset.begin(), pollutionOffset.end(), 0.0) / float(pollutionOffset.size()));
    ofDrawBitmapString(msg, 20, 20);
    
}





// _____      draw the background    __________ \\


void ofApp::drawRiver() {
    
    ofPushMatrix();
    ofTranslate(0, 500);
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
        mesh.draw();

    }
    ofPopMatrix();
    
}


// _____      draw the riverbed    __________ \\

void ofApp::drawBenthic() {
    
    ofPushMatrix();
    ofTranslate(0, 860);

    
    vector <ofxVoronoiCell> cells = voronoiBenthic.getCells();
    for(int i=0; i<cells.size(); i++) {
        float h = 20 + sin(i) * 3;
        float s = 200 + pollutionOffset.at(i);
        float b = 130 + pollutionOffset.at(i) / 3;
        ofSetColor(ofColor::fromHsb(h, s, b));
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.addVertices(cells[i].pts);
        mesh.draw();
        
        
    }
    ofPopMatrix();

}


// _____      draw the top layer    __________ \\

void ofApp::drawLand() {
    
    sky1.draw(0, 0);
    landBG1.draw(0, 0);
}


// _____      draw the openni hands    __________ \\

void ofApp::drawHands() {
    ofSetColor(0, 0, 0);
    ofFill();
    ofDrawRectangle(1700, 20, 192, 144);
    ofSetColor(255, 255, 255);
    openNIDevice.drawDebug(1700, 20, 192, 144);
    
    lines.clear();
    edges.clear();
	    lines.push_back(ofPolyline());
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        
        // draw a circle at the position
        ofSetColor(25, 180, 95);
        ofFill();
        // map x position of hands from the kinect dimensions to the dimensions of the app
        float x = ofMap(handPosition.x, 0, 640, -80, ofGetWidth() + 80);
        // map y position of hands from the kinect dimensions to the appropriate dimensions on the app
        float y = ofMap(handPosition.y, 0, 480, 360, 2050);
        ofDrawCircle(x, y, 8);
        
        for(int j=0; j<voronoiBenthic.getPoints().size(); j++) {
            
            float x2 = voronoiBenthic.getPoints().at(j).x;
            float y2 = voronoiBenthic.getPoints().at(j).y + 860;
            
            float dist = ofDist(x, y, x2, y2);
            //                    cout << dist << endl;
            if (dist < 40 && pollutionOffset.at(j) < -15) {
                pollutionOffset.at(j) += 7;
            }
        }

        
        
        
//        cout << x << ", " << y << endl;
        lines.back().addVertex(x - 4, y);
        lines.back().addVertex(x, y + 4);
        lines.back().addVertex(x + 4, y);
        
    }
    shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
    lines.back().simplify();
    
    for (int i=0; i<lines.back().size(); i++) {
        edge.get()->addVertex(lines.back()[i]);
    }
    
    
    edge.get()->create(box2d.getWorld());
    edges.push_back(edge);
    
}


// _____      draw the pollution particles    __________ \\

void ofApp::drawBox2d() {
    
    // some circles :)
    ofSetColor(151, 122, 93, 120);
    for (int i=0; i<circles.size(); i++) {
        ofFill();
//        ofSetHexColor(0xc0dd3b);
        circles[i].get()->draw();
    }
    
    ofSetHexColor(0x444342);
    ofNoFill();
    for (int i=0; i<lines.size(); i++) {
        lines[i].draw();
    }
    for (int i=0; i<edges.size(); i++) {
        edges[i].get()->draw();
    }

}

////--------------------------------------------------------------
//void ofApp::keyPressed(int key){
//    blendMode = key % 6;
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
////    int xyToPixelArray = x + y * 1280;
////    cout << "pixelList: " << voronoiPixelAssignments.at(xyToPixelArray) << ", distLIst: " << distList.at(xyToPixelArray);
////    cout << ", satList: " << satList.at(xyToPixelArray) << ", hueLIst: " << hueList.at(xyToPixelArray) << endl;
//    for (int i = 0; i < benthicPoly.size(); i++) {
//        if (benthicPoly.at(i).inside(x, y)){cout<< "clicked: " << i << endl;}
//    }
//}
//
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
//
//void ofApp::userEvent(ofxOpenNIUserEvent & event){
//    // show user event messages in the console
//    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
//}

