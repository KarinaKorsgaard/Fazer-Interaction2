//
//  binnedSystem.h
//  blobReceiverTest
//
//  Created by Karina Jensen on 12/06/16.
//
//

#ifndef binnedSystem_h
#define binnedSystem_h

#include "BinnedParticleSystem.h"
#include "defines.h"
#include "BinnedParticle.h"
#include "person.h"

class binnedSystem{
public:
    

    int radius;
    int padding;
    ofColor col;
    // binned
    float timeStep;
    float particleNeighborhood, particleRepulsion;
    float centerAttraction;
  
    vector<Person>* people;
    int kBinnedParticles;
    BinnedParticleSystem particleSystem;
    bool bRepelFrom, slowMotion;
    ofImage* tree;
    vector<ofVec2f> repelFrom;
    ofVec2f binCenter;
    ofVec2f vel;
    bool drawBalls = true;
    float originalBincenter;
    ofPath trunk;
    bool oldApp = false;
    ofTexture tex;
    ofxAutoReloadedShader shader;
    
    ofMesh mesh;
    ofVbo vbo;
    int uniqueWillow;
    int uniqueSpeed;
    int r,g,b;
    vector<float> pSizes;
    int theWidth;
    
    void setup(ofVec2f _center, int _amount, float _radius){
        originalBincenter=_center.x;
        r = int(ofRandom(-20,20));
        g = int(ofRandom(-20,20));
        b = int(ofRandom(-20,20));

        uniqueWillow=int(ofRandom(10,35));
        uniqueSpeed=int(ofRandom(20,70));
        int modCount = 0;


        vel = ofVec2f(ofRandom(-0.5,0.5),ofRandom(-0.5,0.5));
        theWidth = 1.05*sqrt(_amount)*_radius;
        
        shader.load("shader/shader");
        tex.allocate(128,128, GL_RGBA);
        ofDisableArbTex();
        ofLoadImage(tex, "dot.png");
        ofEnableArbTex();
        
        //// binned
        int binPower = 5;
        kBinnedParticles = _amount;
        padding = 20;
        particleNeighborhood = _radius;
        particleSystem.setup(RES_W + padding * 2, RES_H + padding * 2, binPower);
        
       
        
        for(int i = 0; i < kBinnedParticles; i++) {
            float x =  ofRandom(0,RES_W);
            float y = ofRandom(0, RES_H);
            BinnedParticle particle(x, y, 0, 0);
            particleSystem.add(particle);
        }
        
        //vbo.setNormalData(&colorMesh.getVertices()[0], (int)kBinnedParticles, GL_STATIC_DRAW);

        slowMotion = true;
        particleRepulsion = .3;
        centerAttraction = .1;
  
        binCenter = _center;
        timeStep=50;

    }

    
    void update(){

        // clusters
        repelFrom.clear();
        bRepelFrom = false;
        
       // int cNum = int(ofMap(binCenter.x,0,RES_W,0,4));

        
        for(int i = 0; i<people->size();i++){
            for(int u = 0; u<people->at(i).points.size();u++){
                // if(people.area == cNum){
                int y = people->at(i).points[u].y;
                int x = people->at(i).points[u].x;
                
                if(1/b2InvSqrt((binCenter.x-x)*(binCenter.x-x)+(binCenter.y-y)*(binCenter.y-y))< particleSystem.getWidth()/2){
                    repelFrom.push_back(ofPoint(x,y));
                    bRepelFrom = true;
                }
                // }
            }
        }
        
        particleSystem.setupForces();
      //  particleSystem.setTimeStep(timeStep);
        
        float willow = ofMap(sin(ofGetFrameNum()/uniqueSpeed),-1,1,0.2,0.6);
        
        particleRepulsion = willow;
        //binCenter.y = binCenter.y+ofSignedNoise(-10,10, ofGetElapsedTimef()*0.001);

        if(!drawBalls) {
            ofSetLineWidth(1);
            glBegin(GL_LINES); // need GL_LINES if you want to draw inter-particle forces
        }
        
        mesh.clear();
        for(int i = 0; i < particleSystem.size(); i++) {
            BinnedParticle& cur = particleSystem[i];
            // global force on other particles
            particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
            // forces on this particle
            cur.bounceOffWalls(0, 0, particleSystem.getWidth(), particleSystem.getHeight());
            cur.addDampingForce();
            
            
            if(!oldApp)mesh.addVertex(ofVec3f(particleSystem[i].x,particleSystem[i].y,pSizes[i]*0.3));
            else mesh.addVertex(ofVec3f(particleSystem[i].x,particleSystem[i].y,particleNeighborhood*0.3));
            
        }
        
        move();
        
        if(!drawBalls) {
            glEnd();
        }
        // single-pass global forces
        particleSystem.addAttractionForce(binCenter.x, binCenter.y, particleSystem.getWidth() , centerAttraction);
        if(bRepelFrom) {
            for(int i = 0 ; i<repelFrom.size();i++)particleSystem.addRepulsionForce(repelFrom[i].x + padding, repelFrom[i].y + padding, 50, 1);
        }
        particleSystem.update(ofGetLastFrameTime());
        
        
        vbo.setVertexData(&mesh.getVertices()[0], (int)particleSystem.size(), GL_STATIC_DRAW);

        
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
    
    void move(){
    
        if(binCenter.x>RES_W-theWidth/2){vel.x=-vel.x;binCenter.x = RES_W-theWidth/2;}
        
        if(binCenter.y>RES_H-theWidth/2){vel.y =-vel.y;binCenter.y = RES_H-theWidth/2;}
        
        if(binCenter.x<theWidth/2){vel.x =-vel.x;binCenter.x = theWidth/2;}
        
        if(binCenter.y<theWidth/2){vel.y =-vel.y;binCenter.y = theWidth/2;}
        
        binCenter += vel;
    }
    

};
#endif /* binnedSystem_h */
