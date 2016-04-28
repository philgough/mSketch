#include "ofApp.h"


////--------------------------------------------------------------
//
//
//ofImage imgRiver;

//ofMesh mesh;

void ofApp::setup(){
//    ofBackground(0, 0, 0);
    cout << "starting application" << endl;

    // river/benthic layers
    ppSetup();
    
    
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
//    box2d.createBounds();
    box2d.createGround(ofPoint(0, 1080), ofPoint(1920, 1080));
}



// set up backgrounds
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
    
    
    cout << "set up land layer" << endl;
    
    topBgA.load("LandBG_1.png");
    topBgB.load("LandBg_2.png");
    

    
    
    
}




void ofApp::openniSetup() {
    openNIDevice.setup();
//    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
//    openNIDevice.addUserGenerator();
//    openNIDevice.setMaxNumUsers(2);

    
    
    openNIDevice.addHandsGenerator();
    openNIDevice.addAllHandFocusGestures();
    openNIDevice.setMaxNumHands(4);
    
    
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true,
        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false,
        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();
    
    // set properties for all user masks and point clouds
    //openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
//    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
//    openNIDevice.setUsePointCloudsAllUsers(false);
//    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
//    openNIDevice.setPointCloudResolutionAllUsers(6); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    

}




//--------------------------------------------------------------
void ofApp::update(){
    if (ofGetFrameNum() % 2 == 0) {
        openNIDevice.update();
    }
    
    // add some new circles
    if((int)ofRandom(0, 20) == 0) {
        shared_ptr<ofxBox2dCircle> c = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
        c.get()->setPhysics(0.3, 0.6, 0.012);
        c.get()->setup(box2d.getWorld(), ofRandom(20, ofGetWidth()-20), 501, 10);
        c.get()->setVelocity(ofRandom(7)-3.5, ofRandom(3)); // shoot them down!
        circles.push_back(c);
    }
    

    // remove if out of bounds

    for (int i  = 0; i < circles.size(); i++) {
        if (circles[i].get() -> getPosition().y < 500) {
            circles.at(i)->destroy();
        }
        
    }

    box2d.update();

}


//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofDrawBitmapString(ofGetFrameRate(), 20, 20);
//    imgRiver.draw(0, 0);
//    mesh.draw();
    drawRiver();
    drawBenthic();

    //
    ofSetColor(255, 255, 255);
    drawLand();
    // kinect functions
    drawHands();
 
    
    // draw particle elements
    drawBox2d();
    
    ofSetColor(255, 255, 255);
    ofFill();
    ofDrawBitmapString(ofGetFrameRate(), 20, 20);
    
    
    string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
    ofDrawBitmapString(msg, 20, 60);

}





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
    ofTranslate(0, 860);
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



void ofApp::drawLand() {
    
    sky1.draw(0, 0);
    landBG1.draw(0, 0);
    
    //    float sh3, ss3, sb3, eh3, es3, eb3;
    //    float cosValue = cos(ofDegToRad(ofGetFrameNum()/1));
    ////    cout << cosValue << endl;
    //    sh3 = ofMap(cosValue, -1, 1, sh1, sh2);
    //    ss3 = ofMap(cosValue, -1, 1, ss1, ss2);
    //    sb3 = ofMap(cosValue, -1, 1, sb1, sb2);
    //    eh3 = ofMap(cosValue, -1, 1, eh1, eh2);
    //    es3 = ofMap(cosValue, -1, 1, es1, es2);
    //    eb3 = ofMap(cosValue, -1, 1, eb1, eb2);
    //
    //    ofBackgroundGradient( ofColor::fromHsb(eh1, eh2, eh3), ofColor::fromHsb(sh3, ss3, sb3), OF_GRADIENT_LINEAR);
    //    topBuffer.end();
    //    topBuffer.draw(0, 0);
}
//
//
//void ofApp::drawSkele() {
//    
////    ofSetColor(255, 255, 255);
//    
////        ofPushMatrix();
////        // draw debug (ie., image, depth, skeleton)
////        openNIDevice.drawDebug();
////        ofPopMatrix();
//
//    ofFbo topBuffer;
//    topBuffer.allocate(1920, 1080);
//    topBuffer.begin();
//    // add a transparent background
//    ofBackground(0,0,0,0);
//    
////    sky1.draw(0, 0);
////    landBG1.draw(0, 0);
//    // draw the foreground we want to see
//
//    landFG1.draw(0, 0);
//    
//    ofPushMatrix();
//    // use a blend mode so we can see 'through' the mask(s)
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    
//    // get number of current users
//    int numUsers = openNIDevice.getNumTrackedUsers();
//    
//    // iterate through users
//    for (int i = 0; i < numUsers; i++){
//            
//        // get a reference to this user
//        ofPushMatrix();
//        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
//            
//        // draw the mask texture for this user
////        ofScale(.5, .5);
//        user.drawMask();
////        user.getCenter()
//        // you can also access the pixel and texture references individually:
//        
//        // TEXTURE REFERENCE
////        ofTexture & tex = user.getMaskTextureReference();
//        // do something with texture...
////        tex.blur();
////        tex.draw(0, 0, ofGetWidth(), 800);
//        
//        // PIXEL REFERENCE
//        //ofPixels & pix = user.getMaskPixels();
//        // do something with the pixels...
//            
//        // and point clouds:
//            
//        // move it a bit more central
//        ofTranslate(320, 120);
////        user.drawPointCloud();
//        
//        // you can also access the mesh:
//            
//        // MESH REFERENCE
////        ofMesh & mesh = user.getPointCloud();
//        // do something with the point cloud mesh
////        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
////        mesh.addVertices(cells[i].pts);
////        mesh.draw(OF_MESH_FILL);
////        openNIDevice.drawSkeleton(i);
//
//        
//        ofPopMatrix();
//            
//    }
//        
//    ofDisableBlendMode();
//    ofPopMatrix();
//    
//    topBuffer.end();
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    topBuffer.draw(0, 0);
//    ofDisableBlendMode();
//    
//    
//    // draw some info regarding frame counts etc
////    ofSetColor(0, 255, 0);
////    string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
//    
//    
//    
////    topBuffer.end();
////    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SCREEN);
////    topBuffer.draw(0, 0);
////    openNIDevice.drawDebug();
//
//    
//}


void ofApp::drawHands() {

//    ofPushMatrix();
//    openNIDevice.drawDebug();
//    ofPopMatrix();
//    cout << openNIDevice.getNumTrackedHands() << endl;
//    ofPushMatrix();
    
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    // iterate through users
    
    
    lines.clear();
    edges.clear();
    lines.push_back(ofPolyline());
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        
        // draw a rect at the position
        ofSetColor(80, 80, 80);
        ofFill();
//        ofDrawRectangle(handPosition.x, handPosition.y, 2, 2);
        // map x position of hands from the kinect dimensions to the dimensions of the app
        float x = ofMap(handPosition.x, 0, 640, -40, ofGetWidth() + 40);
        // map y position of hands from the kinect dimensions to the appropriate dimensions on the app
        float y = ofMap(handPosition.y, 0, 480, 400, 2000);
        ofDrawCircle(x, y, 8);
        lines.back().addVertex(x, y);
//        cout << i << handPosition << endl;
        // set depthThresholds based on handPosition
//        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
//        
//        // draw ROI over the depth image
//        ofSetColor(255,255,255);
//        depthThreshold.drawROI();
//        
//        // draw depth and mask textures below the depth image at 0.5 scale
//        // you could instead just do pixel maths here for finger tracking etc
//        // by using depthThreshold.getDepthPixels() and/or depthThreshold.getMaskPixels()
//        // and turn off the textures in the initial setup/addDepthTexture calls
//        
//        ofPushMatrix();
//        ofTranslate(320 * i, 480);
//        ofScale(0.5, 0.5);
//        depthThreshold.drawDepth();
//        depthThreshold.drawMask();
//        ofPopMatrix();
        
        // i think this is pretty good but might be a frame behind???
        
        
        
    }
    shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
    lines.back().simplify();
    
    for (int i=0; i<lines.back().size(); i++) {
        edge.get()->addVertex(lines.back()[i]);
    }
    
    
    edge.get()->create(box2d.getWorld());
    edges.push_back(edge);
    
    
    lines.push_back(ofPolyline());
    lines.back().addVertex(2, 0);
    lines.back().addVertex(2, 840);
    edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
    lines.back().simplify();
    
    for (int i=0; i<lines.back().size(); i++) {
        edge.get()->addVertex(lines.back()[i]);
    }
    
    
    edge.get()->create(box2d.getWorld());
    edges.push_back(edge);
    
    lines.push_back(ofPolyline());
    lines.back().addVertex(1918, 0);
    lines.back().addVertex(1918, 840);
    // add the line as an edge to the circles interact with it.
    edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
    lines.back().simplify();
    
    for (int i=0; i<lines.back().size(); i++) {
        edge.get()->addVertex(lines.back()[i]);
    }

    
    edge.get()->create(box2d.getWorld());
    edges.push_back(edge);
    
    
//    ofDisableBlendMode();
//    ofPopMatrix();
    
    

}



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

void ofApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

