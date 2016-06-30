//
//  animal.h
//  blueParticles_Fazer
//
//  Created by Karina Jensen on 30/06/16.
//
//

#ifndef animal_h
#define animal_h


#include "defines.h"


class Animal {
    
public:
    string oscAddress;
    ofVec2f pos;
    ofVideoPlayer still;
    ofVideoPlayer moving;
    bool touched;
    ofPolyline poly;
    ofRectangle boundingBox;
    float radius;
    vector< vector<ofPoint>>* attractPoints;
    int w,h;
    
    bool sendOsc = false;
    bool fowards;
    bool backwards;

//    shared_ptr<ofxBox2dPolygon> b2dPoly;

    void setup(ofVec2f _pos, string _oscAddress, string _moving, string _still,  vector<vector <ofPoint>>* _attractPoints){
        still.load(_still);
        moving.load(_moving);
        
        
        
        still.setLoopState(OF_LOOP_NORMAL);
        moving.setLoopState(OF_LOOP_NONE);
        w = still.getWidth();
        h = still.getHeight();
        pos = _pos;
        oscAddress = _oscAddress;
        touched = false;
        attractPoints = _attractPoints;
        still.update();
        still.play();
        

    }
    
    void update(){
        bool on = false;
        sendOsc= false;

        int region =0;
        region = int(ofMap(pos.x,0,RES_W,0,4));
        for(int i = 0 ; i<attractPoints->at(region).size();i++){
            ofPoint p = attractPoints->at(region)[i];
            if(dist(pos,p)<radius){
                on = true;
                still.setLoopState(OF_LOOP_NONE);
            }
        }
        
        if(on && !still.isPlaying() && !touched){
            touched = true;
            fowards = true;
            sendOsc = true;
            moving.play();
        }

        if(fowards){
            if(!moving.isPlaying() || !on ){
                backwards = true;
                moving.setSpeed(-1);
                fowards = false;
            }
        }
        if(backwards && !moving.isPlaying()){
            backwards = false;
            moving.setSpeed(1);
        }
        
        if(!fowards && !backwards && !on){
            touched = false;
            still.setLoopState(OF_LOOP_NORMAL);
            still.play();
        }
        
//        if(touched && !moving.isPlaying() && !reverse){
//            reverse = true;
//            moving.setSpeed(-1);
//            moving.play();
//        }
//        if(touched && !on && moving.isPlaying()){
//            reverse = true;
//            moving.setSpeed(-1);
////            moving.play();
//        }



        

        
        

        
        
        if(!touched) still.update();
        else moving.update();
        
        
    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(pos);

        ofSetColor(ofColor::white);
        if(!touched)still.draw(-w/2,-h/2,w,h);
        else moving.draw(-w/2,-h/2,w,h);
        ofTranslate(0,-200);
        ofSetColor(ofColor::white);



        ofPopMatrix();
        

        
    }
    
    
    float dist(ofVec2f a, ofPoint b){
        return  1/b2InvSqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
    }
    
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
    //                        poly->addVertices(drawing.getVertices());
    //                        poly->setPhysics(density, bounce, friction);
    //                        poly->triangulatePoly();
    //                        poly.get()->create(box2d.getWorld());
    //                        polyShapes.push_back(poly);
};

#endif /* animal_h */
