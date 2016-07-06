//
//  customParticles.h
//  blueParticles_Fazer
//
//  Created by FazerPro on 05/07/16.
//
//

#ifndef customParticles_h
#define customParticles_h
#include "defines.h"

class CustomParticle : public ofxBox2dCircle {
    
public:
    //int age=0;
    float radius;
    int num;
    //ofColor col;
    ofVec2f attractionPoint;
    CustomParticle() {
        radius = getRadius();
    }
    
    void update(){
        ofVec2f vel= getVelocity();
        ofVec2f p= getPosition();
        //  addAttractionPoint(RES_W/2,RES_H/2,2);
        if(p.x>RES_W-radius){//(num+1)*(RES_W/4)){
            addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
            
        }
        if(p.y<radius*2){
            addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
        }
        
        if(p.x<radius){//num*(RES_W/4)){
            addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
        }
        
        //   if(ofRandom(1)<0.001)addRepulsionForce(attractionPoint,0.1);
        if(vel.length()<1)setVelocity(vel*1.2);
        if(vel.length()>4)setVelocity(vel*0.8);
        
    }
    
    
};

class MovingSoundParticle : public ofxBox2dCircle {
    
public:
    //int age=0;
    float radius;
    int num;
    bool toggle;
    int curTime = 0;
    bool animateRadius;
    string name;
    //ofColor col;
    ofVec2f attractionPoint;
    MovingSoundParticle() {
        radius = getRadius();
    }
    
    float inOut (float t,float b,float c,float d) {
        return c * sin(t/d * (PI/2)) + b;
    };
    
    void update(){
        if(animateRadius){
            curTime++;
            float curRadius = inOut(curTime,radius,radius/2,SOUND_ANIMATION);
            //cout<<curRadius<<endl;
            setRadius(curRadius);
            if(curTime>SOUND_ANIMATION*2){
                curTime=0;
                animateRadius=false;
                setPosition(ofRandom(RES_W), -radius);
                setRadius(radius);
            }
        }
        
        ofVec2f vel= getVelocity();
        ofVec2f p= getPosition();
        if(p.x>RES_W-radius){//(num+1)*(RES_W/4)){
            addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
        }
        if(p.y<radius*2){
            addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
        }
        if(p.x<radius){//num*(RES_W/4)){
            addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
        }
        if(vel.length()<MIN_VEL)setVelocity(vel*1.2);
        if(vel.length()>MAX_VEL)setVelocity(vel*0.8);
        
    }
    
    
};


class AnimalParticle : public ofxBox2dCircle {
    
public:
    //int age=0;
    float radius;
    int num;
    bool toggle;
    int curTime = 0;
    bool animateRadius;
    bool hide = false;
    string name;
    //ofColor col;
    ofVec2f attractionPoint;
    AnimalParticle() {
        radius = getRadius();
    }
    
    float inOut (float t,float b,float c,float d) {
        return c * sin(t/d * (PI/2)) + b;
    };
    
    void update(){
        if(animateRadius){
            curTime++;
            float curRadius = inOut(curTime,radius,radius/2,ANIMAL_ANIMATION);
            //cout<<curRadius<<endl;
            setRadius(curRadius);
            if(curTime>ANIMAL_ANIMATION*2){
                curTime=0;
                animateRadius=false;
                setPosition(ofRandom(RES_W), -radius);
                hide = true;
            }
        }
        
        if(hide && ofRandom(ANIMAL_CHANCE)<1){
            hide = false;
            setPosition(ofRandom(RES_W), -radius);
            setRadius(radius);
        }
        
        if(!hide){
            ofVec2f vel= getVelocity();
            ofVec2f p= getPosition();
            if(p.x>RES_W-radius){//(num+1)*(RES_W/4)){
                addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
            }
            if(p.y<radius*2){
                addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
            }
            if(p.x<radius){//num*(RES_W/4)){
                addAttractionPoint(attractionPoint,CENTER_ATTRACTION);
            }
            if(vel.length()<MIN_VEL)setVelocity(vel*1.2);
            if(vel.length()>MAX_VEL)setVelocity(vel*0.8);
        }
        
    }
    
};
#endif /* customParticles_h */
