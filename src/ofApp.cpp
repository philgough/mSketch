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
    
//    ofBackground(0);

    
    
    // frameBuffer object for kinect blobs
//     kinectCogs.push_back(Cogs(500, 500));
    
    
    // land layer
    // hsb values
//    sh1 = 204 * 255 / 360;
//    ss1 =  96 * 2.55;
//    sb1 =  59 * 2.55;
//    
//    eh1 = 210 * 255 / 360;
//    es1 =  16 * 2.55;
//    eb1 =  95 * 2.55;
//    
//    sh2 = 356 * 255 / 360;
//    ss2 = 67 * 2.55;
//    sb2 = 90 * 2.55;
//    
//    eh2 = 245 * 255 / 360;
//    es2 = 80 * 2.55;
//    eb2 = 39 * 2.55;
    landBG1.load("LandBG_1.png");
    landBG2.load("LandBG_2.png");
    sky1.load("Sky_1.png");
    sky2.load("Sky_2.png");
    sky3.load("Sky_3.png");
    

    
    openniSetup();

    
}




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
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);

    
    
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
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(false);
//    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
//    openNIDevice.setPointCloudResolutionAllUsers(6); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
    

}




//--------------------------------------------------------------
void ofApp::update(){
    if (ofGetFrameNum() % 2 == 0) {
        openNIDevice.update();
    }
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
    drawSkele();
    
    ofFill();
    ofDrawBitmapString(ofGetFrameRate(), 20, 20);

        
    
//    for (int i = 0; i < kinectCogs.size(); i++) {
//        kinectCogs.at(i).drawCogs(100, 10);
//    }
    
    
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


void ofApp::drawSkele() {
    
//    ofSetColor(255, 255, 255);
    
//        ofPushMatrix();
//        // draw debug (ie., image, depth, skeleton)
//        openNIDevice.drawDebug();
//        ofPopMatrix();
    
//    ofFbo topBuffer;
//    topBuffer.allocate(1920, 500);
//    topBuffer.begin();
//    ofBackground(0);
    
    ofPushMatrix();
//        // use a blend mode so we can see 'through' the mask(s)
//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();
    
    // iterate through users
    for (int i = 0; i < numUsers; i++){
            
        // get a reference to this user
        ofPushMatrix();
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
            
        // draw the mask texture for this user
//        ofScale(.5, .5);
//        user.drawMask();
        
        // you can also access the pixel and texture references individually:
        
        // TEXTURE REFERENCE
        ofTexture & tex = user.getMaskTextureReference();
        // do something with texture...
        tex.draw(400, 240);
        
        // PIXEL REFERENCE
        //ofPixels & pix = user.getMaskPixels();
        // do something with the pixels...
            
        // and point clouds:
            
        // move it a bit more central
        ofTranslate(320, 240);
//        user.drawPointCloud();
        
        // you can also access the mesh:
            
        // MESH REFERENCE
//        ofMesh & mesh = user.getPointCloud();
        // do something with the point cloud mesh
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        mesh.addVertices(cells[i].pts);
//        mesh.draw(OF_MESH_FILL);
        openNIDevice.drawSkeleton(i);

        
        ofPopMatrix();
            
    }
        
//    ofDisableBlendMode();
    ofPopMatrix();
    
//    topBuffer.end();
//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
//    topBuffer.draw(0, 0, 1920, 500);
//    ofDisableBlendMode();
    
    
    // draw some info regarding frame counts etc
//    ofSetColor(0, 255, 0);
//    string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
    
    
    
//    topBuffer.end();
//    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_SCREEN);
//    topBuffer.draw(0, 0);
//    openNIDevice.drawDebug();

    
}


void ofApp::drawHands() {

//    ofPushMatrix();
//    openNIDevice.drawDebug();
//    ofPopMatrix();
//    cout << openNIDevice.getNumTrackedHands() << endl;
    ofPushMatrix();
    
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    // iterate through users
    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
        
        // get a reference to this user
        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
        
        // get hand position
        ofPoint & handPosition = hand.getPosition();
        
        // draw a rect at the position
        ofSetColor(80, 80, 80);
        ofFill();
//        ofDrawRectangle(handPosition.x, handPosition.y, 2, 2);
        ofDrawCircle(ofMap(handPosition.x, 0, 640, 0, ofGetWidth()), ofMap(handPosition.y, 0, 480, 500, 860), 8);
        cout << i << handPosition << endl;
        // set depthThresholds based on handPosition
        ofxOpenNIDepthThreshold & depthThreshold = openNIDevice.getDepthThreshold(i); // we just use hand index for the depth threshold index
        
        // draw ROI over the depth image
        ofSetColor(255,255,255);
        depthThreshold.drawROI();
        
        // draw depth and mask textures below the depth image at 0.5 scale
        // you could instead just do pixel maths here for finger tracking etc
        // by using depthThreshold.getDepthPixels() and/or depthThreshold.getMaskPixels()
        // and turn off the textures in the initial setup/addDepthTexture calls
        
        ofPushMatrix();
        ofTranslate(320 * i, 480);
        ofScale(0.5, 0.5);
        depthThreshold.drawDepth();
        depthThreshold.drawMask();
        ofPopMatrix();
        
        // i think this is pretty good but might be a frame behind???
        
    }
    
    ofDisableBlendMode();
    ofPopMatrix();
    
    

}

////--------------------------------------------------------------x
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

void ofApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

