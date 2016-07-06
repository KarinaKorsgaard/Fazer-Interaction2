#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxAutoReloadedShader.h"
#include "ofxSyphon.h"
#include "ofxBox2d.h"
#include "defines.h"
#include "animalPng.h"
#include "person.h"
#include "customParticles.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<ofxOscReceiver>receivers;
    vector<vector<ofPolyline>>blobs;
    
    vector<AnimalPng>animals;

    bool		drawGui;
    bool bDebug = false;
    bool useOverlaps=true;
    
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameterGroup visualControl;
    ofParameterGroup group;
    ofParameter<float>offSet1X,offSet2X,offSet3X,offSet4X,b2bBounce;
    ofParameter<float>offSet1Y,offSet2Y,offSet3Y,offSet4Y;
    ofParameter<float>scale1,scale2,scale3,scale4;
    ofParameter<int>smoothe;
    ofParameter<int>resample;
    ofParameter<int>numAttractionP,attractionPointOverlap;
    ofParameter<int>overLap1,overLap2,overLap3,b2dRepulsion;
    ofParameter<bool>swarm,cluster,evenColor,bUserParticles,bBox2d,simplify, useInsidePoly,drawAnimals,blend_ADD,blurImg,fluidcolor,oldApp,blurImg1,evenColor1;
    ofParameter<ofColor>background;

    ofParameter<float>pRepulsion;
    
    ofParameter<float>sNear, sFar;

    
    vector<Person>people;

    vector<int>rCounters;
    ofxOscSender soundSender;

    ofFbo pointSplineFbo;

    ofTexture solid;


    ofxAutoReloadedShader pointSpline;


    
    //box 2 d;
    ofxBox2d                             box2d;
    vector <shared_ptr<CustomParticle> > customParticles;
    vector <shared_ptr<MovingSoundParticle> > movingSounds;
    vector <shared_ptr<AnimalParticle> > animalParticles;
    int total;
    // swarm mesh and vbo
    ofMesh mesh;
    ofVbo vbo;


    ofFbo finalRender;
    ofxSyphonServer syphon;

    //inverse sqrt
    inline Float32 b2InvSqrt(Float32 x)
    {
        int ix = 0;
        memcpy(&ix, &x, sizeof(x));
        
        Float32 xhalf = 0.5f * x;
        ix = 0x5f3759df - (ix >> 1);
        memcpy(&x, &ix, sizeof(x));
        x = x * (1.5f - xhalf * x * x);
        return x;
    }

    vector<ofColor>fazerColors; 
};
