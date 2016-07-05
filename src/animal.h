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
    bool expand = false;
    bool countDown = false;
    int thres = 200;
    int count = 0;
    bool track = false;
    bool contract = false;
    bool idle = false;
    int idleCounter;
    ofVec2f acc;
//    shared_ptr<ofxBox2dPolygon> b2dPoly;

    void setup(ofVec2f _pos, string _oscAddress, string _moving, string _still,  vector<Person>* _peeps){
        still = new ofVideoPlayer;
        moving = new ofVideoPlayer;
        still->load(_still);
        moving->load(_moving);
        
        still->setLoopState(OF_LOOP_NORMAL);
        moving->setLoopState(OF_LOOP_NONE);
        w = still->getWidth()*0.4;
        h = still->getHeight()*0.4;
        pos = _pos;
        oscAddress = _oscAddress;
        touched = false;
        people = _peeps;
        
        still->play();

    }
    
    void update(){
        idleCounter++;
        isOn = false;
        sendOsc= false;
        

        if(!beginSequence){
            for(int i = 0 ; i<people->size();i++){
                for(int u = 0 ; u<people->at(i).points.size();u++){
                    if(!isOn){
                        ofPoint p = people->at(i).points[u];
                        if(dist(pos,p)<50){
                            isOn = true;
                            idleCounter = 0;
                        }
                    }
                }
            }
        }
        if(idleCounter>1000)idle = true;
        
        if(isOn && !beginSequence){
            beginSequence = true;
            touched = true;
            sendOsc = true;
            still->setPaused(true);
            if(track)cout<<"begin"<<endl;
            count = 0;
        }

        if(beginSequence){
            if(!expand){
                expand = true;
                moving->setPaused(false);
                //moving->setFrame(0);
                moving->setSpeed(1);
                moving->play();
                if(track)cout<<"expand"<<endl;
            }

            if(expand && !moving->isPlaying() && !contract && !countDown){
                countDown = true;
                moving->setPaused(true);
                if(track)cout<<"hiding"<<endl;
                //expand = false;
            }
            
            if(countDown){
                count++;
                if(count>thres){
                    countDown = false;
                    contract = true;
                    moving->setPaused(false);
                   // moving->setFrame(moving->getTotalNumFrames());
                    moving->setSpeed(-1);
                    moving->play();
                    if(track)cout<<"reappear"<<endl;
                    pos = ofVec2f(ofRandom(200,RES_W-200),ofRandom(200,RES_H-200));
                }
            }
            if(contract && !moving->isPlaying()){
                idleCounter = 0;
                contract = false;
                idle = false;
                expand = false;
                still->setPaused(false);
                touched = false;
                countDown = false;
                moving->setPaused(true);
                beginSequence = false;
                if(track)cout<<"stop"<<endl;

            }
        }
    
        

        still->update();
        moving->update();

        

    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(pos);

        ofSetColor(ofColor::white);
        if(!touched)still->draw(-w/2,-h/2,w,h);
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
