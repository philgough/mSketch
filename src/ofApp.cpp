#include "ofApp.h"


//// _____      setup functions    __________ \\



void ofApp::setup()
{
    variableSetup();
    setupPP();
    setupImages();
    setupBox2d();
    if (usingOpenNI) 
    {
        setupOpenni();
    }
    else {
        setupTuio();
    }
    setupOrganisms();
    setupCharacters();
}


void ofApp::variableSetup() 
{
    // _masterState = SCORE_SCREEN;
//    ofEnableSmoothing();
    // ofSetFrameRate(25);
    sidebarHeight = ofGetHeight() - (2*sidebarMargin);
    sinRate = 450;
    if (!usingOpenNI)
    {
        sinRate = sinRate * 2;
    }

}



void ofApp::setupPP()
{
    // Poisson distribution for the river
    PoissonPoints pp = PoissonPoints(5000, 12, 40, _riv_w, _riv_h);

    // scale the distribution to make it appear more watery
    widthScale = 7;

    // push the points from the class to a list...
    numPoints = pp.pp.size();
    for (int i = 0; i < numPoints; i++) 
    {
       pointList.push_back(pp.pp.at(i).location);
    }

    cout << "setup river background" << endl;

    // list of points to hold the points of the river
    vector<ofPoint> riverPoints;
    // for each point we found
    for (int i = 0; i < numPoints; i++) 
    {
       float mapx = ofMap(pointList.at(i).x, 0, _riv_w, 0, _riv_w * widthScale);
       
       
       // add the ofxPoint from our first list to the second
       ofPoint pv = ofPoint(mapx, pointList.at(i).y);
       pv.set(mapx, pointList.at(i).y);
       if (mapx < ofGetWidth()) 
       {
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
    for (int i = 0; i < voronoiRiver.getCells().size(); i++ ) 
    {
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
    for (int i = 0; i < numPoints; i++) 
    {
       pointList.push_back(pp.pp.at(i).location);
    }
    vector<ofPoint> benthicPoints;
    for (int i = 0; i < numPoints; i++) 
    {
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
    for(int i=0; i<pointList.size(); i++) 
    {
       for (int j = 0; j < cells[i].pts.size(); j++) 
       {
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
   bush.load("background_bush.png");
   city.load("background_city.png");
   farm.load("background_farm.png");
   sky.load("Sky_1.png");
   
   // welcome page images
   helloImage.load("welcomeA_small.png");
   goodbyeImage.load("goodbyeA_small.png");
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


void ofApp::setupOpenni() 
{
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.setSpeed(60);
    // openNIDevice.addUserGenerator();
    // openNIDevice.setMaxNumUsers(2);

    openNIDevice.addHandsGenerator();
    openNIDevice.addAllHandFocusGestures();

    int maxNumHands = 6;
    openNIDevice.setMaxNumHands(6);

    for(int i = 0; i < maxNumHands; i++){
       ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
       openNIDevice.addDepthThreshold(depthThreshold);
   }

    openNIDevice.start();

}

void ofApp::setupOrganisms() 
{
   CSVHeaders = {"env.cp", "z", "0.05", "0.95", "Phylum","Class","Order","Family","Genus","Final"};
   
   // random value for pH
   currentpH = ofRandom(1.6);
   
   // displayPh = ofToString(currentpH + 6.8, 1);
   // this should be scaled to between  6.8 and 8.4
   cout << "starting organism setup" << endl;
   
   
   PoissonPoints tempPP = PoissonPoints(1000, 40, 20, _riv_w, _benth_h - sidebarMargin);
   cout << "number of organisms: " << tempPP.pp.size() << endl;
   
//    loadDataset();
   
   for (int i = 0; i < tempPP.pp.size(); i++) 
   {
       ofVec2f loc = ofVec2f(tempPP.pp.at(i).location.x, tempPP.pp.at(i).location.y + (sidebarMargin/4 + ofGetHeight() - (_benth_h)));
       
       int type = i % 4;
       
       Organism tempO = Organism(i, loc, type, loadDataset(type, int(i/4)+1), &currentpH);
       organisms.push_back(tempO);
   }

   
   cout << "end organism setup" << endl;
}



map<string, string> ofApp::loadDataset(int type, int ind) 
{
   // some duplicates, so loop round and make sure we don't get out of bounds
   int index = 1 + ind % 19;
   
   ofxCsvRow row;
   
   string header;
   map <string, string> tempMap;
   
   switch (type) 
   {
           // case 0 will be negative pH reaction
       case 0:
           csvData.load("csvFiles/phNegZ.csv", ",");
           
           row = csvData.getRow(index);
           
           for (int i = 0; i < csvData.getNumCols(); i++) 
           {
               header = csvData.getRow(0).at(i);

               tempMap[header] = row.at(i);
              // cout << header << " : " << row.at(i) << endl;
           }
//            tempData.push_back(tempMap);
           break;
           
           // case 1 will be positive pH reaction
       case 1:
           csvData.load("csvFiles/phPosZ.csv", ",");

           row = csvData.getRow(index);
           for (int i = 0; i < csvData.getNumCols(); i++) 
           {
               header = csvData.getRow(0).at(i);
               tempMap[header] = row.at(i);
           }
//            tempData.push_back(tempMap);
           break;
           
           // case 2 will be negative phosphorus (pollution) reaction
       case 2:
           csvData.load("csvFiles/phosNegZ.csv", ",");
           
           row = csvData.getRow(index);
           for (int i = 0; i < csvData.getNumCols(); i++) 
           {
               header = csvData.getRow(0).at(i);
               tempMap[header] = row.at(i);
           }
//            tempData.push_back(tempMap);
           break;
           
           // case 3 will be positive phosphours (pollution) reaction
       case 3:
           csvData.load("csvFiles/phosPosZ.csv", ",");
           
           row = csvData.getRow(index);
           for (int i = 0; i < csvData.getNumCols(); i++) 
           {
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


void ofApp::setupTuio()
{
    tuioClient.start(3333);

    ofAddListener(tuioClient.cursorAdded, this, &ofApp::tuioAdded);
    ofAddListener(tuioClient.cursorRemoved, this, &ofApp::tuioRemoved);
    ofAddListener(tuioClient.cursorUpdated, this, &ofApp::tuioUpdated);
}


void ofApp::setupCharacters()
{
    Character anthony = Character("faceA.png", ofGetWidth() - 700, 260, "script.csv", 0, 600, 350, 600);
    characters.push_back(anthony);
}



















































//// _____      update functions    __________ \\





void ofApp::update()
{
    // update the hand tracking
    if (usingOpenNI)
    {
        openNIDevice.update();
    }    
    else
    {
        tuioClient.getMessage();
    }
    updateEnvironment();
    updateBox2d();
    // UpdateCharacters();
}


void ofApp::updateEnvironment() 
{
    sinP = sin(ofGetFrameNum()/sinRate) + 1;
    sinPh = sin(ofGetFrameNum()/(sinRate *.89)) + 1;
    currentPollution = phosMax * sinP;
    currentpH = phRange * sinPh;
}



void ofApp::updateBox2d() 
{
   int numCircles = 50;
    // add some new circles

    if((int)ofRandom(0, 60 - (currentPollution * 100)) < 1.3 - currentPollution) 
    {
       shared_ptr<ofxBox2dCircle> c = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
       c.get()->setPhysics(0.3, 0.6, 0.012);
       c.get()->setup(box2d.getWorld(), ofRandom(20, ofGetWidth()-20), ofGetHeight() - (_benth_h + _riv_h), 10);
       c.get()->setVelocity(ofRandom(7)-3.5, ofRandom(3)); // shoot them down!
       circles.push_back(c);
    }

    vector   <shared_ptr<ofxBox2dCircle> > tempCircles; // default box2d circles

    for (int i  = 0; i < circles.size(); i++) 
    {
       float x =circles[i].get() -> getPosition().x;
       float y =circles[i].get() -> getPosition().y;
       if (y < ofGetHeight()) 
       {
           tempCircles.push_back(circles.at(i));
           if (y > ofGetHeight() - _benth_h) 
           {
               for(int j=0; j<voronoiBenthic.getPoints().size(); j++) 
               {
                   float x2 = benthicPoly[j].getCentroid2D().x;
                   float y2 = benthicPoly[j].getCentroid2D().y - (270); // this offset number is here because we adjusted the scales of the voronoi regions

                   float dist = ofDist(x, y, x2, y2);
                   if (dist < 60 && pollutionOffset.at(j) > -175) 
                   {
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
            pollutionOffset[i] *= 0.95;
        }
    }
}


// void ofApp::racters()
// {
//     characters.start().UpdateCharacter();
// }UpdateCha





















//// _____      draw functions    __________ \\



void ofApp::draw()
{
    drawMain();

    // drawSwitch(_masterState);
//    ofSetColor(200, 200, 20);
//    string msg = " Runtime: " + ofToString(ofGetElapsedTimeMillis()/1000) + "s FPS: " + ofToString(ofGetFrameRate()) + " circles.size(): " + ofToString(circles.size());
//    ofDrawBitmapString(msg, 10, 14);


}

void ofApp::drawMain()
{
    drawLand();
    drawRiver();
    drawBenthic();
    drawBox2d();
    // drawStars();
    // drawBug();
    // drawMainTimer();
    drawOrganism();
    drawSidebars();
    drawCharacters();
    if (usingOpenNI)
    {
        drawOpenNI();
    }
    else 
    {
        drawTuio();
    }
    drawSidebarIndicators();
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
   for(int i=0; i<cells.size(); i++) 
   {
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
   for(int i=0; i<cells.size(); i++) 
   {
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
    // cout << currentPollution << endl;
    
    if (currentPollution > .1 && currentPollution < .45)
    {
        if (abs(ofGetWidth() - landShiftX) > .3)
        {
            landShiftX += 0.05 * (ofGetWidth()- landShiftX);
        }
    }
        // landShiftX = ofGetWidth();
    else if (currentPollution > .45)
    {
        // landShiftX *= .3;
        if (abs(ofGetWidth()*2 - landShiftX) > .3)
        {
            landShiftX += 0.05 * (ofGetWidth()*2 - landShiftX);
        }
    }
    else if (currentPollution < .1 && landShiftX > 0.3)
    {
        landShiftX -= 0.05 * landShiftX;
    }
    
    ofPushMatrix();
    
    ofTranslate(landShiftX, -10);
    bush.draw(0, 0);
    farm.draw(-ofGetWidth(), 0);
    city.draw(ofGetWidth() * -2, 0);
    bush.draw(ofGetWidth() * -3, 0);
    ofPopMatrix();
    
    
    
}


void ofApp::drawOrganism() 
{
    for (int i = 0; i < organisms.size(); i++) 
    {
       // cout << o.organismColor.getHue() << ": " << o.organismColor.getSaturation() << ": " << o.organismColor.getBrightness() << endl;
        // organisms.at(i).updateOrganism(&currentPollution, &currentpH);
        organisms.at(i).drawOrganism();
    }
}



void ofApp::drawSidebars() 
{   
    // cout << currentPollution << endl;
    // draw left sidebar
    ofMesh mesh;
    mesh.enableColors();
    ofColor col;
    col.set(20, 20, 200, sidebarAlphaL);
    mesh.addVertex(ofVec2f(sidebarMargin, ofGetHeight() - sidebarMargin));
    mesh.addColor(col);
    mesh.addVertex(ofVec2f(sidebarMargin + sidebarWidth, ofGetHeight() - sidebarMargin));
    mesh.addColor(col);
    
    col.set(230, 230, 255, sidebarAlphaL);
    mesh.addColor(col);
    mesh.addVertex(ofVec2f(sidebarMargin + sidebarWidth, sidebarMargin));
    mesh.addColor(col);
    mesh.addVertex(ofVec2f(sidebarMargin, sidebarMargin));
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    mesh.draw();
    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle(sidebarMargin, sidebarMargin, sidebarWidth, ofGetHeight() - (2*sidebarMargin));
    mesh.clear();
    
    // draw right sidebar
    // col.set(200, 20, 20);
    col.setHsb(53, 255, 119, sidebarAlphaR);
    mesh.addVertex(ofVec2f(ofGetWidth() - sidebarMargin, ofGetHeight() - sidebarMargin));
    mesh.addColor(col);
    mesh.addVertex(ofVec2f(ofGetWidth() - (sidebarMargin + sidebarWidth), ofGetHeight() - sidebarMargin));
    mesh.addColor(col);
    
    col.set(255, 230, 230, sidebarAlphaR);
    mesh.addColor(col);
    mesh.addVertex(ofVec2f(ofGetWidth() - (sidebarMargin + sidebarWidth), sidebarMargin));
    mesh.addColor(col);
    mesh.addVertex(ofVec2f(ofGetWidth() - sidebarMargin, sidebarMargin));
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    mesh.draw();
    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle(ofGetWidth() - (sidebarMargin + sidebarWidth), sidebarMargin, sidebarWidth, ofGetHeight() - (2*sidebarMargin));

}





void ofApp::drawOpenNI()
{

    sidebarAlphaL = 100;
    sidebarAlphaR = 100;
//     openNIDevice.drawDebug(1500, 20, 486, 144);
//    cout << openNIDevice.getNumTrackedHands() << endl;

    for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++)
    {
        lastUserInput = ofGetElapsedTimeMillis();

        ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);

        ofPoint & handPosition = hand.getPosition();
        handPosition.set(
                ofMap(handPosition.x, 0, openNIDevice.getWidth(), -150, ofGetWidth() + 150),
                ofMap(handPosition.y, 0, openNIDevice.getHeight(), -150, ofGetHeight() + 150)
                );
//        ofPoint & handPosition = hand.getWorldPosition();
        // ofSetColor(25, 180, 95);
        // ofFill();
        // ofDrawCircle(handPosition.x, handPosition.y, 10);
        drawHands(handPosition);
//        cout << handPosition << endl;
        // see if it's inside the left sidebar
        ofVec2f insidePoint, cornerPoint;
        insidePoint.set(sidebarMargin + sidebarWidth, ofGetHeight() - sidebarMargin);
        // top left
        cornerPoint.set(sidebarMargin, sidebarMargin);
        if (handPosition.x < insidePoint.x && handPosition.y < insidePoint.y)
        {
            if (cornerPoint.x < handPosition.x && cornerPoint.y < handPosition.y) 
            {

                // sinPh = 2 - ofMap(hand.getPosition().y, 0, openNIDevice.getHeight(), 0, sidebarHeight)/(ofGetHeight());
                sinPh = 2 - ofMap(handPosition.y, sidebarMargin, ofGetHeight() - sidebarMargin, 0, 2);
                // cout << "over the lhs sidebar on frame " << ofGetFrameNum() << endl;
                // sinPh = (handPosition.y - sidebarMargin)/indicatorHeight;
                // cout << sinPh << endl;
                sidebarAlphaL = 255;
                currentpH = phRange * sinPh;
                // for (int i = 0; i < organisms.size(); i++) 
                //     organisms.at(i).updateOrganism(&currentPollution, &currentpH);
                // }
            }
        }
        //
        insidePoint.set(ofGetWidth() - (sidebarMargin + sidebarWidth), sidebarMargin);
        cornerPoint.set(ofGetWidth() - sidebarMargin, ofGetHeight() - sidebarMargin);
        if (handPosition.x > insidePoint.x && handPosition.y > insidePoint.y)
        {
            if (cornerPoint.x > handPosition.x && cornerPoint.y > handPosition.y)
            {
//                cout << "over the rhs sidebar on frame" << ofGetFrameNum() << endl;
                // sinP = 0;
                // sinP = 2 - ofMap(hand.getPosition().y, 0, openNIDevice.getHeight(), 0, sidebarHeight)/(ofGetHeight());
                sinP = 2 - ofMap(handPosition.y, sidebarMargin, ofGetHeight() - sidebarMargin, 0, 2);
                // cout << "over the lhs sidebar on frame " << ofGetFrameNum() << endl;
                // sinPh = (handPosition.y - sidebarMargin)/indicatorHeight;
                // cout << sinPh << endl;
                sidebarAlphaR = 255;
                currentPollution = phosMax * sinP;
            }
        }
    }
    for (int i = 0; i < organisms.size(); i++) 
    {
        organisms.at(i).updateOrganism(&currentPollution, &currentpH);
    }
}



void ofApp::drawTuio()
{
    sidebarAlphaL = 100;
    sidebarAlphaR = 100;

    ofFill();
    ofSetColor(0);

    for (int i = 0; i < tuioLocations.size(); i++)
    {
        // ofDrawCircle(tuioLocations.at(i).x,tuioLocations.at(i).y, 20);


        ofPoint handPosition = tuioLocations.at(i);

//        ofPoint & handPosition = hand.getWorldPosition();
        // ofSetColor(25, 180, 95);
        // ofFill();
        // ofDrawCircle(handPosition.x, handPosition.y, 10);
        drawHands(handPosition);
//        cout << handPosition << endl;
        // see if it's inside the left sidebar
        ofVec2f insidePoint, cornerPoint;
        insidePoint.set(sidebarMargin + sidebarWidth, ofGetHeight() - sidebarMargin);
        // top left
        cornerPoint.set(sidebarMargin, sidebarMargin);
        if (handPosition.x < insidePoint.x && handPosition.y < insidePoint.y)
        {
            if (cornerPoint.x < handPosition.x && cornerPoint.y < handPosition.y) 
            {

                // sinPh = 2 - ofMap(hand.getPosition().y, 0, openNIDevice.getHeight(), 0, sidebarHeight)/(ofGetHeight());
                sinPh = 2 - ofMap(handPosition.y, sidebarMargin, ofGetHeight() - sidebarMargin, 0, 2);
                // cout << "over the lhs sidebar on frame " << ofGetFrameNum() << endl;
                // sinPh = (handPosition.y - sidebarMargin)/indicatorHeight;
                // cout << sinPh << endl;
                sidebarAlphaL = 255;
                currentpH = phRange * sinPh;
                // for (int i = 0; i < organisms.size(); i++) 
                //     organisms.at(i).updateOrganism(&currentPollution, &currentpH);
                // }
            }
        }
        //
        insidePoint.set(ofGetWidth() - (sidebarMargin + sidebarWidth), sidebarMargin);
        cornerPoint.set(ofGetWidth() - sidebarMargin, ofGetHeight() - sidebarMargin);
        if (handPosition.x > insidePoint.x && handPosition.y > insidePoint.y)
        {
            if (cornerPoint.x > handPosition.x && cornerPoint.y > handPosition.y)
            {
//                cout << "over the rhs sidebar on frame" << ofGetFrameNum() << endl;
                // sinP = 0;
                // sinP = 2 - ofMap(hand.getPosition().y, 0, openNIDevice.getHeight(), 0, sidebarHeight)/(ofGetHeight());
                sinP = 2 - ofMap(handPosition.y, sidebarMargin, ofGetHeight() - sidebarMargin, 0, 2);
                // cout << "over the lhs sidebar on frame " << ofGetFrameNum() << endl;
                // sinPh = (handPosition.y - sidebarMargin)/indicatorHeight;
                // cout << sinPh << endl;
                sidebarAlphaR = 255;
                currentPollution = phosMax * sinP;
            }
        }





    }
    for (int i = 0; i < organisms.size(); i++) 
    {
        organisms.at(i).updateOrganism(&currentPollution, &currentpH);
    }
}






void ofApp::drawSidebarIndicators()
{
    sidebarIndicatorPh = ofGetHeight() - (((sidebarHeight/2) - indicatorHeight) * sinPh + (indicatorHeight/2)) - (sidebarMargin*2 + indicatorHeight/2)-(indicatorHeight/2);
    sidebarIndicatorP = ofGetHeight() - (((sidebarHeight/2) - indicatorHeight) * sinP + (indicatorHeight/2)) - (sidebarMargin*2 + indicatorHeight/2)-(indicatorHeight/2);

    // cout << currentpH << endl;
    // draw LHS (pH) Indicator
    ofSetColor(255, sidebarAlphaL + 80);
    ofFill();
    ofDrawCircle(sidebarMargin + indicatorHeight, sidebarMargin + sidebarIndicatorPh + (indicatorHeight/2), indicatorHeight);
    ofSetColor(255, sidebarAlphaR + 80);
    // draw RHS (Phosphorus)
    ofDrawCircle(ofGetWidth() - (sidebarMargin + indicatorHeight), sidebarMargin + sidebarIndicatorP + (indicatorHeight/2), indicatorHeight);
    // draw text values
    ofSetColor(0, sidebarAlphaL + 80);
    float textoffset = -30;
    string msg = "pH: " + ofToString(currentpH + phMin, 2);
    ofDrawBitmapString(msg, textoffset +  sidebarMargin + indicatorHeight, sidebarMargin + sidebarIndicatorPh + (indicatorHeight/2));
    ofSetColor(0, sidebarAlphaR + 80);
    msg = "P: " + ofToString(currentPollution, 2);
    ofDrawBitmapString(msg, textoffset + ofGetWidth() - (sidebarMargin + indicatorHeight), sidebarMargin + sidebarIndicatorP + (indicatorHeight/2));
    ofSetColor(255);
    msg = "pH (acidity) level";
    ofDrawBitmapString(msg, textoffset +  sidebarMargin, sidebarMargin*.75);
    msg = "Pollution level";
    ofDrawBitmapString(msg, textoffset + ofGetWidth() - (sidebarMargin + indicatorHeight*1.8), sidebarMargin*.7);
    ofSetColor(0, 80);
    ofNoFill();
    ofDrawCircle(ofGetWidth() - (sidebarMargin + indicatorHeight), sidebarMargin + sidebarIndicatorP + (indicatorHeight/2), indicatorHeight);
    ofDrawCircle(sidebarMargin + indicatorHeight, sidebarMargin + sidebarIndicatorPh + (indicatorHeight/2), indicatorHeight);
    
}



void ofApp::drawBox2d() 
{
//    
    // some circles :)
    ofSetColor(151, 122, 93, 200);
    for (int i=0; i<circles.size(); i++) 
    {
       ofFill();
    //        ofSetHexColor(0xc0dd3b);
       circles[i].get()->draw();
//        auto c = circles[i];
//        c.get();
//        (circles[i].get()->bodyDef.position.x;
    }

}


void ofApp::drawCharacters()
{
    characters.at(0).drawCharacter(0, lastUserInput);
}


void ofApp::drawHands(ofPoint loc)
{
    ofSetColor(25, 180, 95);
    ofFill();
    ofDrawCircle(loc, 15);
    ofSetColor(0);
    ofNoFill();
    ofDrawCircle(loc, 15);
}
























void ofApp::tuioAdded(ofxTuioCursor &tuioCursor){
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),ofMap(tuioCursor.getY(), 0, 1, -150, 150 + ofGetHeight()));
    tuioLocations.push_back(loc);
    tuioIndex.push_back(tuioCursor.getSessionId());
    tuioLastUpdate.push_back(ofGetElapsedTimeMillis());
    lastUserInput = ofGetElapsedTimeMillis();
    // cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void ofApp::tuioUpdated(ofxTuioCursor &tuioCursor){
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),ofMap(tuioCursor.getY(), 0, 1, -150, 150 + ofGetHeight()));
    for (int i = 0; i < tuioLocations.size(); i++)
    {
        if (tuioCursor.getSessionId() == tuioIndex.at(i))
        {
            tuioLocations.at(i).set(loc);
            tuioLastUpdate.at(i) = ofGetElapsedTimeMillis();
        }
    }
    lastUserInput = ofGetElapsedTimeMillis();
    // cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void ofApp::tuioRemoved(ofxTuioCursor &tuioCursor){
    ofPoint loc = ofPoint(tuioCursor.getX()*ofGetWidth(),tuioCursor.getY()*ofGetHeight());
    
    vector <ofPoint> tempLocations;
    vector <float> tempIndex;
        
    for (int i = 0; i < tuioIndex.size(); i++)
    {
        if (tuioCursor.getSessionId() != tuioIndex.at(i) && tuioLastUpdate.at(i) + 2000 < ofGetElapsedTimeMillis())
        {
            tempLocations.push_back(tuioLocations.at(i));
            tempIndex.push_back(tuioCursor.getSessionId());
        }
        else {
            // cout << tuioCursor.getSessionId() << endl;
        }
    }
    // cout << "locaitons: " << tuioLocations.size() << ", index: " << tuioIndex.size() << endl;
    tuioLocations = tempLocations;
    tuioIndex = tempIndex;
    tempLocations.clear();
    tempIndex.clear();
    lastUserInput = ofGetElapsedTimeMillis();
    // cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}

