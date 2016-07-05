//
//  animalPng.h
//  blueParticles_Fazer
//
//  Created by FazerPro on 05/07/16.
//
//



#ifndef animalPng_h
#define animalPng_h


class AnimalPng{
    
public:

    float sequenceFPS;
    bool  bFrameIndependent;
    vector <ofImage> images;
    
    ofVec2f pos;
    int w,h;
    bool playing;
    void setup(string _path, ofVec2f _pos){
        
        ofDirectory dir;
        
        int nFiles = dir.listDir(_path);
        if(nFiles) {
            
            for(int i=0; i<dir.size(); i++) {
                
                // add the image to the vector
                string filePath = dir.getPath(i);
                images.push_back(ofImage());
                images.back().load(filePath);
                w = images.back().getWidth();
            }
            
        }
        else ofLog(OF_LOG_WARNING) << "Could not find folder";
        
        // this toggle will tell the sequence
        // be be indepent of the app fps
        bFrameIndependent = true;
        
        // this will set the speed to play
        // the animation back we set the
        // default to 24fps
        sequenceFPS = 24;
        


    }
    
    void update(){

    }
    
    void draw(ofVec2f _pos, int r){
        
        if((int)images.size() <= 0) {
            ofSetColor(255);
            ofDrawBitmapString("No Images...", 150, ofGetHeight()/2);
            return;
        }
        
        // this is the total time of the animation based on fps
        //float totalTime = images.size() / sequenceFPS;
        
        uint64_t frameIndex = 0;
        
        if(bFrameIndependent) {
            // calculate the frame index based on the app time
            // and the desired sequence fps. then mod to wrap
            frameIndex = (int)(ofGetElapsedTimef() * sequenceFPS) % images.size();
        }
        float s = r*1.5;
        images[frameIndex].draw(_pos.x-s/2,_pos.y-s/2, s, s);
    }
    


};
#endif /* animalPng_h */



