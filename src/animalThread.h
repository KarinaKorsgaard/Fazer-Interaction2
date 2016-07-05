//
//  animalThread.h
//  blueParticles_Fazer
//
//  Created by FazerPro on 04/07/16.
//
//

#ifndef animalThread_h
#define animalThread_h

#include "animal.h"

class MyThread : public ofThread {
public:
    vector<Animal>*animals;
private:
    // the thread function
    void threadedFunction() {
        
        // start
        
        while(isThreadRunning()) {
            
            for(int i = 0; i<animals->size();i++)animals->at(i).update();
        }
        
        // done
    }
    
    
};

#endif /* animalThread_h */
