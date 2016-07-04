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
#include "person.h"

class Animal {
    
public:
    string oscAddress;
    ofVec2f pos;
    ofVideoPlayer* still;
    ofVideoPlayer* moving;
    bool touched;
    ofPolyline poly;
    ofRectangle boundingBox;
    float radius;
    vector<Person>* people;
    int w,h;
    ofVec2f vel;
    bool sendOsc = false;

    bool beginSequence = false;
    bool isOn = false;
    bool contract = false;
    bool expand = false;
    bool resetForContract = false;
    
    bool track = false;
    ofVec2f acc;
//    shared_ptr<ofxBox2dPolygon> b2dPoly;

    void setup(ofVec2f _pos, string _oscAddress, string _moving, string _still,  vector<Person>* _peeps){
        still = new ofVideoPlayer;
        moving = new ofVideoPlayer;
        still->load(_still);
        moving->load(_moving);
       // w = still->getWidth();
       // h = still->getHeight();
       // still->resize(w/4,h/4);
       // moving->resize(w/4,h/4);
        
        
        still->setLoopState(OF_LOOP_NORMAL);
        moving->setLoopState(OF_LOOP_NONE);
        w = still->getWidth();
        h = still->getHeight();
        pos = _pos;
        
      // if(pos.x<210)track=true;
        oscAddress = _oscAddress;
        touched = false;
        people = _peeps;
        radius = 150;
        vel = ofVec2f(ofRandom(-2,2), ofRandom(-0.5,0.5));
        if(vel.x<0.4)vel.x = 2;
       // acc = ofVec2f(ofRandom(-1,1),-0.8);
    }
    
    void update(){
        isOn = false;
        sendOsc= false;

        int region =0;
        region = int(ofMap(pos.x,0,RES_W,0,4));
        for(int i = 0 ; i<people->size();i++){
            for(int u = 0 ; u<people->at(i).points.size();u++){
                if(!isOn){
                    ofPoint p = people->at(i).points[u];
                    if(dist(pos,p)<50){
                        isOn = true;
                    }
                }
            }
        }
        
        if(isOn && !beginSequence){
            beginSequence = true;
            touched = true;
            sendOsc= true;
            moving->setFrame(0);
            bool contract = false;
            bool expand = false;
            still->setPaused(true);
            
            if(track)cout<<"begin"<<endl;
        }

        if(beginSequence){
            if(!expand){
                expand = true;
                moving->setSpeed(1);
                moving->play();
                if(track)cout<<"expand"<<endl;
            }
            
            if(expand &&  !moving->isPlaying() && !resetForContract){
                resetForContract = true;
                moving->setFrame(120);
                moving->setPaused(true);
                if(track)cout<<"limbo"<<endl;
                // moving->setPaused(true);
               // if(track)cout<<"limbo"<<endl;
            }
            
            if(expand && !isOn && !contract){
               // expand = false;
                contract = true;
                moving->setPaused(false);
                moving->setSpeed(-1);
                moving->play();
                if(track)cout<<"contract"<<endl;
            }
            
            if(contract && isOn){
                contract = false;
                expand = true;
                resetForContract = false;
                moving->setSpeed(1);
                if(track)cout<<"re-expand"<<endl;
            }
            
            if(contract && !moving->isPlaying()){
                contract = false;
                expand = false;
                beginSequence = false;
                touched = false;
                still->setPaused(false);
                resetForContract=false;
                if(track)cout<<"stop"<<endl;
            }
        }
    
        
        
//        if(!touched) still->update();
//        else moving->update();
                still->update();
                moving->update();

        
        // move : borders:
        int lim =150;
        if( pos.x > RES_W-lim ){
            pos.x = RES_W-lim;
            vel.x *= -1;
        }else if( pos.x < lim ){
            pos.x = lim;
            vel.x *= -1;
        }
        if( pos.y > RES_H-lim){
            pos.y = RES_H-lim;
            vel.y = -ofRandom(6);
        }

        
        pos+=vel;
        vel.y+=0.1;
    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(pos);

        ofSetColor(ofColor::white);
        if(!touched)still->draw(-w/2,-h/2,w,h);
       // ofSetColor(ofColor::green);
        if(touched) moving->draw(-w/2,-h/2,w,h);
    



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
