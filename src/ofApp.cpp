#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    group.setName("FazerParticles");
    parameters.setName("offsetControl");
    ofParameterGroup aligning;
    aligning.add(scale1.set("scale1",1,0.5,2.5));
    aligning.add(scale2.set("scale1",1,0.5,2.5));
    ofParameterGroup one,two,three,four;
    one.setName("one");
    two.setName("two");
    three.setName("three");
    four.setName("four");
    
    one.add(offSet1X.set("offSet1X",0,RES_W,-100));
    one.add(offSet1Y.set("offSet1Y",0,-500,500));
    
    // this overlap is always 0.
    two.add(offSet2X.set("offSet2X",0,RES_W,-100));
    two.add(offSet2Y.set("offSet2Y",0,-900,500));
   // two.add(scale2.set("scale2",0,3,0));
    
    two.add(overLap1.set("overLap1",0,-700,700));
   
    three.add(offSet3X.set("offSet3X",0,RES_W,-100));
    three.add(offSet3Y.set("offSet3Y",0,-900,500));
  //  three.add(scale3.set("scale3",0,3,0));
    
    three.add(overLap2.set("overLap2",0,-700,700));
    
    four.add(offSet4X.set("offSet4X",0,RES_W,-100));
    four.add(offSet4Y.set("offSet4Y",0,-900,500));
   // four.add(scale4.set("scale4",0,3,0));
    
    four.add(overLap3.set("overLap3",0,-700,700));
    aligning.add(numAttractionP.set("numAttraction",5,1,60));
  //  aligning.add(useInsidePoly.set("useInsidePoly", true));
    
    aligning.add(one);
    aligning.add(two);
    aligning.add(three);
    aligning.add(four);
    
    visualControl.setName("visuals");
    visualControl.add(swarm.set("swarm", true));
    visualControl.add(drawAnimals.set("drawAnimals", true));
    visualControl.add(cluster.set("clusters", true));
    visualControl.add(cAttraction.set("cAttraction",0,0,1));
    visualControl.add(pRepulsion.set("pRepulsion",0,0,1));
    visualControl.add(blend_ADD.set("blend_ADD", true));
    visualControl.add(oldApp.set("evenSizes", true));
    visualControl.add(fluidcolor.set("all same color", true));
    visualControl.add(blurImg.set("blurImg", true));
    visualControl.add(evenColor.set("uniform Color swarm", true));
    
    visualControl.add(blurImg1.set("blurImg cluster", true));
    visualControl.add(evenColor1.set("uniform Color cluster", true));
    
    visualControl.add(sNear.set("sNear",0,1,200));
    visualControl.add(sFar.set("sFar",0,1,200));
    visualControl.add(b2dRepulsion.set("b2dRepulsion",0,1,200));

    
    visualControl.add(density.set("density",0,0,2));
    visualControl.add(bounce.set("bounce",0,0,2));
    visualControl.add(friction.set("friction",0,0,2));
  
    
    ofParameterGroup colorControl;
    colorControl.add(clusterRange1.set("background",ofColor(255,255), ofColor(0,0),ofColor(255)));
    colorControl.add(swarmColor.set("swarmnBox2dColor",ofColor(255,255), ofColor(0,0),ofColor(255)));
    colorControl.add(clusterRange2.set("cluster color",ofColor(255,255), ofColor(0,0),ofColor(255)));
    
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

    
    pointSplineFbo.allocate(RES_W,RES_H);
    pointSplineFbo.begin();
    ofClear(0);
    pointSplineFbo.end();
    
    finalRender.allocate(RES_W,RES_H);
    finalRender.begin();
    ofClear(0);
    finalRender.end();
    

    sparkImg.allocate(128,128, GL_RGBA);
    ofDisableArbTex();
    ofLoadImage(sparkImg, "dot.png");
    ofEnableArbTex();
    
    solid.allocate(128,128, GL_RGBA);
    ofDisableArbTex();
    ofLoadImage(solid, "clear.png");
    ofEnableArbTex();
    
    pointSpline.load("shader/shader");
 
    ofImage fCol;
    fCol.load("fazerColors.png");
    int stripeW = fCol.getWidth() / 5;
    for(int i = stripeW/2; i< fCol.getWidth();i+=stripeW){
        ofColor c = fCol.getColor(i,stripeW);
        fazerColors.push_back(c);
        
    }
    cout << fazerColors.size()<<endl;

    //ofSetCircleResolution(100);
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.createGround(0, RES_H, RES_W, RES_H);
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    
    int num = SWARM_NUM;
    ofMesh colorMesh;
    int cIndx = 1;
    int fours = 0;
    
    for(unsigned int i = 0; i < num; i++){
        fours = (i*4) / num;
        customParticles.push_back(shared_ptr<CustomParticle>(new CustomParticle));
        CustomParticle * p = customParticles.back().get();
        float r = ofRandom(8, 45);		// a random radius 4px - 20px
        r = ofRandom(8,r);
        //float density, float bounce, float friction
        p->setPhysics(ofRandom(0.1,5), ofRandom(0.1,0.5) ,0);
        p->setup(box2d.getWorld(), ofRandom((fours-1)*RES_W/4,fours*RES_W/4), ofRandom(100,RES_H-100), r);
        p->radius=p->getRadius();
        p->setVelocity(ofRandom(-0.5,0.5), ofRandom(-0.5,0.5));
        p->attractionPoint = ofVec2f(fours*RES_W/4 +RES_W/8, RES_H/2);
        p->num =fours;
        ///pointSizes.push_back(r);
        
        colorMesh.addVertex(ofVec3f(fazerColors[cIndx].r,fazerColors[cIndx].g,fazerColors[cIndx].b));
        cIndx = (i%(fazerColors.size()-1)) +1;
        
    }

    for(int i = 0; i<7;i++){
        for(int u = 0; u<3;u++){
            movingSounds.push_back(shared_ptr<MovingSoundParticle>(new MovingSoundParticle));
            MovingSoundParticle * p = movingSounds.back().get();
            float r = ofRandom(30,55);		// a random radius 4px - 20px
            p->setPhysics(ofRandom(0.1,5), ofRandom(0.1,0.5) ,0);
            p->setup(box2d.getWorld(), ofRandom(RES_W), ofRandom(RES_H), r);
            p->radius=p->getRadius();
            p->setVelocity(ofRandom(-0.5,0.5), ofRandom(-0.5,0.5));
            p->attractionPoint = ofVec2f(RES_W/2,RES_H/2);
            p->num = 0;
            p->name = "/"+ofToString(i)+"/"+ofToString(u);
            colorMesh.addVertex(ofVec3f(fazerColors[0].r,fazerColors[0].g,fazerColors[0].b));
        }
    }
    int total = SWARM_NUM+movingSounds.size();
    
    vbo.setNormalData(&colorMesh.getVertices()[0], (int)total, GL_STATIC_DRAW);
    

    colorMesh.clear();
    cIndx = 0;
    //cluster / binned sys
    for(int i = 0 ; i<CLUSTER_NUM; i++){
        int size =ofRandom(20,100);
        int num = ofRandom(50,80);
        binnedSystem b;
        clusters.push_back(b);
        clusters.back().setup(ofVec2f((RES_W/(CLUSTER_NUM+1))*(i+1),ofRandom(100, RES_H-100)),num, size);
        clusters.back().people = &people;
        for(int u = 0; u< num;u++){
            colorMesh.addVertex(ofVec3f(fazerColors[cIndx].r,fazerColors[cIndx].g,fazerColors[cIndx].b));
            cIndx = u%fazerColors.size();
            clusters.back().pSizes.push_back(ofRandom(20,size));
            
        }
        clusters.back().vbo.setNormalData(&colorMesh.getVertices()[0], (int)num, GL_STATIC_DRAW);
    }
    
//    string path = "animals";
//    ofDirectory dir(path);
//
//    dir.allowExt("mov");
//    dir.listDir();
//    
//    for(int i = 0; i<dir.size();i+=2){
//        Animal ani = *new Animal;
//        ani.setup(ofVec2f(ofRandom(100,RES_W-100),ofRandom(100,RES_H-100)),"/ani/"+ofToString(int(i/2),0),dir.getPath(i+1),dir.getPath(i),&people);
//        //if(i<1)ani.track=true;
//        animals.push_back(ani);
//    }
    
    string path = "animals";
    ofDirectory dir(path);
    
    //dir.allowExt("mov");
    dir.listDir();
    
    for(int i = 0; i<dir.size();i++){
        AnimalPng ani = *new AnimalPng;
        ani.setup(dir.getPath(i),ofVec2f(200,200));
        //if(i<1)ani.track=true;
        animals.push_back(ani);
        
        cout<<dir.getPath(i);
    }

    soundSender.setup("localhost",3000);
    drawGui = true;
    bDebug = true;
    attractPoints.resize(4);
    rCounters.resize(4);
    syphon.setName("FazerParticles");
   // ofSetFrameRate(30);
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){


    vector<float>ofsetlistX;
    ofsetlistX={offSet1X,offSet2X,offSet3X,offSet4X};
    
    vector<float>ofsetlistY;
    ofsetlistY={offSet1Y,offSet2Y,offSet3Y,offSet4Y};
    
    vector<float>scaleList;
    scaleList={scale1,scale1,scale1,scale1};
    


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
                        pt.y= ofMap(msg.getArgAsFloat(i+1),0,512,0,RES_H*scale2) + ofsetlistY[r];;
                        blobs[r][pointCloudIndx].addVertex(pt);
                        
                    }
                    if(blobs[r][pointCloudIndx].size()>0) blobs[r][pointCloudIndx].addVertex(blobs[r][pointCloudIndx].getVertices()[0]);
                    blobs[r][pointCloudIndx]=blobs[r][pointCloudIndx].getResampledBySpacing(numAttractionP);

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
    

    
    people.clear();
    for(int i = 0; i < blobs.size();i++){
        for(int u = 0; u< blobs[i].size();u++){
            Person karina = *new Person;
            ofPolyline p=blobs[i][u];
            vector<ofPoint>points;
            if(p.size()>0){
                points.push_back(p.getCentroid2D());
                for( int pt = 0; pt < p.getVertices().size(); pt++) {
                    points.push_back(p.getVertices().at(pt));
                }
            }
            karina.centroid = blobs[i][u].getCentroid2D();
            karina.poly = p;
          //  karina.area = int(ofMap(karina.centroid.x, 0, RES_W, 0, 4));
            karina.points = points;
            people.push_back(karina);
        }
    }

    
    
    if(drawAnimals){
        for(int i = 0 ; i< animals.size();i++){
            animals[i].update();

        }
    }
    
    if(swarm){
        
        mesh.clear();
        for(int i = 0; i<customParticles.size();i++){
           // int cNum =customParticles[i]->num;
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

            
                for(int u = 0; u<people.size();u++){
                    bool found = false;
                    for(int p = 0; p<people[u].points.size();p++){
                        if(!found){
                            int x2 = people[u].points[p].x;
                            int y2 = people[u].points[p].y;
                            int dist =1/b2InvSqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                            if(dist<sNear){
                                found = true;
                                movingSounds[i]->animateRadius=true;
                                movingSounds[i]->toggle = true;
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
    
        int total = SWARM_NUM+movingSounds.size();
        box2d.update();
        vbo.setVertexData(&mesh.getVertices()[0], total, GL_STATIC_DRAW);
    }

    
    
    if(cluster){
        //int red = ((sin(ofGetElapsedTimef()/10))+1) * 160/2;
        //clusterRange2.set(ofColor(red,clusterRange2->g,160-red));
        //swarmColor.set(ofColor(160-red,red,swarmColor->b));
        
        for(int i = 0 ; i< clusters.size();i++){
            clusters[i].update();
            clusters[i].oldApp = oldApp;
            if(people.size()==0)  clusters[i].centerAttraction = 0;
            else clusters[i].centerAttraction = cAttraction;
        }
    }


    //pointspline replacement
    if(cluster||swarm||bBox2d){
        pointSplineFbo.begin();
        ofClear(0, 0);
        ofEnableAlphaBlending();
        
       // if(drawAnimals)for(auto a: animals)a.draw();
        
        pointSpline.begin();
        
        glDepthMask(GL_FALSE);
        ofEnablePointSprites();
        if(blend_ADD){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
        }
        if(cluster){
            int cIndex = 0;
            for(int i = 0 ; i< clusters.size();i++){
                cIndex = i%fazerColors.size();
                
                if(evenColor1)pointSpline.setUniform1f("evenColor", 1.);
                else pointSpline.setUniform1f("evenColor", 0.);
                
                if(!evenColor1)pointSpline.setUniform3f("col", float(fazerColors[cIndex].r), float(fazerColors[cIndex].g), float(fazerColors[cIndex].b));
                
                if(blurImg1)pointSpline.setUniform1f("addthis", 7.5);
                else pointSpline.setUniform1f("addthis", 2.);
                
                if(fluidcolor)pointSpline.setUniform3f("col", float(clusterRange2->r+clusters[i].r), float(clusterRange2->g+clusters[i].g), float(clusterRange2->b+clusters[i].b));
                
                if(blurImg1)sparkImg.bind();
                else solid.bind();
                clusters[i].vbo.draw(GL_POINTS, 0, (int)clusters[i].particleSystem.size());
                if(blurImg1)sparkImg.unbind();
                else solid.unbind();
            }
        }

        ofDisableBlendMode();
        ofEnableAlphaBlending();

        if(swarm){
            if(evenColor)pointSpline.setUniform1f("evenColor", 1.);
            else pointSpline.setUniform1f("evenColor", 0.);
            
            if(blurImg)pointSpline.setUniform1f("addthis", 7.5);
            else pointSpline.setUniform1f("addthis", 2.);
            
            int cIndex = 0;
            if(!fluidcolor)pointSpline.setUniform3f("col", float(swarmColor->r), float(swarmColor->g), float(swarmColor->b));
            if(fluidcolor)pointSpline.setUniform3f("col", float(clusterRange2->r), float(clusterRange2->g), float(clusterRange2->b));
            
            if(!blurImg) solid.bind();
            else sparkImg.bind();
            vbo.draw(GL_POINTS, 0, int(SWARM_NUM+movingSounds.size()));
            if(!blurImg) solid.unbind();
            else sparkImg.unbind();
            
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
        
        pointSplineFbo.end();

        
        
        
        ofDisableBlendMode();

        


    }
    
    
    finalRender.begin();
    ofClear(0);
    ofSetColor(255);
    ofEnableAlphaBlending();

    if(cluster||swarm){
//    ofSetColor(255);
//    textureShader.begin();
//    textureShader.setUniformTexture("tex0", texture, 0);
//    textureShader.setUniformTexture("tex1", pointSplineFbo.getTexture(), 1);
    pointSplineFbo.draw(0,0);
//    textureShader.end();
    }
    if(drawAnimals){
        int itr = 0;
        for(int i = 0; i<movingSounds.size();i++){
            animals[itr].draw(movingSounds[i]->getPosition(),movingSounds[i]->getRadius());
          //  movingSounds[i]->draw(animals[itr].imagesPix[animals[itr].thisFrame/2]);
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
//        ofSetColor(255,0,0);
//        ofDrawRectangle(RES_W-k2 - (k3-k2), 10, k3-k2, RES_H-10);
        
         k1 = offSet2X + ((RES_W/4)*scale1 /2);
         k2 = offSet2X;
         k3 =(RES_W/4)*scale1 + offSet2X;
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);
//        ofSetColor(255,0,0);
//        ofDrawRectangle(RES_W-k2 - (k3-k2), 10, k3-k2, RES_H-10);
        
         k1 = offSet3X + ((RES_W/4)*scale1 /2);
         k2 = offSet3X;
         k3 =(RES_W/4)*scale1 + offSet3X;
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);
//        ofSetColor(255,0,0);
//        ofDrawRectangle(RES_W-k2 - (k3-k2), 10, k3-k2, RES_H-10);
        
   
        
         k1 = offSet4X + ((RES_W/4)*scale1 /2);
         k2 = offSet4X;
         k3 =(RES_W/4)*scale1 + offSet4X;
        
        ofNoFill();
        ofSetColor(ofColor::green);
        ofDrawLine(RES_W-k1,0,RES_W-k1,RES_H);
//        ofSetColor(255,0,0);
//        ofDrawRectangle(RES_W-k2 - (k3-k2), 10, k3-k2, RES_H-10);
        //(RES_W-k1, 0, RES_W-k2, RES_H, RES_W-k3, RES_H);
        
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


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='d')bDebug = !bDebug;
    if(key=='g')drawGui = !drawGui;
    if(key=='l'){soudoLine=!soudoLine;
        if(!soudoLine){
            blobs[0][0].clear();
            blobs[0][1].clear();
        }
    }


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


