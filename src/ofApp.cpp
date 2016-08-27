#include "ofApp.h"


////--------------------------------------------------------------
//
//
//void ofApp::setup(){
//    cout << "starting application" << endl;
//
//    // river/benthic layers
//    ppSetup();
//    organismSetup();
//    
//    setupImages();
//    
//
//    // kinect
//    openniSetup();
//    
//    // pollution particles
//    box2d.init();
//    box2d.setGravity(0, 3);
//    box2d.createGround(ofPoint(0, ofGetHeight()), ofPoint(1920, ofGetHeight()));
//    
////    ofSetFrameRate(1);
// 
//    starSetup();
//    
//    dejaVuSans.load("DejaVuSans.ttf", 12);
//    dejaVuSansBold.load("DejaVuSans-Bold.ttf", 28);
//    comic.load("comic.ttf", 18);
//    
//   _masterState = WELCOME_SCREEN;
//    // _masterState = INTERACTIVE_PLAY_STATE;
//    _lastState = 0;
//    _nextState = 0;
//    
//    _fadeAlpha = 0;
//    
//    _noPlayerCheck = 0;
////    _noPlayerDuration = 10000;
//    _stateTimer = 0;
//    _fadeAlpha = 0;
//    
//    welcomeBlock.init("comic.ttf", 18);
//    welcomeStringA.push_back("Hi, my name is Dr. Anthony.");
//    welcomeStringA.push_back("I am a scientist, and my job is to see the effect of pollution and natural changes on the tiny organisms that live in the top layer of soil in an esturary.");
//    welcomeStringA.push_back("Would you like to help?");
//    welcomeStringA.push_back("Wave your hand to let me know when you're ready to get started.");
//
//    timer = 0;
//    
//    setupCharacters();
//
//    
//    
//    
//    // Set up variables for the different locations
//    // pollution goes first
//    environmentVariables.push_back(ofVec2f(5, 7.3));
//    environmentVariables.push_back(ofVec2f(20, 6.8));
//    environmentVariables.push_back(ofVec2f(30, 8.0));
//    
//}
//
//
//
//
//
//
//
//
//// _____      setup the river backgound      __________ \\
//
//
//void ofApp::ppSetup() {
//
//    PoissonPoints pp = PoissonPoints(5000, 12, 40, _riv_w, _riv_h);
//    
//    widthScale = 9;
//    
//    numPoints = pp.pp.size();
//    for (int i = 0; i < numPoints; i++) {
//        pointList.push_back(pp.pp.at(i).location);
//    }
//    
//    cout << "setup river background" << endl;
//    
//    
//    
//    vector<ofPoint> riverPoints;
//    vector<ofPoint> benthicPoints;
//    
//    for (int i = 0; i < numPoints; i++) {
//        float mapx = ofMap(pointList.at(i).x, 0, _riv_w, 0, _riv_w * widthScale);
//        
//        
//        
//        ofPoint pv = ofPoint(mapx, pointList.at(i).y);
//        pv.set(mapx, pointList.at(i).y);
//        if (mapx < ofGetWidth()) {
//            riverPoints.push_back(pv);
//        }
//        
//        ofPoint bv = ofPoint(pointList.at(i).x, pointList.at(i).y);
//        benthicPoints.push_back(bv);
//        pollutionOffset.push_back(0.0);
//        
//    }
//    
//    
//    // set up the river voronoi mesh
//    ofRectangle bounds = ofRectangle(0, 0, ofGetWidth(), _riv_h);
//    
//    voronoiRiver.setBounds(bounds);
//    voronoiRiver.setPoints(riverPoints);
//    
//    voronoiRiver.generate();
//    
//    riverPoints.clear();
//    
//    
//    
//    for (unsigned int i = 0; i < voronoiRiver.getCells().size(); i++ ) {
//        auto cell = voronoiRiver.getCells().at(i);
//        riverPoints.push_back(cell.pt);
//    }
//    
//    cout << "river pionts:" << riverPoints.size() << endl;
//    
//    
//    // setup the riverbed (benthic) mesh
//    
//    voronoiBenthic.setBounds(bounds);
//    voronoiBenthic.setPoints(benthicPoints);
//    
//    voronoiBenthic.generate();
//    
//    benthicPoints.clear();
//    
//    
//    cout << "finish pp setup" << endl;
//    
//    
//    cout << "set up land layer" << endl;
//    
//    topBgA.load("LandBG_1.png");
//    topBgB.load("LandBg_2.png");
//    
//
//    
//    
//    // make a polyline for each cell
//    ofPolyline tempPoints;
//    
//    vector <ofxVoronoiCell> cells = voronoiBenthic.getCells();
//    for(int i=0; i<pointList.size(); i++) {
//        for (int j = 0; j < cells[i].pts.size(); j++) {
//            // I have no idea why you need to take 860 off x here??
//            tempPoints.addVertex(ofPoint(cells[i].pts[j].x - 860, cells[i].pts[j].y) + 860);
//        }
//        tempPoints.close();
//        benthicPoly.push_back(tempPoints);
//        tempPoints.clear();
//    }
//    
//    
//}
//
//
//
//
//
//
//
//
//// _____      setup openni __________ \\
//
//
//
//void ofApp::openniSetup() {
//    openNIDevice.setup();
//    openNIDevice.addDepthGenerator();
//    openNIDevice.setRegister(true);
//    openNIDevice.setMirror(true);
//
//    
//    
//    openNIDevice.addHandsGenerator();
//    openNIDevice.addAllHandFocusGestures();
//    openNIDevice.setMaxNumHands(6);
//    
//    
//    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
//        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
//        openNIDevice.addDepthThreshold(depthThreshold);
//    }
//    
//    openNIDevice.start();
//
//    
//
//}
//
//
//
//
//
//
//// _____      setup the organisms __________ \\
//
//
//void ofApp::organismSetup() {
//    CSVHeaders = {"env.cp", "z", "0.05", "0.95", "Phylum","Class","Order","Family","Genus","Final"};
//    
//    // random value for pH
//    phValue = ofRandom(1.6);
//    
//    displayPh = ofToString(phValue + 6.8, 1);
//    // this should be scaled to between  6.8 and 8.4
//    cout << "starting organism setup" << endl;
//    
//    
//    PoissonPoints tempPP = PoissonPoints(1000, 80, 40, _riv_w, 180);
//    cout << "number of organisms: " << tempPP.pp.size() << endl;
//    
////    loadDataset();
//    
//    for (int i = 0; i < tempPP.pp.size(); i++) {
//        ofVec2f loc = ofVec2f(tempPP.pp.at(i).location.x, tempPP.pp.at(i).location.y + 880);
//        
//        int type = i % 4;
//        
//        Organism tempO = Organism(i, loc, type, loadDataset(type, int(i/4)+1), &phValue);
//        organisms.push_back(tempO);
//    }
//
//    
//    cout << "end organism setup" << endl;
//
//}
//
//
//
//// _____      retrieve data for organisms __________ \\
//
//
//map<string, string> ofApp::loadDataset(int type, int index) {
//    
//    
//    
//    ofxCsvRow row;
//    
//    string header;
//    map <string, string> tempMap;
//    
//    switch (type) {
//            // case 0 will be negative pH reaction
//        case 0:
//            csvData.load("csvFiles/phNegZ.csv", ",");
//            
//            row = csvData.getRow(index);
//            
//            for (int i = 0; i < csvData.getNumCols(); i++) {
//                header = csvData.getRow(0).at(i);
//                tempMap[header] = row.at(i);
//            }
////            tempData.push_back(tempMap);
//            break;
//            
//            // case 1 will be positive pH reaction
//        case 1:
//            csvData.load("csvFiles/phPosZ.csv", ",");
//
//            row = csvData.getRow(index);
//            for (int i = 0; i < csvData.getNumCols(); i++) {
//                header = csvData.getRow(0).at(i);
//                tempMap[header] = row.at(i);
//            }
////            tempData.push_back(tempMap);
//            break;
//            
//            // case 2 will be negative phosphorus (pollution) reaction
//        case 2:
//            csvData.load("csvFiles/phosNegZ.csv", ",");
//            
//            row = csvData.getRow(index);
//            for (int i = 0; i < csvData.getNumCols(); i++) {
//                header = csvData.getRow(0).at(i);
//                tempMap[header] = row.at(i);
//            }
////            tempData.push_back(tempMap);
//            break;
//            
//            // case 3 will be positive phosphours (pollution) reaction
//        case 3:
//            csvData.load("csvFiles/phosPosZ.csv", ",");
//            
//            row = csvData.getRow(index);
//            for (int i = 0; i < csvData.getNumCols(); i++) {
//                header = csvData.getRow(0).at(i);
//                tempMap[header] = row.at(i);
//            }
////            tempData.push_back(tempMap);
//            break;
//            
//        default:
//            cout << "loadDataset failed?" << endl;
//            return;
//            break;
//    }
//    
//    
//    return tempMap;
//}
//
//
//
//// _____      draw stars for water quality rating __________ \\
//
//
//void ofApp::starSetup () {
//    //setup the star ratings
//    vector <ofVec3f> starPoints;
//    
//    starRadii.set(7.5, 10);
//    
//    // make a 5-sided star, i < 2*5
//    
//    for(int i = 0; i < 10; i++) {
//        float offset = starRadii.x + (starRadii.y * (i % 2));
//        
//        //        println(offset);
//        float posX = offset * sin(ofDegToRad(i*(360/(5*2))));
//        float posY = offset * cos(ofDegToRad(i*(360/(5*2))));
//        
//        starPoints.push_back(ofPoint(posX, posY, 0));
//    }
//    //    ofMesh tempStar;
//    //    tempStar.addVertices(starPoints);
//    for (int i = 0; i < 5; i++) {
//        ofMesh tempStar;
//        tempStar.addVertices(starPoints);
//        star.push_back(tempStar);
//        tempStar.clear();
//    }
//    starPoints.clear();
//    
//}
//
//
//// _____      retrieve data for organisms __________ \\
//
//void ofApp::setupImages() {
//    // land layer
//    
//    
//    bush.load("background_bush.png");
//    city.load("background_city.png");
//    farm.load("background_farm.png");
//    sky.load("Sky_1.jpg");
//    
//    // welcome page image
//    helloImage.load("welcomeA.png");
//    
//    
//}
//
//void ofApp::setupCharacters() {
//    dayTimer = 0;
//    talkingTimer = 0;
//    
//    
//    // string imageLoc, float tempX, float tempY, float scaleX, float scaleY, int tempid
//    
//    Character Anthony = Character("faceA.png", ofGetWidth() - 700, 260, "script.csv", 0, 600, 350, 600);
//    Character Kate = Character("faceK.png", 450, 300, "script.csv", 1, 600, 350, 600);
//    Character Thomas = Character("faceT.png", 450, 300, "script.csv", 2, 600, 350, 600);
//    characters.push_back(Anthony);
//    characters.push_back(Kate);
//    characters.push_back(Thomas);
////    cout << "characters size: " << characters.size();
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////--------------------------------------------------------------
//void ofApp::updateMain(){
//    
//    
//    // update the kinect position every second frame
////    if (ofGetFrameNum() % 2 == 0) {
//        openNIDevice.update();
////    }
//    // x == pollution rate
//    int numCircles = environmentVariables.at(location%3).x;
//    
//    
//    // add some new circles
//    if((int)ofRandom(0, 40) == 0 && circles.size() < numCircles) {
//        shared_ptr<ofxBox2dCircle> c = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
//        c.get()->setPhysics(0.3, 0.6, 0.012);
//        c.get()->setup(box2d.getWorld(), ofRandom(20, ofGetWidth()-20), 501, 10);
//        c.get()->setVelocity(ofRandom(7)-3.5, ofRandom(3)); // shoot them down!
//        circles.push_back(c);
//    }
//    
//    vector   <shared_ptr<ofxBox2dCircle> > tempCircles; // default box2d circles
//
//    for (int i  = 0; i < circles.size(); i++) {
//        float x =circles[i].get() -> getPosition().x;
//        float y =circles[i].get() -> getPosition().y;
//        if (y > 500 && x > -10 && x < ofGetWidth() + 10 && y < ofGetHeight()) {
//            tempCircles.push_back(circles.at(i));
//            if (y > 860) {
//                for(int j=0; j<voronoiBenthic.getPoints().size(); j++) {
//                    float x2 = benthicPoly[j].getCentroid2D().x;
//                    float y2 = benthicPoly[j].getCentroid2D().y;
//
//                    float dist = ofDist(x, y, x2, y2);
//                    if (dist < 40 && pollutionOffset.at(j) > -175) {
//                        pollutionOffset.at(j) -= 1;
//                    }
//                }
//            }
//        }
//    }
//    circles = tempCircles;
//    tempCircles.clear();
//    
//    // update the physics engine
//    box2d.update();
//    
//    
//    // update the organisms
//    for (int i = 0; i < organisms.size(); i++) {
//        // figure out which location is closest
//        float nearestDistance = 9000000001;
//        int closestCellIndex = 0;
//        
//        for (int j = 0; j < benthicPoly.size(); j++) {
//            
//            ofPoint centroid = benthicPoly.at(j).getCentroid2D();
//            float distance = ofDist(centroid.x, centroid.y, organisms.at(i).location.x, organisms.at(i).location.y);
//            if(distance < nearestDistance) {
//                nearestDistance = distance;
//                closestCellIndex = j;
//            }
//        }
//        
//        // update the organism
//        organisms[i].updateOrganism(&pollutionOffset[closestCellIndex], &phValue);
//    }
//
//    
//    updateCharacters();
//
//}
//
//
//void ofApp::updateCharacters() {
//    for (int i = 0; i < characters.size(); i++) {
//        characters.at(i).updateCharacter();
//    }
//    
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////--------------------------------------------------------------
//
//
//
//
//void ofApp::drawIntro() {
//    ofBackground(255);
//    drawHands();
//    ofSetColor(255, 255, 255);
//    helloImage.draw( ofGetWidth()/6, 50, helloImage.getWidth()/2, helloImage.getHeight()/2);
//    
//    float nextHeight = 0;
//    if (!userPresentAtIntro) {
//        for (int i = 0; i < welcomeStringA.size(); i++) {
//            //  welcomeBlock.setColor(0, 0, 0, 255);
//            welcomeBlock.setText(welcomeStringA.at(i));
//            welcomeBlock.wrapTextX(400);
//            welcomeBlock.draw(4*ofGetWidth()/8, 100 + nextHeight);
//            nextHeight += welcomeBlock.getHeight() + 12;
//        }
//        if (openNIDevice.getNumTrackedHands() > 0) {
//            if (timer == 0) timer = ofGetElapsedTimeMillis();
//            if (ofGetElapsedTimeMillis() - timer > 1000) {
//                userPresentAtIntro = true;
//                timer == 0;
//            }
//        }
//    }
//    else {
//        welcomeBlock.setText(welcomeStringB);
//        welcomeBlock.draw(ofGetWidth()/2, 100);
//        if (ofGetElapsedTimeMillis() - timer > 3000 && _masterState == WELCOME_SCREEN) {
//            nextState();
////            userPresentAtIntro = false;
//        }
//    }
//    
//    
//}
//
//
//
//
//
//
//void ofApp::drawMain() {
//
//    drawRiver();
//    drawBenthic();
//    
//    ofSetColor(255, 255, 255);
//    ofFill();
//    for (int i = 0; i < organisms.size(); i++ ) {
//        organisms.at(i).drawOrganism();
//    }
//    
//    //
//    ofSetColor(255, 255, 255);
//    drawLand();
//    
//    
//    // kinect functions
//    drawHands();
// 
//    
//    // draw particle elements
//    drawBox2d();
//    
//    
//    
//    ofSetColor(255, 255, 255);
//    
//    ofFill();
//    
//    
//    drawCharacters();
//
//    
//    
//    float averagePollution = -1.0 * accumulate(pollutionOffset.begin(), pollutionOffset.end(), 0.0) / float(pollutionOffset.size());
//
//    drawStars(averagePollution);
//    
//    dejaVuSansBold.drawString(displayPh, 230, 55);
//    dejaVuSans.drawString("Estuary pH", 220, 70);
//    
////    string msg = " Runtime: " + ofToString(ofGetElapsedTimeMillis()/1000) + "s FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate()) + " circles.size(): " + ofToString(circles.size()) + " Pollution: " + ofToString(averagePollution/175);
//    string msg = ofToString(ofGetFrameRate());
//    ofDrawBitmapString(msg, 20, 20);
//    
////    cout << "characters size: " << characters.size() << endl;
//    // if there's someone there
//    if (openNIDevice.getNumTrackedHands() > 0) {
////        // reset the timer
//        _noPlayerCheck = ofGetElapsedTimeMillis();
//    }
//    // if there's no one there
//    else {
//        // and there hasn't been for long enough
//        if (ofGetElapsedTimeMillis() > _noPlayerCheck + _noPlayerDuration) {
//            //            nextState();
//            // then go to the welcome screen.
//            cout << "nobody here, heading home" << endl;
//            int t = ofGetElapsedTimeMillis();
//            _noPlayerCheck = t;
//            _nextState = WELCOME_SCREEN;
//            _lastState = INTERACTIVE_PLAY_STATE;
//            _masterState = FADE_OUT;
//            
//            // reset the locations visited
////            location1Visited = false;
////            location2Visited = false;
//
//        }
//    }
//    
//    
////    changeLocation = false;
//    // check to see if it's time to go to the score screen
//    if (_stateTimer + _drawMainDuration < ofGetElapsedTimeMillis()) {
////        changeLocation = true;
////        if (location1Visited == false) {
////            location1Visited = true;
////        }
////        else {
////            location2Visited = true;
////        }
//        nextState();
//    }
//    
//    
//    // some feedback on the timer status
//    if (_masterState == INTERACTIVE_PLAY_STATE)
//    {
//        ofSetColor(200, 200, 10);
//        ofMesh mesh;
//        mesh.addVertex(ofPoint(0, 3));
//        mesh.addVertex(ofPoint(0, 0));
//        float x =  1.0 * (ofGetElapsedTimeMillis() - _stateTimer)/(1.0 * _drawMainDuration);
//        mesh.addVertex(ofPoint(ofGetWidth() * x, 0));
//        mesh.addVertex(ofPoint(ofGetWidth() * x, 3));
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        mesh.draw();
//        mesh.clear();
//    }
////    else
////    {
////        
////        ofSetColor(255, 0, 0);
////        ofMesh mesh;
////        mesh.addVertex(ofPoint(0, 3));
////        mesh.addVertex(ofPoint(0, 0));
////
////        mesh.addVertex(ofPoint(ofGetWidth(), 0));
////        mesh.addVertex(ofPoint(ofGetWidth(), 3));
////        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
////        mesh.draw();
////        mesh.clear();
////        
////    }
//
//
//    
//}
//
//
//
//
//
//// _____      draw the background    __________ \\
//
//
//void ofApp::drawRiver() {
//    
//    ofPushMatrix();
//    ofTranslate(0, 500);
//    ofRectangle bounds = voronoiRiver.getBounds();
//    ofSetLineWidth(0);
//    ofNoFill();
//    ofSetColor(220);
//    ofDrawRectangle(bounds);
//    
//    
//    vector <ofxVoronoiCell> cells = voronoiRiver.getCells();
//    for(int i=0; i<cells.size(); i++) {
//        float sinVar = sin(((i * 1.5) + ofGetFrameNum())*0.032);
//        float h = 140 + sinVar * 5;
//        float s = 170 + sinVar * 8;
//        float b = 255;
//        ofSetColor(ofColor::fromHsb(h, s, b));
//        ofMesh mesh;
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        mesh.addVertices(cells[i].pts);
//        mesh.draw();
//
//    }
//    ofPopMatrix();
//    
//}
//
//
//// _____      draw the riverbed    __________ \\
//
//void ofApp::drawBenthic() {
//    
//    ofPushMatrix();
//    ofTranslate(0, 860);
//
//    
//    vector <ofxVoronoiCell> cells = voronoiBenthic.getCells();
//    for(int i=0; i<cells.size(); i++) {
//        float h = 20 + sin(i) * 3;
//        float s = 200 + pollutionOffset.at(i);
//        float b = 130 + pollutionOffset.at(i) / 3;
//        ofSetColor(ofColor::fromHsb(h, s, b));
//        
//        ofMesh mesh;
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        mesh.addVertices(cells[i].pts);
//        mesh.draw();
//        
//        
//    }
//    ofPopMatrix();
//
//}
//
//
//// _____      draw the top layer    __________ \\
//
//void ofApp::drawLand() {
//    sky.draw(0, 0);
//    int loc = location % 3;
//    switch (loc) {
//        case 0:
//            bush.draw(0, 0);
//            break;
//            
//        case 1:
//            farm.draw(0, 0);
//            break;
//        case 2:
//            city.draw(0, 0);
//            break;
//        default:
//            break;
//    }
//    
//}
//
//
//// _____      draw the openni hands    __________ \\
//
//void ofApp::drawHands() {
//    ofSetColor(0, 0, 0);
//    ofFill();
//    ofDrawRectangle(1700, 20, 192, 144);
//    ofSetColor(255, 255, 255);
//    openNIDevice.drawDebug(1700, 20, 192, 144);
//    
//    lines.clear();
//    edges.clear();
//    lines.push_back(ofPolyline());
//    
//    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
//        _noPlayerCheck = 0;
//
//        // get a reference to this user
//        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
//        
//        // get hand position
//        ofPoint & handPosition = hand.getPosition();
//        
//        // draw a circle at the position
//        ofSetColor(25, 180, 95);
//        ofFill();
//        // map x position of hands from the kinect dimensions to the dimensions of the app
//        float x = ofMap(handPosition.x, 0, 640, -80, ofGetWidth() + 80);
//        // map y position of hands from the kinect dimensions to the appropriate dimensions on the app
//        float y = ofMap(handPosition.y, 0, 480, 360, 2050);
//        ofDrawCircle(x, y, 8);
//        
//        for(int j=0; j<voronoiBenthic.getPoints().size(); j++) {
//            
//            float x2 = voronoiBenthic.getPoints().at(j).x;
//            float y2 = voronoiBenthic.getPoints().at(j).y + 860;
//            
//            float dist = ofDist(x, y, x2, y2);
//            //                    cout << dist << endl;
//            if (dist < 40 && pollutionOffset.at(j) < -15) {
//                pollutionOffset.at(j) += 7;
//            }
//        }
//
////        cout << x << ", " << y << endl;
//        lines.back().addVertex(x - 4, y);
//        lines.back().addVertex(x, y + 4);
//        lines.back().addVertex(x + 4, y);
//        if (openNIDevice.getNumTrackedHands() < 2){
//            for (int k = 0; k < organisms.size(); k++) {
//                float e = ofDist(organisms.at(k).location.x, organisms.at(k).location.y, x, y);
//                if (e < 40) {
//                    organisms[k].callAnthony(&characters[0]);
//                }
//            }
//        }
//        
//    }
//    shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
//    lines.back().simplify();
//    
//    for (int i=0; i<lines.back().size(); i++) {
//        edge.get()->addVertex(lines.back()[i]);
//    }
//    
//    
//    edge.get()->create(box2d.getWorld());
//    edges.push_back(edge);
//    
//}
//
//
//// _____      draw the pollution particles    __________ \\
//
//void ofApp::drawBox2d() {
//    
//    // some circles :)
//    ofSetColor(151, 122, 93, 120);
//    for (int i=0; i<circles.size(); i++) {
//        ofFill();
////        ofSetHexColor(0xc0dd3b);
//        circles[i].get()->draw();
//    }
//    
//    ofSetHexColor(0x444342);
//    ofNoFill();
//    for (int i=0; i<lines.size(); i++) {
//        lines[i].draw();
//    }
//    for (int i=0; i<edges.size(); i++) {
//        edges[i].get()->draw();
//    }
//
//}
//
//
//
//// _____      draw the star rating    __________ \\
//
//
//void ofApp::drawStars(float averagePollution) {
//    // draw star rating
//    
//    ofSetColor(200, 200, 10);
//    
//    for (int i = 0; i < 5; i++) {
//        if (averagePollution/175 > (i/4.7)+.1) {
//            star.at(i).setMode(OF_PRIMITIVE_LINE_LOOP);
//        }
//        else {
//            star.at(i).setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        }
//        ofPushMatrix();
//        ofTranslate(40 + ((starRadii.y * 3.5) * (4-i)), 40);
//        star.at(i).draw();
//        ofPopMatrix();
//    }
//    string msg = "Water Quality Grade";
//    
//    dejaVuSans.drawString(msg, 30, 70);
//    
//}
//
//
//void ofApp::drawCharacters() {
//    for (int i = 0; i < characters.size(); i++) {
//        characters[i].drawCharacter(_stateTimer, location);
//    }
//}
//
//
//
//
//
//////--------------------------------------------------------------
////void ofApp::keyPressed(int key){
////    blendMode = key % 6;
//
////}
////
//////--------------------------------------------------------------
////void ofApp::keyReleased(int key){
////
////}
////
//////--------------------------------------------------------------
////
//////--------------------------------------------------------------
////void ofApp::mouseDragged(int x, int y, int button){
////
////}
//
////--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button)
//{
//////    int xyToPixelArray = x + y * 1280;
//////    cout << "pixelList: " << voronoiPixelAssignments.at(xyToPixelArray) << ", distLIst: " << distList.at(xyToPixelArray);
//////    cout << ", satList: " << satList.at(xyToPixelArray) << ", hueLIst: " << hueList.at(xyToPixelArray) << endl;
////    for (int i = 0; i < benthicPoly.size(); i++) {
////        if (benthicPoly.at(i).inside(x, y)){cout<< "clicked: " << i << endl;}
////    }
//    nextState();
//}
////
//////--------------------------------------------------------------
////void ofApp::mouseReleased(int x, int y, int button){
////
////}
////
//////--------------------------------------------------------------
////void ofApp::mouseEntered(int x, int y){
////
////}
////
//////--------------------------------------------------------------
////void ofApp::mouseExited(int x, int y){
////
////}
////
//////--------------------------------------------------------------
////void ofApp::windowResized(int w, int h){
////
////}
////
//////--------------------------------------------------------------
////void ofApp::gotMessage(ofMessage msg){
////
////}
////
//////--------------------------------------------------------------
////void ofApp::dragEvent(ofDragInfo dragInfo){ 
////
////}
////
////
////
////
//////--------------------------------------------------------------
////
////void ofApp::userEvent(ofxOpenNIUserEvent & event){
////    // show user event messages in the console
////    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
////}
//
//
//
//void ofApp::update()
//{
//    switch (_masterState) {
//        case WELCOME_SCREEN:
//            openNIDevice.update();
//            break;
//        case INTERACTIVE_PLAY_STATE:
//            // draw main display
//            updateMain();
//            break;
//        case SCORE_SCREEN:
//            // draw the score screen
//            openNIDevice.update();
//            break;
//        default:
//            break;
//    }
//    
//    
//}
//
//
//void ofApp::draw() {
//    drawSwitch(_masterState);
//
//}
//
//
//void ofApp::drawSwitch(int s) {
//    switch (s) {
//        case WELCOME_SCREEN:
//            // introduction
//            drawIntro();
//            break;
//            
//            
//        case INTERACTIVE_PLAY_STATE:
//            // draw main display
////            updateMain();
//            drawMain();
//            break;
//            
//        case FADE_OUT :
//            fadeOut();
//            break;
//    
//        case FADE_IN :
//            fadeIn();
//            break;
//        
//        case SCORE_SCREEN:
//            drawScoreScreen();
//            break;
//            
//        default:
//            break;
//
//    }
//}
//
//
//void ofApp::nextState() {
//    cout << "selecting the next state" << endl;
//    int t = ofGetElapsedTimeMillis();
//    _stateTimer = t;
//    switch(_masterState)
//    {
//        case WELCOME_SCREEN :
//            _lastState = WELCOME_SCREEN;
//            _nextState = INTERACTIVE_PLAY_STATE;
//            _masterState = FADE_OUT;
//            dayTimer = 0;
//            for(int i = 0; i < characters.size(); i++) {
//                characters[i].timer = t;
//            }
//            break;
//            
//        case INTERACTIVE_PLAY_STATE:
//            _nextState = SCORE_SCREEN;
//            _lastState = INTERACTIVE_PLAY_STATE;
//            _masterState = FADE_OUT;
//            
//            
//            break;
//        case FADE_OUT :
//            _masterState = FADE_IN;
//            break;
//            
//        case FADE_IN :
//            _masterState = _nextState;
//            break;
//            default:
//            
//            break;
//            
//        case SCORE_SCREEN :
//            // go to the next location;
//            location++;
//            
//            // reset the pollution
//            circles.clear();
//            for (int i = 0; i < pollutionOffset.size(); i++) {
//                pollutionOffset.at(i) = 0.0;
//            }
//            
//            if (location == 3){
//                _nextState = THANK_YOU_SCREEN;
//            }
//            else {
//                _nextState = INTERACTIVE_PLAY_STATE;
//            }
//            _lastState = SCORE_SCREEN;
//            _masterState = FADE_OUT;
//            break;
//            
//        case THANK_YOU_SCREEN :
//            // make sure we go back to the first location
//            location = 0;
//            
//            circles.clear();
//            for (int i = 0; i < pollutionOffset.size(); i++) {
//                pollutionOffset.at(i) = 0.0;
//            }
//            
//            _nextState = WELCOME_SCREEN;
//            _lastState = THANK_YOU_SCREEN;
//            _masterState = FADE_OUT;
//            
//            
//    } // end switch
//} // end nextState
//
//
//
//void ofApp::fadeOut()
//{
////    cout << "fade out" << endl;
//    
//    drawSwitch(_lastState);
////    cout << "fade out alpha: " << _fadeAlpha << endl;
//
//    if (_fadeAlpha < 254)
//    {
//        _fadeAlpha += (255 - _fadeAlpha) * 0.1;
//    } else
//    {
//        nextState();
//    } // end alpha check
//
//    // fade interactive area
//    ofSetColor(255, 255, 255, _fadeAlpha);
//    ofFill();
//    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
//} // end fadeOut
//
//
//
//void ofApp::fadeIn()
//{
////    cout << "fade in" << ofGetFrameNum() << endl;
//    
//    drawSwitch(_nextState);
//    
//    if (_fadeAlpha > 1)
//    {
////        cout << "fade in alpha: " << _fadeAlpha << endl;
//        _fadeAlpha += (0 - _fadeAlpha) * 0.1;
//        
//    } else
//    {
//        
//        timer = ofGetElapsedTimeMillis();
//        nextState();
//    } // end alpha check
//
//    ofSetColor(255, 255, 255, _fadeAlpha);
//    ofFill();
//    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
//    
//} // end fadeIn
//
//
//
//// draw the score screen
//void ofApp::drawScoreScreen()
//{
//    ofBackground(255, 255, 255);
//    // cout << "this is the score screen" << endl;
//    
//    if (_stateTimer + _scoreScreenDuration < ofGetElapsedTimeMillis())
//    {
//        nextState();
//    }
//    
//    if (_masterState == SCORE_SCREEN)
//    {
//        ofSetColor(200, 200, 10);
//        ofMesh mesh;
//        mesh.addVertex(ofPoint(0, 3));
//        mesh.addVertex(ofPoint(0, 0));
//        float x =  1.0 * (ofGetElapsedTimeMillis() - _stateTimer)/(1.0 * _scoreScreenDuration);
////        cout << x << endl;
//        mesh.addVertex(ofPoint(ofGetWidth() * x, 0));
//        mesh.addVertex(ofPoint(ofGetWidth() * x, 3));
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        mesh.draw();
//        mesh.clear();
//    }
//    else
//    {
//        ofSetColor(200, 200, 10);
//        ofMesh mesh;
//        mesh.addVertex(ofPoint(0, 3));
//        mesh.addVertex(ofPoint(0, 0));
//        //        float x = ofGetWidth() * (ofGetElapsedTimeMillis()/(_stateTimer + _scoreScreenDuration));
//        //        cout << x << endl;
//        mesh.addVertex(ofPoint(ofGetWidth(), 0));
//        mesh.addVertex(ofPoint(ofGetWidth(), 3));
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        mesh.draw();
//        mesh.clear();
//        
//    }
//}
//
//void ofApp::drawByeScreen () {
//    if (_stateTimer + _scoreScreenDuration < ofGetElapsedTimeMillis())
//    {
//        nextState();
//    }
//}
//













//// _____      setup functions    __________ \\



void ofApp::setup()
{
    variableSetup();
    setupPP();
    setupImages();
    setupBox2d();
    setupStars();
    setupOpenni();
    setupBug();
    setupWelcomeScreen();
}


void ofApp::variableSetup() 
{
    _masterState = WELCOME_SCREEN;
}



void ofApp::setupPP()
{
    // Poisson distribution for the river
    PoissonPoints pp = PoissonPoints(5000, 12, 40, _riv_w, _riv_h);

    // scale the distribution to make it appear more watery
    widthScale = 7;

    // push the points from the class to a list...
    numPoints = pp.pp.size();
    for (int i = 0; i < numPoints; i++) {
       pointList.push_back(pp.pp.at(i).location);
    }

    cout << "setup river background" << endl;

    // list of points to hold the points of the river
    vector<ofPoint> riverPoints;
    // for each point we found
    for (int i = 0; i < numPoints; i++) {
       float mapx = ofMap(pointList.at(i).x, 0, _riv_w, 0, _riv_w * widthScale);
       
       
       // add the ofxPoint from our first list to the second
       ofPoint pv = ofPoint(mapx, pointList.at(i).y);
       pv.set(mapx, pointList.at(i).y);
       if (mapx < ofGetWidth()) {
           riverPoints.push_back(pv);
       }       
    }


    // set up the river voronoi mesh
    ofRectangle bounds = ofRectangle(0, 0, ofGetWidth(), _riv_h);

    voronoiRiver.setBounds(bounds);
    voronoiRiver.setPoints(riverPoints);

    voronoiRiver.generate();

    riverPoints.clear();

    // create the voronoi cells around each of the poisson points
    for (unsigned int i = 0; i < voronoiRiver.getCells().size(); i++ ) {
       auto cell = voronoiRiver.getCells().at(i);
       riverPoints.push_back(cell.pt);
    }

    cout << "river pionts:" << riverPoints.size() << endl;

    cout << "setup benthic background" << endl;


    // distribute some points for the benthic layer
    pp = PoissonPoints(5000, 12, 40, _riv_w, _benth_h);

    pointList.clear();
    numPoints = pp.pp.size();
    cout << "benthic points created: " << numPoints << endl;
    for (int i = 0; i < numPoints; i++) {
       pointList.push_back(pp.pp.at(i).location);
    }
    vector<ofPoint> benthicPoints;
    for (int i = 0; i < numPoints; i++) {
       pollutionOffset.push_back(0);
       ofPoint pv = ofPoint(pointList.at(i).x, pointList.at(i).y);
       benthicPoints.push_back(pv);
    }
       
    // set up the benthic voronoi mesh
    bounds = ofRectangle(0, 0, ofGetWidth(), _benth_h);

    voronoiBenthic.setBounds(bounds);
    voronoiBenthic.setPoints(benthicPoints);

    voronoiBenthic.generate();

    benthicPoints.clear();
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

void ofApp::setupImages() 
{
   // land layer
    location = 0;
   
   bush.load("background_bush.png");
   city.load("background_city.png");
   farm.load("background_farm.png");
   sky.load("Sky_1.png");
   
   // welcome page images
   helloImage.load("welcomeA_small.png");
   introBush.load("introBush.png");
   selectionImages.push_back(introBush);
   introFarm.load("introFarm.png");
   selectionImages.push_back(introFarm);
   introCity.load("introCity.png");
   selectionImages.push_back(introCity);

}

void ofApp::setupBox2d() 
{
// pollution particles
    box2d.init();
    box2d.setGravity(0, 3);
}


void ofApp::setupStars () {
    dejaVuSans.load("DejaVuSans.ttf", 12);

    //setup the star ratings
    vector <ofVec3f> starPoints;

    starRadii.set(7.5, 10);

    // make a 5-sided star, i < 2*5

    for(int i = 0; i < 10; i++) {
       float offset = starRadii.x + (starRadii.y * (i % 2));
       
       //        println(offset);
       float posX = offset * sin(ofDegToRad(i*(360/(5*2))));
       float posY = offset * cos(ofDegToRad(i*(360/(5*2))));
       
       starPoints.push_back(ofPoint(posX, posY, 0));
    }
    //    ofMesh tempStar;
    //    tempStar.addVertices(starPoints);
    for (int i = 0; i < 5; i++) {
       ofMesh tempStar;
       tempStar.addVertices(starPoints);
       star.push_back(tempStar);
       tempStar.clear();
    }
    starPoints.clear();

}

void ofApp::setupOpenni() 
{
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);

    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);

    openNIDevice.start();
    
    openNIDevice.setUseMaskTextureAllUsers(true);

    // for (int i = 0; i < openNIDevice.getMaxNumUsers(); i++) 
    // {
    //     ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true);
    //     openNIDevice.addDepthThreshold(depthThreshold);
    // }
}


void ofApp::setupBug()
{
    numPlayers = 1;
    for (int i  = 0; i < numPlayers; i++)
    {
        bugLocations.push_back(ofVec2f(ofGetWidth()/2, ofGetHeight() - (_benth_h/2)));
        bugHealth.push_back(1);
    }
    
    
}






void ofApp::setupWelcomeScreen() 
{

    welcomeBlock.init("comic.ttf", 18);
    welcomeStringA.push_back("Hi, my name is Dr. Anthony.");
    welcomeStringA.push_back("I am a scientist, and my job is to see the effect of pollution and natural changes on the tiny organisms that live in the top layer of soil in an esturary.");
    welcomeStringA.push_back("Let's see how organisms survive in different conditions.");
    welcomeStringA.push_back("Select a locaiton to see which organisms thrive there.");
}
























//// _____      update functions    __________ \\





void ofApp::update()
{
    switch (_masterState) {
        case WELCOME_SCREEN:
            openNIDevice.update();
            updateBugTargetLocations();
            break;
        case INTERACTIVE_PLAY_STATE:
            // draw main display
            updateMain();
            break;
        case SCORE_SCREEN:
            // draw the score screen
            openNIDevice.update();
            break;
        default:
            break;
    }   
}


void ofApp::updateMain()
{
    updatePollution();
    updateOpenNi();
    updateBugTargetLocations();
    updateBug();
    updateEndgame();
}
void ofApp::updatePollution() 
{
   int numCircles = 50;
    // add some new circles

    if((int)ofRandom(0, 50 - location * 10) == 0 && circles.size() < numCircles) {
       shared_ptr<ofxBox2dCircle> c = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
       c.get()->setPhysics(0.3, 0.6, 0.012);
       c.get()->setup(box2d.getWorld(), ofRandom(20, ofGetWidth()-20), ofGetHeight() - (_benth_h + _riv_h), 10);
       c.get()->setVelocity(ofRandom(7)-3.5, ofRandom(3)); // shoot them down!
       circles.push_back(c);
    }

    vector   <shared_ptr<ofxBox2dCircle> > tempCircles; // default box2d circles

    for (int i  = 0; i < circles.size(); i++) {
       float x =circles[i].get() -> getPosition().x;
       float y =circles[i].get() -> getPosition().y;
       if (y < ofGetHeight()) {
           tempCircles.push_back(circles.at(i));
           if (y > ofGetHeight() - _benth_h) {
               for(int j=0; j<voronoiBenthic.getPoints().size(); j++) {
                   float x2 = benthicPoly[j].getCentroid2D().x;
                   float y2 = benthicPoly[j].getCentroid2D().y - (270); // this offset number is here because we adjusted the scales of the voronoi regions

                   float dist = ofDist(x, y, x2, y2);
                   if (dist < 60 && pollutionOffset.at(j) > -175) {
                       pollutionOffset.at(j) -= 15 - dist / 6;
                   }
               }
           }
       }
    }
    circles = tempCircles;
    tempCircles.clear();

    // update the physics engine
    box2d.update();
    if (ofGetFrameNum() % 5 == 0)
    {
        for (int i = 0; i < pollutionOffset.size(); i++) 
        {
            pollutionOffset[i] *= 0.99;
        }
    }
}



void ofApp::updateOpenNi() 
{
    openNIDevice.update();
}

void ofApp::updateBug()
{
    
    float numUsers = openNIDevice.getNumTrackedUsers();
    
    for (int i = 0; i < numPlayers; i++) 
    {
        if (numUsers > 0)
        {
            if (bugHealth.at(i) > 0 && bugTargetLocations.size() > 0)
            {
                float x = ofLerp(bugLocations.at(i).x, bugTargetLocations.at(i).x, .05);
                float y = ofLerp(bugLocations.at(i).y, bugTargetLocations.at(i).y, .05);
                bugLocations.at(i).set(x, y);
            }
        }
    }
    
    // update the organisms
    for (int i = 0; i < numPlayers; i++) {
        // figure out which location is closest
        float nearestDistance = 9000000001;
        int closestCellIndex = 0;

        for (int j = 0; j < benthicPoly.size(); j++) {

            ofPoint centroid = benthicPoly.at(j).getCentroid2D();
            float distance = ofDist(centroid.x, centroid.y, bugLocations.at(i).x, bugLocations.at(i).y);
            if(distance < nearestDistance) {
                nearestDistance = distance;
                closestCellIndex = j;
            }
        }

        // the bug gains a little health back
        if (bugHealth.at(i) > 0 && bugHealth.at(i) < 1 && pollutionOffset[closestCellIndex] >= -3 ) {bugHealth.at(i) *= 1.01;}
    
        // The bug looses health if it is over a polluted cell (add because the pollution value is negative)
        bugHealth.at(i) +=  pollutionOffset[closestCellIndex]/(175 * 200);
        // cout << bugHealth.at(i) << "over" << pollutionOffset[closestCellIndex] << endl;
        if (bugHealth.at(i) <= 0) {cout << "dead" << endl;}
    }
    
    
    
}


void ofApp::updateBugTargetLocations()
{
    float numUsers = openNIDevice.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++)
    {
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);

        // grab a point from each of their hands
        ofPoint ptL = user.getJoint(JOINT_LEFT_HAND).getWorldPosition();
        ofPoint ptR = user.getJoint(JOINT_RIGHT_HAND).getWorldPosition();
        // convert it to the screen position
        ofPoint drawMapL = ofPoint(ofMap(ptL.x, -820, 820, 0, ofGetWidth()),ofMap(ptL.y, -700, 820, ofGetHeight(), ofGetHeight() - _benth_h));
        ofPoint drawMapR = ofPoint(ofMap(ptR.x, -820, 820, 0, ofGetWidth()),ofMap(ptR.y, -700, 820, ofGetHeight(), ofGetHeight() - _benth_h));
        
        // draw a circle there
        // at the position half way between their hands
        float x = ofLerp(drawMapL.x, drawMapR.x, .5);
        float y = ofLerp(drawMapL.y, drawMapR.y, .5);
        

        bugTargetLocations.push_back(ofVec2f(x, y));

    }
    // cout << "number of hands to follow: "  << bugTargetLocations.size() << endl;
}




void ofApp::updateEndgame()
{
    int numDeadPlayers = 0;
    for (int i = 0; i < bugHealth.size(); i++) {
        if (bugHealth.at(i) <= 0) 
        {
            numDeadPlayers ++;
        }
    }
    if (numDeadPlayers == numPlayers) 
    {

        gameOutcome = -1;
        nextState();
    }
}































//// _____      draw functions    __________ \\



void ofApp::draw()
{
    drawSwitch(_masterState);
    ofSetColor(255, 255, 0);
    string msg = " Runtime: " + ofToString(ofGetElapsedTimeMillis()/1000) + "s FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate()) + " circles.size(): " + ofToString(circles.size());

    ofDrawBitmapString(msg, 10, 14);
}

void ofApp::drawMain()
{
    drawLand();
    drawRiver();
    drawBenthic();
    drawBox2d();
    drawStars();
    drawBug();
    drawMainTimer();
}






// _____      draw the background    __________ \\


void ofApp::drawRiver() 
{
   
   ofPushMatrix();
   ofTranslate(0, (ofGetHeight() - _benth_h) - _riv_h);
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

void ofApp::drawBenthic() 
{
   
   ofPushMatrix();
   ofTranslate(0, ofGetHeight() - _benth_h);
   
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

void ofApp::drawLand() 
{
    ofSetColor(255);
   sky.draw(0, 0);
   int loc = location % 3;
   switch (loc) {
       case 0:
           bush.draw(0, 0);
           break;
           
       case 1:
           farm.draw(0, 0);
           break;
       case 2:
           city.draw(0, 0);
           break;
       default:
           break;
   }
   
}



void ofApp::drawBox2d() 
{
//    
    // some circles :)
    ofSetColor(151, 122, 93, 120);
    for (int i=0; i<circles.size(); i++) {
       ofFill();
    //        ofSetHexColor(0xc0dd3b);
       circles[i].get()->draw();
    }

}




void ofApp::drawStars() 
{
    // draw star rating
    float averagePollution = -1.0 * accumulate(pollutionOffset.begin(), pollutionOffset.end(), 0.0) / float(pollutionOffset.size());

    ofSetColor(200, 200, 10);

    for (int i = 0; i < 5; i++) {
       if (averagePollution/175 > (i/4.7)+.1) {
           star.at(i).setMode(OF_PRIMITIVE_LINE_LOOP);
       }
       else {
           star.at(i).setMode(OF_PRIMITIVE_TRIANGLE_FAN);
       }
       ofPushMatrix();
       ofTranslate(40 + ((starRadii.y * 3.5) * (4-i)), 40);
       star.at(i).draw();
       ofPopMatrix();
    }
    string msg = "Water Quality Grade";

    dejaVuSans.drawString(msg, 30, 70);
}



void ofApp::drawBug() 
{
    
    ofSetColor(0, 0, 0);
    ofFill();
    ofDrawRectangle(1700, 20, 192, 144);
    ofSetColor(255, 255, 255);
    openNIDevice.drawDebug(1700, 20, 192, 144);
    
    
    for (int i = 0; i < openNIDevice.getNumTrackedUsers(); i++)
    {
        // ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);

        // user.drawMask();
        if (bugTargetLocations.size() > 0) {
            if (bugHealth.at(i) > 0)
            {
                ofNoFill();
                ofSetColor(60, 60, 60);
                ofDrawLine(bugTargetLocations.at(i), bugLocations.at(i));
            }
            ofFill();
            ofSetColor(255, i * 255, 0);
            ofDrawCircle(bugTargetLocations.at(i), 5);
        }
    }
    

    ofSetColor(0, 0, 255);

    for (int i = 0; i < bugLocations.size(); i++) 
    {
        if (bugHealth.at(i) <= 0)
        {
            ofSetColor(150, 150, 150);
        }
        ofDrawCircle(bugLocations.at(i), 10);
        // draw a health bar
        ofPoint hb;
        hb.set(bugLocations.at(i).x - hb_w/2, h_off + bugLocations.at(i).y - hb_h/2);
        ofSetColor(200, 200, 10);
        ofFill();
        ofDrawRectangle(hb.x, hb.y, hb_w, hb_h);
        ofSetColor(255, 0, 0);
        ofDrawRectangle(hb.x + hb_w - hb_bo, hb.y + hb_bo, (-hb_w * (1-bugHealth.at(i))) + hb_bo*2, hb_h - (hb_bo *2));
    }

    bugTargetLocations.clear();

    ofNoFill();

    
}




void ofApp::drawMainTimer() {
 

    // draw the timer bar
    if (_masterState == INTERACTIVE_PLAY_STATE)
    {
        if (_stateTimer + _drawMainDuration < ofGetElapsedTimeMillis()) {
            nextState();
        }
        ofSetColor(200, 200, 10);
        ofMesh mesh;
        mesh.addVertex(ofPoint(0, 3));
        mesh.addVertex(ofPoint(0, 0));
        float x =  1.0 * (ofGetElapsedTimeMillis() - _stateTimer)/(100 * _scoreScreenDuration);
        // cout << x << endl;
        mesh.addVertex(ofPoint(ofGetWidth() * x, 0));
        mesh.addVertex(ofPoint(ofGetWidth() * x, 3));
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.draw();
        mesh.clear();
    }
    else
    {
        ofSetColor(200, 200, 10);
        ofMesh mesh;
        mesh.addVertex(ofPoint(0, 3));
        mesh.addVertex(ofPoint(0, 0));
        mesh.addVertex(ofPoint(ofGetWidth(), 0));
        mesh.addVertex(ofPoint(ofGetWidth(), 3));
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.draw();
        mesh.clear();
       
   }
}




void ofApp::drawSwitch(int s) {
   switch (s) {
       case WELCOME_SCREEN:
           // introduction
           drawIntro();
           break;
           
           
       case INTERACTIVE_PLAY_STATE:
           // draw main display
           updateMain();
           drawMain();
           break;
           
       case FADE_OUT :
           fadeOut();
           break;
   
       case FADE_IN :
           fadeIn();
           break;
       
       case SCORE_SCREEN:
           drawScoreScreen();
           break;
           
       default:
           break;

   }
}


void ofApp::nextState() {
   cout << "selecting the next state" << endl;
   int t = ofGetElapsedTimeMillis();
   _stateTimer = t;
   switch(_masterState)
   {
       case WELCOME_SCREEN :
           _lastState = WELCOME_SCREEN;
           _nextState = INTERACTIVE_PLAY_STATE;
           _masterState = FADE_OUT;
           dayTimer = 0;
           // for(int i = 0; i < characters.size(); i++) {
           //     characters[i].timer = t;
           // }
           break;
           
       case INTERACTIVE_PLAY_STATE:
           _nextState = SCORE_SCREEN;
           _lastState = INTERACTIVE_PLAY_STATE;
           _masterState = FADE_OUT;
           
           
           break;
       case FADE_OUT :
           _masterState = FADE_IN;
           break;
           
       case FADE_IN :
           _masterState = _nextState;
           break;
           default:
           
           break;
           
       case SCORE_SCREEN :
           // // go to the next location;
           // location++;
           
           // reset the pollution
           circles.clear();
           for (int i = 0; i < pollutionOffset.size(); i++) {
               pollutionOffset.at(i) = 0.0;
           }

           _nextState = WELCOME_SCREEN;
           _lastState = SCORE_SCREEN;
           _masterState = FADE_OUT;
           break;
           
           
   } // end switch
} // end nextState









void ofApp::fadeOut()
{
//    cout << "fade out" << endl;
   
   drawSwitch(_lastState);
//    cout << "fade out alpha: " << _fadeAlpha << endl;

   if (_fadeAlpha < 254)
   {
       _fadeAlpha += (255 - _fadeAlpha) * 0.1;
   } else
   {
       nextState();
   } // end alpha check

   // fade interactive area
   ofSetColor(255, 255, 255, _fadeAlpha);
   ofFill();
   ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
} // end fadeOut



void ofApp::fadeIn()
{
//    cout << "fade in" << ofGetFrameNum() << endl;
   
   drawSwitch(_nextState);
   
   if (_fadeAlpha > 1)
   {
//        cout << "fade in alpha: " << _fadeAlpha << endl;
       _fadeAlpha += (0 - _fadeAlpha) * 0.1;
       
   } else
   {
       
       timer = ofGetElapsedTimeMillis();
       nextState();
   } // end alpha check

   ofSetColor(255, 255, 255, _fadeAlpha);
   ofFill();
   ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
   
} // end fadeIn

void ofApp::drawIntro()
{
    ofBackground(255);
    // update the number of players
    ofSetColor(0, 0, 0);
    ofFill();
    ofDrawRectangle(1700, 20, 192, 144);
    ofSetColor(255, 255, 255);
    openNIDevice.drawDebug(1700, 20, 192, 144);
    
    
    int foundPlayers = openNIDevice.getNumTrackedUsers();


    /// add the selector
    float selectorHeight = 100;
    float selectorWidth = 1700;
    float selectorOffset = 550;

    if (foundPlayers > 0 && bugTargetLocations.size() > 0)
    {
        ofVec2f bl = bugTargetLocations.at(0);
        if (selectorOffset < bl.x && bl.x < selectorOffset + selectorWidth)
        {
            // cout << "win" << endl;
            // 3 is the number of locations we can choose from.
            for (int i = 0; i < 3; i++)
            {
                if (selectorOffset + i * selectorHeight < bl.y && bl.y < selectorOffset + i * selectorHeight + selectorHeight)
                {
                    selectedLevel = i;
                    break;
                }
                else {
                    selectedLevel = -1;
                }
            }
        }
    }

    
    // ofNoFill();

    if (selectedLevel == pSelectedLevel && selectedLevel >= 0) {
        selectionPercent += 0.005;
        if (selectionPercent >= 1) {
            location = selectedLevel;
            nextState();
        }
    }
    else {
        selectionPercent = 0;
    }

    for (int i = 0; i < 3; i ++) 
    {
        ofSetColor(255);
        selectionImages.at(i).draw(50, selectorOffset + i * selectorHeight);
        ofSetColor(255, 140);
        ofFill();
        if (i == selectedLevel) {
            ofDrawRectangle(50, selectorOffset + i * selectorHeight, selectorWidth * selectionPercent, selectorHeight);
        }

        ofNoFill();
        ofSetColor(200, 255 * i/3, 150);
        ofDrawRectangle(50, selectorOffset + i * selectorHeight, selectorWidth, selectorHeight);
    }
    for (int i = 0; i < bugTargetLocations.size(); i++)
    {
        ofFill();
        ofSetColor(255, i * 255, 0);
        ofDrawCircle(bugTargetLocations.at(i), 5);
    }
    ofNoFill();
    pSelectedLevel = selectedLevel;
    bugTargetLocations.clear();


    ofFill();
    ofSetColor(255);
    float nextHeight = 0;
    helloImage.draw(640, 80);
    for (int i = 0; i < welcomeStringA.size(); i++) {
        welcomeBlock.setText(welcomeStringA.at(i));
        welcomeBlock.wrapTextX(400);
        ofSetColor(0, 0, 0);
        welcomeBlock.draw(4*ofGetWidth()/8, 100 + nextHeight);
        nextHeight += welcomeBlock.getHeight() + 12;
    }


}

// draw the score screen
void ofApp::drawScoreScreen()
{
   // ofBackground(255, 255, 255);
   // cout << "this is the score screen" << endl;
   drawMain();
   ofSetColor(255, 255, 255, 100);
   ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    string msg;
    if (gameOutcome < 0)
    {
        msg = "The organism has died.";
    }
    
    else {
        msg = "Finished";
    }
    
    

   if (_stateTimer + _scoreScreenDuration < ofGetElapsedTimeMillis())
   {
       nextState();
   }
   
   if (_masterState == SCORE_SCREEN)
   {
       ofSetColor(200, 200, 10);
       ofMesh mesh;
       mesh.addVertex(ofPoint(0, 3));
       mesh.addVertex(ofPoint(0, 0));
       float x =  1.0 * (ofGetElapsedTimeMillis() - _stateTimer)/(1.0 * _scoreScreenDuration);
//        cout << x << endl;
       mesh.addVertex(ofPoint(ofGetWidth() * x, 0));
       mesh.addVertex(ofPoint(ofGetWidth() * x, 3));
       mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
       mesh.draw();
       mesh.clear();
   }
   else
   {
       ofSetColor(200, 200, 10);
       ofMesh mesh;
       mesh.addVertex(ofPoint(0, 3));
       mesh.addVertex(ofPoint(0, 0));
       //        float x = ofGetWidth() * (ofGetElapsedTimeMillis()/(_stateTimer + _scoreScreenDuration));
       //        cout << x << endl;
       mesh.addVertex(ofPoint(ofGetWidth(), 0));
       mesh.addVertex(ofPoint(ofGetWidth(), 3));
       mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
       mesh.draw();
       mesh.clear();
   }






}
