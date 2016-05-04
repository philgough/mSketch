#include "ofApp.h"


////--------------------------------------------------------------


void ofApp::setup(){
    cout << "starting application" << endl;

    // river/benthic layers
    ppSetup();
    organismSetup();
    
    setupImages();
    

    // kinect
    openniSetup();
    
    // pollution particles
    box2d.init();
    box2d.setGravity(0, 3);
    box2d.createGround(ofPoint(0, ofGetHeight()), ofPoint(1920, ofGetHeight()));
    
//    ofSetFrameRate(1);
 
    starSetup();
    
    dejaVuSans.load("DejaVuSans.ttf", 12);
    dejaVuSansBold.load("DejaVuSans-Bold.ttf", 28);
    
    
    _masterState = WELCOME_SCREEN;
    _lastState = 0;
    _nextState = 0;
    
    _fadeAlpha = 0;
    
    _stateTimer = 0;
    _fadeAlpha = 0;
    
    welcomeBlock.init("DejaVuSans.ttf", 14);
    welcomeStringA.push_back("Hi, my name is Dr. Anthony.");
    welcomeStringA.push_back("I am a scientist, and my job is to see the effect of pollution and natural changes on the tiny organisms that live in the top layer of soil in an esturary.");
    welcomeStringA.push_back("Would you like to help?");
    welcomeStringA.push_back("Wave your hand to let me know when you're ready to get started.");

    timer = 0;

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
    CSVHeaders = {"env.cp", "z", "0.05", "0.95", "Phylum","Class","Order","Family","Genus","Final"};
    
    // random value for pH
    phValue = ofRandom(1.6);
    
    displayPh = ofToString(phValue + 6.8, 1);
    // this should be scaled to between  6.8 and 8.4
    cout << "starting organism setup" << endl;
    
    
    PoissonPoints tempPP = PoissonPoints(1000, 80, 40, riv_w, 180);
    cout << "number of organisms: " << tempPP.pp.size() << endl;
    
//    loadDataset();
    
    for (int i = 0; i < tempPP.pp.size(); i++) {
        ofVec2f loc = ofVec2f(tempPP.pp.at(i).location.x, tempPP.pp.at(i).location.y + 880);
        
        int type = i % 4;
        
        Organism tempO = Organism(i, loc, type, loadDataset(type, int(i/4)+1));
        organisms.push_back(tempO);
    }

    
    cout << "end organism setup" << endl;

}



// _____      retrieve data for organisms __________ \\


map<string, string> ofApp::loadDataset(int type, int index) {
    
    
    
    ofxCsvRow row;
    
    string header;
    map <string, string> tempMap;
    
    switch (type) {
            // case 0 will be negative pH reaction
        case 0:
            csvData.load("csvFiles/phNegZ.csv", ",");
            
            row = csvData.getRow(index);
            
            for (int i = 0; i < csvData.getNumCols(); i++) {
                header = csvData.getRow(0).at(i);
                tempMap[header] = row.at(i);
            }
//            tempData.push_back(tempMap);
            break;
            
            // case 1 will be positive pH reaction
        case 1:
            csvData.load("csvFiles/phPosZ.csv", ",");

            row = csvData.getRow(index);
            for (int i = 0; i < csvData.getNumCols(); i++) {
                header = csvData.getRow(0).at(i);
                tempMap[header] = row.at(i);
            }
//            tempData.push_back(tempMap);
            break;
            
            // case 2 will be negative phosphorus (pollution) reaction
        case 2:
            csvData.load("csvFiles/phosNegZ.csv", ",");
            
            row = csvData.getRow(index);
            for (int i = 0; i < csvData.getNumCols(); i++) {
                header = csvData.getRow(0).at(i);
                tempMap[header] = row.at(i);
            }
//            tempData.push_back(tempMap);
            break;
            
            // case 3 will be positive phosphours (pollution) reaction
        case 3:
            csvData.load("csvFiles/phosPosZ.csv", ",");
            
            row = csvData.getRow(index);
            for (int i = 0; i < csvData.getNumCols(); i++) {
                header = csvData.getRow(0).at(i);
                tempMap[header] = row.at(i);
            }
//            tempData.push_back(tempMap);
            break;
            
        default:
            cout << "loadDataset failed?" << endl;
            return;
            break;
    }
    
    
    return tempMap;
}



// _____      draw stars for water quality rating __________ \\


void ofApp::starSetup () {
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


// _____      retrieve data for organisms __________ \\

void ofApp::setupImages() {
    // land layer
    landBG1.load("LandBG_1.png");
    landBG2.load("LandBG_1.png");
    landFG1.load("LandFG_1.png");
    landFG2.load("LandFG_2.png");
    sky1.load("Sky_1.png");
    sky2.load("Sky_2.png");
    sky3.load("Sky_3.png");
//    
//    // faces for feedback
//    faceA.load("faceA.png");
//    aHasSomethingToSay = false;
//    faceK.load("faceK.png");
//    kHasSomethingToSay = false;
//    faceT.load("faceT.png");
//    tHasSomethingToSay = false;
    
    // welcome page image
    helloImage.load("welcomeA.png");
    
    
}

void ofApp::setupCharacters() {
    dayTimer = 0;
    talkingTimer = 0;
    
    // string imageLoc, float tempX, float tempY, float scaleX, float scaleY, int tempid
//    Anthony = Character("faceA.png", ofGetWidth() - 100, 300,.5,.5, 0);
//    Kate = Character("faceK.png", 20, 120, .5, .5, 1);
//    Thomas = Character("faceT.png", 100, 120, .5, .5, 1);
}






































//--------------------------------------------------------------
void ofApp::updateMain(){
    
    
//    phValue = ofGetFrameNum() / 500.0;
    
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
    
    
    // update the organisms
    for (int i = 0; i < organisms.size(); i++) {
        // figure out which location is closest
        float nearestDistance = 9000000001;
        int closestCellIndex = 0;
        
        for (int j = 0; j < benthicPoly.size(); j++) {
            
            ofPoint centroid = benthicPoly.at(j).getCentroid2D();
            float distance = ofDist(centroid.x, centroid.y, organisms.at(i).location.x, organisms.at(i).location.y);
            if(distance < nearestDistance) {
                nearestDistance = distance;
                closestCellIndex = j;
            }
        }
        
        // update the organism
        organisms[i].updateOrganism(&pollutionOffset[closestCellIndex], &phValue);
    }

}


void ofApp::updateCharacters() {
    
//    int t = ofGetElapsedTimeMillis();
//    
//    
//    if (true) {
//        aHasSomethingToSay = true;
//        spokenMessage = "It's nearly time to go!";
//        talkingTimer = t;
//    }
    
}



































//--------------------------------------------------------------




void ofApp::drawIntro() {
    ofBackground(255);
    drawHands();
    ofSetColor(255, 255, 255);
    helloImage.draw( ofGetWidth()/6, 50, helloImage.getWidth()/2, helloImage.getHeight()/2);
    
    float nextHeight = 0;
    if (!userPresentAtIntro) {
        for (int i = 0; i < welcomeStringA.size(); i++) {
            //  welcomeBlock.setColor(0, 0, 0, 255);
            welcomeBlock.setText(welcomeStringA.at(i));
            welcomeBlock.wrapTextX(400);
            welcomeBlock.draw(4*ofGetWidth()/8, 100 + nextHeight);
            nextHeight += welcomeBlock.getHeight() + 8;
        }
        if (openNIDevice.getNumTrackedHands() > 0) {
            if (timer == 0) timer = ofGetElapsedTimeMillis();
            if (ofGetElapsedTimeMillis() - timer > 1000) {
                userPresentAtIntro = true;
                timer == 0;
            }
        }
    }
    else {
        welcomeBlock.setText(welcomeStringB);
        welcomeBlock.draw(ofGetWidth()/2, 100);
        if (ofGetElapsedTimeMillis() - timer > 3000 && _masterState == WELCOME_SCREEN) {
            nextState();
//            userPresentAtIntro = false;
        }
    }
    
    
}






void ofApp::drawMain() {

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
    
    
    landFG1.draw(0, 0);
    
    
    
    float averagePollution = -1.0 * accumulate(pollutionOffset.begin(), pollutionOffset.end(), 0.0) / float(pollutionOffset.size());

    drawStars(averagePollution);
    
    dejaVuSansBold.drawString(displayPh, 230, 55);
    dejaVuSans.drawString("Estuary pH", 220, 70);
    
//    string msg = " Runtime: " + ofToString(ofGetElapsedTimeMillis()/1000) + "s FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate()) + " circles.size(): " + ofToString(circles.size()) + " Pollution: " + ofToString(averagePollution/175);
//    ofDrawBitmapString(msg, 20, 20);
 
//    drawCharacters();
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



// _____      draw the star rating    __________ \\


void ofApp::drawStars(float averagePollution) {
    // draw star rating
    
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


void ofApp::drawCharacters() {
//    Anthony.drawCharacter();
//    Kate.drawCharacter();
//    Thomas.drawCharacter();
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



void ofApp::update() {
    switch (_masterState) {
        case WELCOME_SCREEN:
            openNIDevice.update();
            break;
        case INTERACTIVE_PLAY_STATE:
            // draw main display
            updateMain();
            break;
            
        default:
            break;
    }
    
    
}


void ofApp::draw() {
    drawSwitch(_masterState);

}


void ofApp::drawSwitch(int s) {
    switch (s) {
        case WELCOME_SCREEN:
            // introduction
            drawIntro();
            break;
            
            
        case INTERACTIVE_PLAY_STATE:
            // draw main display
            drawMain();
            break;
            
        case FADE_OUT :
            fadeOut();
            break;
    
        case FADE_IN :
            fadeIn();
            break;
            
        default:
            break;

    }
}


void ofApp::nextState() {
    cout << "selecting the next state" << endl;
    switch(_masterState)
    {
        case WELCOME_SCREEN :
            _lastState = WELCOME_SCREEN;
            _nextState = INTERACTIVE_PLAY_STATE;
            _masterState = FADE_OUT;
            dayTimer = 0;
            break;
            
        case INTERACTIVE_PLAY_STATE:
            _nextState = WELCOME_SCREEN;
            _lastState = INTERACTIVE_PLAY_STATE;
            _masterState = FADE_OUT;
            
        case FADE_OUT :
            _masterState = FADE_IN;
            break;
            
        case FADE_IN :
            _masterState = _nextState;
            break;
            default:
            
            break;
    } // end switch
} // end nextState



void ofApp::fadeOut()
{
    cout << "fade out" << endl;
    
    drawSwitch(_lastState);
    cout << "fade out alpha: " << _fadeAlpha << endl;

    if (_fadeAlpha < 254)
    {
        _fadeAlpha += (255 - _fadeAlpha) * 0.2;
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
    cout << "fade in" << ofGetFrameNum() << endl;
    
    drawSwitch(_nextState);
    
    if (_fadeAlpha > 1)
    {
        cout << "fade in alpha: " << _fadeAlpha << endl;
        _fadeAlpha += (0 - _fadeAlpha) * 0.2;
        
    } else
    {
        
        timer = ofGetElapsedTimeMillis();
        nextState();
    } // end alpha check

    ofSetColor(255, 255, 255, _fadeAlpha);
    ofFill();
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
} // end fadeIn

