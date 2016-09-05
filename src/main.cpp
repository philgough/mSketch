	#include "ofMain.h"
#include "ofApp.h"

//========================================================================
//int main( ){
////    ofAppGlutWindow window;
////    window.setGlutDisplayString("rgba double samples>=4");
////    glfwWindowHint(GLFW_DECORATED, false);
//	ofSetupOpenGL(1920,1040,OF_WINDOW);			// <-------- setup the GL context
////    ofScaleMode(OF_SCALEMODE_CENTER);
////    ofSetupOpenGL(1920, 1040, OF_WINDOW);
//    ofSetWindowTitle("18S rDNA");
////    ofSetupOpenGL(1020, 1040, OF_)
////        ofSetupOpenGL(&window, 1920, 1080,OF_FULLSCREEN);
//	// this kicks off the running of my app
//	// can be OF_WINDOW or OF_FULLSCREEN
//	// pass in width and height too:
////    glfwWindowHint(GLFW_DECORATED, false);
//    
//    
//    
//    
//    
//    
//	ofRunApp(new ofApp());
//}



int main()
{
    
    ofGLFWWindowSettings settings;
    settings.decorated = false;
    settings.width = 1920;
    settings.height = 1040;
    settings.setPosition(ofVec2f(1920,0));
    settings.resizable = false;
    
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}