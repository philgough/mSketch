#pragma once

#include "ofMain.h"
#include "PoissonPoints.hpp"
#include "ofxVoronoi.h"
#include "character.hpp"
#include "organisms.hpp"

#include "ofxOpenNI.h"

#include "ofxBox2d.h"

#include "ofxCsv.h"

#include "ofxTextBlock.h"

//#include "openni_motor.cpp"


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
		// void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void mouseEntered(int x, int y);
//		void mouseExited(int x, int y);
//		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);
    

        void variableSetup();
    
    
//private:
    
//    PoissonPoints pp = PoissonPoints(float tempMaxNumPoints, float tempMinDist, float tempTestPoints, float width, float height);
    
    int numPoints;
    vector<ofVec2f> pointList;
    vector<ofVec2f> riverLocations;
    vector<int> voronoiPixelAssignments;
    vector<float> hueList;
    vector<float> satList;
    vector<float> distList;
    
    
    
    void setupPP();
    void setupRiverbed();
    void updateRiver();
    void drawRiver();
    void drawBenthic();
    
    float widthScale;

    // river
    ofxVoronoi voronoiRiver;
    
    ofxVoronoi voronoiBenthic;

    vector <ofPolyline> benthicPoly;

    vector<ofPoint> riverPoints;
    bool isBorder(ofPoint _pt);
    
    vector <float> pollutionOffset;
    int _riv_w = 1920;
    int _riv_h = 200;
    int _benth_h = 450;
    
    
    
    // land layer
    // ofImage topBgA;
    // ofImage topBgB;
    
    void drawLand();
    
    // colours for changing background gradient on top level
//    float sh1, ss1, sb1, sh2, ss2, sb2, eh1, es1, eb1, eh2, es2, eb2;
//    ofImage landBG1, landBG2;
//    ofImage landFG1, landFG2;
//    ofImage sky1, sky2, sky3, sky4;
    
    ofImage bush, city, farm, sky;
    ofImage introBush, introCity, introFarm;
    vector <ofImage> selectionImages;
    // kinect
    void setupOpenni();
    void updateOpenNi();
    void drawSkele();
    void drawHands();
    void userEvent(ofxOpenNIUserEvent & event);

    
    ofxOpenNI openNIDevice;


    // particles
    ofxBox2d                               box2d;   // the box2d world
    vector   <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles

    // line for the circles to interact with
    vector <ofPolyline>                    lines;
    vector <shared_ptr<ofxBox2dEdge> >      edges;

    void drawBox2d();
    void setupBox2d();
    void updatePollution();
//    bool shouldRemove(ofxBox2dCircle c);
    
    


    // Welcome Screen Vars
    int selectedLevel = -1;
    int pSelectedLevel = 0;
    float selectionPercent = 0;





    
    
    // organisms
    void setupOrganisms();
    vector <Organism> organisms;
    map<string, string> loadDataset(int type, int index);
    vector <string> CSVHeaders;
    ofxCsv csvData;
    float phMin = 6.8;
    float phMax = 8.5;
    float phosMin = 0.0;
    float phosMax = 0.3;
    float phRange = 1.7;
    float currentpH;

    void drawOrganism();


    void setupBug();
    void updateBug();
    void updateBugTargetLocations();
    void drawBug();
    int numPlayers;
    vector <ofVec2f> bugLocations;
    vector <float> bugHealth;
    // health bar dimensions
    float hb_w = 40;
    float hb_h = 10;
    float h_off = -20;
    float hb_bo = 1;
    vector <ofVec2f> bugTargetLocations;
    
    
    // interface elements
    vector <ofMesh> star;
    ofVec2f starRadii;
    ofTrueTypeFont dejaVuSans;
    ofTrueTypeFont dejaVuSansBold;
    ofTrueTypeFont comic;
    string displayPh;
    void setupStars();
    void drawStars();
    void setupImages();
//    ofImage faceA, faceK, faceT;
    ofImage helloImage, goodbyeImage;
//    bool aHasSomethingToSay, kHasSomethingToSay, tHasSomethingToSay;
//    Character Anthony;
//    Character Kate;
//    Character Thomas;
    vector <Character> characters;
    
    
    // things the characters to/say to the user
    void setupCharacters();
    void updateCharacters();
    void drawCharacters();
    vector <string> aComments, kComments, tComments;
    int talkingTimer;
    int dayTimer;
    string spokenMessage;
    
    
    // drawing states
    void updateMain();
    void drawMain();
    void drawIntro();
    static const int WELCOME_SCREEN = 0;
    static const int INTERACTIVE_PLAY_STATE = 1;
    static const int FADE_IN = 2;
    static const int FADE_OUT = 3;
    static const int UPDATE_CYCLE = 4;
    
    // score screen states
    static const int SCORE_SCREEN = 5;

    // goodbye screen
    static const int THANK_YOU_SCREEN = 6;
    
    int location = 0;
    
    void drawScoreScreen();
    void drawByeScreen();
    
    int _masterState;
    int _lastState;
    int _nextState;
    
    float _fadeAlpha;
    
    int _stateTimer;
    int _noPlayerCheck;
    int _noPlayerDuration = 300000;
    
    bool userPresentAtIntro = false;
    
    
    
    
    // text
    
    ofxTextBlock welcomeBlock;
    vector <string> welcomeStringA;
    string welcomeStringB = "Great, let's get started!";

    int timer;
    
    void drawSwitch(int s);
    void nextState();
    void lastState();
    void fadeIn();
    void fadeOut();
    
    
    
    // most of the interaction is timed, so make some timer functions
    void drawMainTimer();
    void scoreTimer();
    int _drawMainDuration = 40000;
    int _scoreScreenDuration = 20000;
    
    
    // variable for the pollution and pH values as a 2d vector
    vector <ofVec2f> environmentVariables;
    

    void updateEndgame();
    int gameOutcome = 0;
    ofxTextBlock goodbyeBlock;

    void setupWelcomeScreen();




    float currentPollution = 0;
    

    void drawSidebars();
    void drawSidebarIndicators();
    float sidebarMargin = 60;
    float sidebarWidth = 80;
    float sidebarHeight;
    void updateEnvironment();
    float sidebarIndicatorP = 0;
    float sidebarIndicatorPh = 0;
    float indicatorHeight = 40;
    float sinP;
    float sinPh;
    int sidebarAlphaL, sidebarAlphaR;
    void updateBox2d();

    float landShiftX = 0;
};
