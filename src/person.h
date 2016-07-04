//
//  person.h
//  blueParticles_Fazer
//
//  Created by Karina Jensen on 04/07/16.
//
//

#ifndef person_h
#define person_h

class Person{
public:
    ofPolyline poly;
    ofPoint centroid;
    vector<ofPoint>points;
    int area;
    int idNum;
    int age;
    
    void draw(){
        for(int i = 0; i<points.size();i++)ofDrawCircle(points[i],4);
    }
};


#endif /* person_h */
