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
    vector<ofImage>images;
    vector<ofPixels>imagesPix;
    int totalNumFrames=0;
    float thisFrame = 0;
    ofVec2f pos;
    int w,h;
    
    void setup(string _path, ofVec2f _pos){
        string path = _path;
        ofDirectory dir(path);
        
        dir.allowExt("png");
        dir.listDir();
        
        for(int i = 0; i<dir.size();i++){
            ofPixels pix = *new ofPixels;
            ofLoadImage(pix,dir.getPath(i));
            imagesPix.push_back(pix);
            totalNumFrames++;
        }
        totalNumFrames--;
        if(images.size()>0){
            w = images[0].getWidth();
            h= images[0].getHeight();
        }
    }
    
    void update(){
        
        thisFrame=int(thisFrame+1)%((totalNumFrames*2));
    }
    
    void draw(){
        //imagesPix[int(thisFrame/2)].draw(pos, w,h);
    }
    


};
#endif /* animalPng_h */
