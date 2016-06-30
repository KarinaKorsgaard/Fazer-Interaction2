#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxAutoReloadedShader.h"
#include "ofxSyphon.h"
#include "ofxBox2d.h"
#include "defines.h"
#include "ofxPostProcessing.h"

#include "swarmParticle.h"
#include "binnedSystem.h"

class CustomParticle : public ofxBox2dCircle {
    
public:
    int age=0;
    float radius;
    int num;
    ofVec2f attractionPoint;
    CustomParticle() {
        radius = getRadius();
    }
    
    void update(){
        ofVec2f vel= getVelocity();
        ofVec2f p= getPosition();
      //  addAttractionPoint(RES_W/2,RES_H/2,2);
        if(getPosition().x>(num+1)*(RES_W/4)){
            addAttractionPoint(attractionPoint,3);
           // addAttractionPoint(RES_W/2,RES_H/2,2);
//            setVelocity(vel);
//            setPosition(RES_W,p.y);
        }
        if(getPosition().y>RES_H){
            addAttractionPoint(attractionPoint,3);
            // addAttractionPoint(RES_W/2,RES_H/2,2);
//            setVelocity(vel);
//            setPosition(p.x,RES_H);
        }
        
        if(getPosition().x<num*(RES_W/4)){
            addAttractionPoint(attractionPoint,3);
            // addAttractionPoint(RES_W/2,RES_H/2,2);
//            setVelocity(vel);
//            setPosition(0,p.y);
        }
        if(getPosition().y<0){
            addAttractionPoint(attractionPoint,3);
            // addAttractionPoint(RES_W/2,RES_H/2,2);
//            setVelocity(vel);
//            setPosition(p.x,0);
        }
       // addAttractionPoint(attractionPoint,0.5);
        if(vel.length()<1)setVelocity(vel*2);
        if(vel.length()>5)setVelocity(vel*0.8);
    }
};

class soundParticle{
    public:

};

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
    
    int binnedReset;
    bool					drawGui;
    bool bDebug = true;
    bool useB2d = true;
    
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
    ofParameter<bool>swarm,cluster,evenColor,bUserParticles,bBox2d,simplify, drawAnimals,blend_ADD,blurImg,fluidcolor,oldApp;
    ofParameter<ofColor>swarmColor;
    ofParameter<ofColor>clusterRange1;
    ofParameter<ofColor>clusterRange2;
    ofParameter<float>cAttraction, pRepulsion, density,bounce,friction;
    
    ofParameter<float>sNear, sFar;
    // POINTS IN BLOBS!
    vector<vector<ofPoint>> attractPoints;
    
    
    // swarming Particles
    vector <swarmParticle> swarmParticles;
    void resetParticles();
    
    //clusters
    vector<binnedSystem>clusters;
    binnedSystem backgroundCluster;
    vector<ofImage>theTrees;
    
    vector<vector<ofVec2f>> soundGrid;
    vector<vector<bool>> soundToggle;
    vector<vector<int>> soundAge;
    ofxOscSender soundSender;
    
    ofFbo pointSplineFbo;
    ofTexture sparkImg;
    ofTexture solid;
    ofTexture dot;
    ofxAutoReloadedShader pointSpline;
    
    //box 2 d;
    ofxBox2d                             box2d;
    vector <shared_ptr<CustomParticle> > customParticles;
    vector <shared_ptr<ofxBox2dCircle> >  circles;
    vector <shared_ptr<ofxBox2dEdge> >   edges;
    vector <shared_ptr<ofxBox2dRect> >   rects;
    vector <shared_ptr<ofxBox2dPolygon> > polyShapes;
    
    // swarm mesh and vbo
    ofMesh mesh;
    ofVbo vbo;
    vector<int>pointSizes;
    vector<ofVec2f>centroids;
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
    ofxPostProcessing post;
    vector<ofColor>fazerColors; 
};
