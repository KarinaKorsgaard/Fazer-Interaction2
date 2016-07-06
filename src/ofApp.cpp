#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    group.setName("FazerParticles");
    parameters.setName("offsetControl");
    ofParameterGroup aligning;
    aligning.add(scale1.set("scaleX",1,0.1,4));
    aligning.add(scale2.set("scaleY",1,0.1,4));
    ofParameterGroup one,two,three,four;
    one.setName("one");
    two.setName("two");
    three.setName("three");
    four.setName("four");
    //aligning.add(useOverlaps.set("use define overlaps", true));
    one.add(offSet1X.set("offSet1X",0,RES_W,-100));
    one.add(offSet1Y.set("offSet1Y",0,-900,500));
    two.add(offSet2X.set("offSet2X",0,RES_W,-100));
    two.add(offSet2Y.set("offSet2Y",0,-900,500));
    //two.add(overLap1.set("overLap1",0,-700,700));
    three.add(offSet3X.set("offSet3X",0,RES_W,-100));
    three.add(offSet3Y.set("offSet3Y",0,-900,500));
   // three.add(overLap2.set("overLap2",0,-700,700));
    four.add(offSet4X.set("offSet4X",0,RES_W,-100));
    four.add(offSet4Y.set("offSet4Y",0,-900,500));
  //  four.add(overLap3.set("overLap3",0,-700,700));
    aligning.add(numAttractionP.set("numAttraction",5,1,60));
    
   // aligning.add(blobFilter.set("blobFilter",5,0,20));
   // aligning.add(fc.set("fc",0.02,0.01,3));

    aligning.add(one);
    aligning.add(two);
    aligning.add(three);
    aligning.add(four);
    
    visualControl.setName("visuals");
    visualControl.add(swarm.set("particles", true));
    visualControl.add(drawAnimals.set("animals", true));
    
    //visualControl.add(sNear.set("sNear",0,1,200));
    visualControl.add(b2dRepulsion.set("b2dRepulsion",0,1,200));

    ofParameterGroup colorControl;
    colorControl.add(background.set("background",ofColor(255,255), ofColor(0,0),ofColor(255)));

    
    group.add(aligning);
    group.add(parameters);
    group.add(visualControl);
    group.add(colorControl);

    gui.setup(group);
    gui.loadFromFile("settings.xml");
    
    
    // osc receiver setup:
    for(int i = 0; i<4;i++){
        ofxOscReceiver rec = *new ofxOscReceiver;
        int port = 8001+i;
        rec.setup(port);
        receivers.push_back(rec);
    }
    
    //15 blob limit for each kinect:
    blobs.resize(4);
    for(int i = 0; i< blobs.size();i++){
        blobs[i].resize(15);
    }
    peeps.setFc(0.2);
    
    pointSplineFbo.allocate(RES_W,RES_H+80);
    pointSplineFbo.begin();
    ofClear(0);
    pointSplineFbo.end();
    
    finalRender.allocate(RES_W,RES_H);
    finalRender.begin();
    ofClear(0);
    finalRender.end();

    solid.allocate(128,128, GL_RGBA);
    ofDisableArbTex();
    ofLoadImage(solid, "clear.png");
    ofEnableArbTex();
    
    pointSpline.load("shader/shader");
 
    ofImage fCol;
    fCol.load("fazerColors.jpeg");
    int stripeW = fCol.getWidth() / 5;
    for(int i = stripeW/2; i< fCol.getWidth();i+=stripeW){
        ofColor c = fCol.getColor(i,stripeW);
        fazerColors.push_back(c);
    }
    cout << fazerColors.size()<<endl;

    
    string path = "animals";
    ofDirectory dir(path);
    dir.listDir();
    
    for(int i = 0; i<dir.size();i++){
        AnimalPng ani = *new AnimalPng;
        ani.setup(dir.getPath(i),ofVec2f(ofRandom(RES_W),ofRandom(RES_H)));
        animals.push_back(ani);
        
        cout<<dir.getPath(i)<<endl;
    }
    
    //ofSetCircleResolution(100);
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.createGround(0, RES_H, RES_W, RES_H-20);
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    
    int num = SWARM_NUM;
    ofMesh colorMesh;
    int cIndx = 1;
    int fours = 0;
    


    for(unsigned int i = 0; i < SWARM_NUM; i++){
        fours++;
        fours = fours%2;
        customParticles.push_back(shared_ptr<CustomParticle>(new CustomParticle));
        CustomParticle * p = customParticles.back().get();
        float r = ofRandom(PARTICLES_MIN_RADIUS, PARTICLES_MAX_RADIUS);
        r = ofRandom(PARTICLES_MIN_RADIUS,r);
        //float density, float bounce, float friction
        p->setPhysics(ofRandom(0.1,5), ofRandom(0.1,0.5) ,0.1);
        p->setup(box2d.getWorld(), ofRandom(RES_W), -10, r);
        p->radius=p->getRadius();
        p->setVelocity(ofRandom(-0.5,0.5), ofRandom(-0.5,0.5));
        p->attractionPoint = ofVec2f(RES_W/8 * fours + RES_W/16, RES_H/2);
        colorMesh.addVertex(ofVec3f(fazerColors[cIndx].r,fazerColors[cIndx].g,fazerColors[cIndx].b));
        cIndx = (i%(fazerColors.size()-1)) +1;
        
    }
    
    for(int i = 0; i<NUM_SOUND_PARTICLES;i++){
        for(int u = 0; u<3;u++){
            movingSounds.push_back(shared_ptr<MovingSoundParticle>(new MovingSoundParticle));
            MovingSoundParticle * p = movingSounds.back().get();
            float r = ofRandom(SOUND_PARTICLES_MIN_RADIUS,SOUND_PARTICLES_MAX_RADIUS);		// a random radius 4px - 20px
            p->setPhysics(ofRandom(0.1,5), ofRandom(0.1,0.5) ,0);
            p->setup(box2d.getWorld(), ofRandom(RES_W), ofRandom(RES_H-50), r);
            p->radius=p->getRadius();
            p->setVelocity(ofRandom(-0.5,0.5), ofRandom(-0.5,0.5));
            p->attractionPoint = ofVec2f(RES_W/2,RES_H/2);
            p->name = "/"+ofToString(i)+"/"+ofToString(u);
            colorMesh.addVertex(ofVec3f(fazerColors[0].r,fazerColors[0].g,fazerColors[0].b));
        }
    }
    for(int u = 0; u<animals.size();u++){
        animalParticles.push_back(shared_ptr<AnimalParticle>(new AnimalParticle));
        AnimalParticle * p = animalParticles.back().get();
        float r = ofRandom(ANIMAL_PARTICLES_MIN_RADIUS,ANIMAL_PARTICLES_MAX_RADIUS);		// a random radius 4px - 20px
        p->setPhysics(5, ofRandom(0.1,0.5) ,0);
        p->setup(box2d.getWorld(), ofRandom(RES_W), ofRandom(RES_H-50), r);
        p->radius=p->getRadius();
        p->setVelocity(ofRandom(-0.5,0.5), ofRandom(-0.5,0.5));
        p->attractionPoint = ofVec2f(RES_W/2,RES_H/2);
        p->name = "/animals/"+ofToString(u);
        colorMesh.addVertex(ofVec3f(fazerColors[0].r,fazerColors[0].g,fazerColors[0].b));
    }
    
    total = customParticles.size()+movingSounds.size()+animalParticles.size();
    
    vbo.setNormalData(&colorMesh.getVertices()[0], (int)total, GL_STATIC_DRAW);
    

    soundSender.setup("localhost",3000);
    drawGui = true;
    bDebug = false;

    rCounters.resize(4);
    syphon.setName("FazerParticles");
    ofSetFrameRate(40);
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){


    vector<float>ofsetlistX;
    ofsetlistX={offSet1X,offSet2X,offSet3X,offSet4X};
    
    vector<float>ofsetlistY;
    ofsetlistY={offSet1Y,offSet2Y,offSet3Y,offSet4Y};
    
    vector<int>overlaps;
    if(useOverlaps)overlaps={0,overLap1,overLap2,overLap3};
    else overlaps={0,0,0,0};
    
    for(int r = 0; r<receivers.size();r++){
        for(int i = 0; i<rCounters.size();i++)rCounters[i]++;
        
        while(receivers[r].hasWaitingMessages()){
            
            
            ofxOscMessage msg;
            receivers[r].getNextMessage(msg);

            
            if(msg.getAddress()=="/numPeople"){
                for(int i=msg.getArgAsInt(0);i<blobs[r].size();i++){
                    blobs[r][i].clear();
                    
                }
                rCounters[r]=0;
            }
            
            if(msg.getNumArgs()>3){
                int pointCloudIndx=0;
                pointCloudIndx = ofToInt(ofSplitString(msg.getAddress(),"/")[1]);
                if(pointCloudIndx<blobs[r].size()){
                    blobs[r][pointCloudIndx].clear();
    
                    for(int  i = 0 ; i< msg.getNumArgs();i+=2){
                        
                        ofVec2f pt;
                        pt.x= RES_W-(ofMap(msg.getArgAsFloat(i),0,512,0,(RES_W/4)*scale1 ) + ofsetlistX[r]);
                        pt.y= ofMap(msg.getArgAsFloat(i+1),0,512,0,RES_H*scale2) + ofsetlistY[r];
                        
                        //if(pt.x > ofsetlistX[r] - overlaps[r]) blobs[r][pointCloudIndx].addVertex(pt);
                        blobs[r][pointCloudIndx].addVertex(pt);
                        
                    }
                    if(blobs[r][pointCloudIndx].size()>0){
                        blobs[r][pointCloudIndx].close();
                        blobs[r][pointCloudIndx]=blobs[r][pointCloudIndx].getResampledBySpacing(numAttractionP);
                    }
                }
            }
        }
    }
    
    //if no incoming, clear blobs
    for(int i = 0; i<rCounters.size();i++){
        if(rCounters[i]>200){
            for(int u = 0; u<blobs[i].size();u++)blobs[i][u].clear();
        }
    }
    
//    prevCentroids.clear();
//    for(auto p:people){
//        prevCentroids.push_back(p.centroid);
//        //cout<<prevCentroids.back().x<<endl;
//    }
    
    people.clear();
    for(int i = 0; i < blobs.size();i++){
        for(int u = 0; u< blobs[i].size();u++){
           ofPolyline p=blobs[i][u];
            if(p.size()>0){
                Person karina = *new Person;
            
                vector<ofPoint>points;
            
                points.push_back(p.getCentroid2D());
                for( int pt = 0; pt < p.getVertices().size(); pt++) {
                    points.push_back(p.getVertices().at(pt));
                }
                karina.centroid = points[0];
                karina.poly = p;
                karina.points = points;
                people.push_back(karina);
            }

        }
    }

    preoplePresentToggle = preoplePresent;
    // if there are more than 4 blobs, we assume at least one is human
    if(people.size()>3)preoplePresent = true;
    else preoplePresent = false;
    
//    // if there is less than 4 blobs, check if any moved since last frame
//    if( people.size()>0 && !preoplePresent){
//        
//        vector<bool>didCentroidsMove; // vector for holding if we found a centroid that was close to previous
//        for(int i = 0; i<people.size();i++){
//            int x1 = people[i].centroid.x;
//            int y1 = people[i].centroid.y;
//            
//            bool didPersonMove = true;
//            for(int u = 0; u<prevCentroids.size();u++){
//                int x2 = prevCentroids[u].x;
//                int y2 = prevCentroids[u].y;
//                int dist =1/b2InvSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
//                if(dist<blobFilter){ // if person centroid is close to any of the previous, it did not move
//                    didPersonMove = false;
//                }
//                
//               // cout <<dist<<endl;
//            }
//            
//            didCentroidsMove.push_back(didPersonMove);
//        }
//        bool arePeopleMoving = false;
//        for(int i = 0; i<didCentroidsMove.size();i++){
//            if(didCentroidsMove[i]){ // if anyone moved, someone is present
//                arePeopleMoving = true;
//                
//                //countPeopleLeft = 0;
//                
//            }
//        }
//        preoplePresent = arePeopleMoving;
//    }
//    cout << preoplePresent <<endl;
//    peeps.setFc(fc);
//    // send soundtrigger!
//    if(preoplePresent)peeps.update(2);
//    else peeps.update(0);
//
//    
//    cout << "biquad "+ofToString((peeps.value()))<<endl;
//    int value = int(peeps.value());
    
    if(preoplePresent && !preoplePresentToggle){ // if there were noone and now someone
        ofxOscMessage m;
        m.setAddress("/peoplePresent");
        m.addInt32Arg(1);
        soundSender.sendMessage(m);
        fuckLife = true;
    }

    if(!preoplePresent)countPeopleLeft++;
    if(countPeopleLeft>400 && fuckLife){
        fuckLife = false;
        ofxOscMessage m;
        m.setAddress("/peoplePresent");
        m.addInt32Arg(0);
        soundSender.sendMessage(m);
    }
    
    
    if(drawAnimals){
        for(int i = 0 ; i< animals.size();i++){
            animals[i].update();
        }
    }
    
    if(swarm){
        mesh.clear();
        for(int i = 0; i<customParticles.size();i++){
            
            int x1 = customParticles[i]->getPosition().x;
            int y1 = customParticles[i]->getPosition().y;
            
            bool found = false;
            
            for(int u = 0; u<people.size();u++){
                if(!found && abs(x1-people[u].centroid.x)<400){
                    for(int p = 0; p<people[u].points.size();p++){
                        if(!found){
                            int x2 = people[u].points[p].x;
                            int y2 = people[u].points[p].y;
                            int dist =1/b2InvSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                            if(dist<customParticles[i]->getRadius()*1.5){
                                found = true;
                                customParticles[i]->addRepulsionForce(people[u].centroid, b2dRepulsion);
                            }
                        }
                    }
                }
            }
            customParticles[i]->update();
            mesh.addVertex(ofVec3f(float(customParticles[i]->getPosition().x),float(customParticles[i]->getPosition().y),customParticles[i]->getRadius()));
            
        }
        for(int i = 0; i<movingSounds.size();i++){
            movingSounds[i]->toggle = false;
            if(!movingSounds[i]->animateRadius){
                
            int x1 = movingSounds[i]->getPosition().x;
            int y1 = movingSounds[i]->getPosition().y;

            bool found = false;
                for(int u = 0; u<people.size();u++){
                    if(!found && abs(x1-people[u].centroid.x)<400){
                        for(int p = 0; p<people[u].points.size();p++){
                            if(!found){
                                int x2 = people[u].points[p].x;
                                int y2 = people[u].points[p].y;
                                int dist =1/b2InvSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                                if(dist<movingSounds[i]->getRadius()*1.5){
                                    found = true;
                                    movingSounds[i]->animateRadius=true;
                                    movingSounds[i]->toggle = true;
                                }
                            }
                        }
                    }
                }
            }
            
            movingSounds[i]->update();
            mesh.addVertex(ofVec3f(float(movingSounds[i]->getPosition().x),float(movingSounds[i]->getPosition().y),movingSounds[i]->getRadius()));
            

            if(movingSounds[i]->toggle){
                ofxOscMessage m;
                m.setAddress(movingSounds[i]->name);
                m.addInt32Arg(1);
                soundSender.sendMessage(m);
                movingSounds[i]->toggle=false;
            }
        }
        
        for(int i = 0; i<animalParticles.size();i++){
            animalParticles[i]->toggle = false;
            
            if(!animalParticles[i]->animateRadius && !animalParticles[i]->hide){
                
                int x1 = animalParticles[i]->getPosition().x;
                int y1 = animalParticles[i]->getPosition().y;
                
                bool found = false;
                for(int u = 0; u<people.size();u++){
                    if(!found && abs(x1-people[u].centroid.x)<400){
                        for(int p = 0; p<people[u].points.size();p++){
                            if(!found){
                                int x2 = people[u].points[p].x;
                                int y2 = people[u].points[p].y;
                                int dist =1/b2InvSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                                if(dist<animalParticles[i]->getRadius()*1.5){
                                    found = true;
                                    animalParticles[i]->animateRadius=true;
                                    animalParticles[i]->toggle = true;
                                }
                            }
                        }
                    }
                }
            }
            
            animalParticles[i]->update();
            mesh.addVertex(ofVec3f(float(animalParticles[i]->getPosition().x),float(animalParticles[i]->getPosition().y),animalParticles[i]->getRadius()));
            
            
            if(animalParticles[i]->toggle){
                ofxOscMessage m;
                m.setAddress(animalParticles[i]->name);
                m.addInt32Arg(1);
                soundSender.sendMessage(m);
                animalParticles[i]->toggle=false;
            }
        }
        
        box2d.update();
        vbo.setVertexData(&mesh.getVertices()[0], total, GL_STATIC_DRAW);
    }


    if(swarm){
        pointSplineFbo.begin();
        ofClear(0, 0);
        ofEnableAlphaBlending();
        ofBackground(background);
        
        pointSpline.begin();
        ofSetColor(255);
        glDepthMask(GL_FALSE);
        ofEnablePointSprites();
        ofEnableAlphaBlending();

        if(swarm){
            
            solid.bind();
            vbo.draw(GL_POINTS, 0, int(vbo.getNumVertices()));
            solid.unbind();
  
        }
        
        pointSpline.end();
        glDepthMask(GL_TRUE);
        ofDisablePointSprites();
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for(int i = 0; i<movingSounds.size();i++){
            if(movingSounds[i]->animateRadius){
                int r = movingSounds[i]->getRadius();
                int oR = movingSounds[i]->radius;
                ofSetColor(fazerColors[0]);
                ofDrawCircle(movingSounds[i]->getPosition(),r+2);
            }
        }
        
        for(int i = 0; i<animalParticles.size();i++){
            if(animalParticles[i]->animateRadius){
                int r = animalParticles[i]->getRadius();
                int oR = animalParticles[i]->radius;
                ofSetColor(fazerColors[0]);
                ofDrawCircle(animalParticles[i]->getPosition(),r+2);
            }
        }
        pointSplineFbo.end();
        ofDisableBlendMode();
    }
    
    
    finalRender.begin();
    ofClear(0);
    ofSetColor(255);
    ofEnableAlphaBlending();

    if(swarm){

        pointSplineFbo.draw(0,0);

    }
    if(drawAnimals){
        int itr = 0;
        for(int i = 0; i<animalParticles.size();i++){
            animals[itr].draw(animalParticles[i]->getPosition(),animalParticles[i]->getRadius(), animalParticles[i]->getRotation());
            itr ++;
            itr=itr%animals.size();
        }
    }
    
    if(bDebug){

        ofSetLineWidth(10);
        int k1 = offSet1X + ((RES_W/4)*scale1 /2);
        int k2 = offSet1X;
        int k3 =(RES_W/4)*scale1 + offSet1X;
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);

        k1 = offSet2X + ((RES_W/4)*scale1 /2);
        k2 = offSet2X;
        k3 =(RES_W/4)*scale1 + offSet2X;
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);

        k1 = offSet3X + ((RES_W/4)*scale1 /2);
        k2 = offSet3X;
        k3 =(RES_W/4)*scale1 + offSet3X;
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);
        
        k1 = offSet4X + ((RES_W/4)*scale1 /2);
        k2 = offSet4X;
        k3 =(RES_W/4)*scale1 + offSet4X;
        
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);

        ofFill();
        ofSetLineWidth(4);

        ofSetColor(255);
        for(auto p:people)p.draw();
        
        ofSetLineWidth(2);
        ofSetColor(ofColor::white);
        int itr = 0;
        for(int i = 0; i<RES_W;i+=240){
            ofDrawLine(i, 0, i, RES_H);
            ofDrawBitmapStringHighlight(ofToString(itr)+ " m", i+10, 10);
            itr++;
        }
    }
    finalRender.end();
    
    ofFill();

    if(finalRender.isAllocated())syphon.publishTexture(&finalRender.getTexture());

}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(),0));

    if( drawGui )
    {
         gui.draw();
    }

    for(int i=0; i < rCounters.size();i++){
        if(rCounters[i]>200)ofSetColor(255, 0, 0);
        else ofSetColor(0, 255, 0);
        ofDrawRectangle(gui.getWidth()+30, 20 * i + 20, 18, 18);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='d')bDebug = !bDebug;
    if(key=='g')drawGui = !drawGui;


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


