//
//  ofPuffer.h
//  ludumdare29
//
//  Created by Alex on 4/27/14.
//
//

#ifndef __ludumdare29__ofPuffer__
#define __ludumdare29__ofPuffer__

#include "ofxAssimpModelLoader.h"

class ofPuffer : public ofxAssimpModelLoader
{
public:
    ofPuffer();
    ofPuffer(ofVec3f position);
    
    void update();
    void draw();
    void open();
    
    std::string getType() { return "puffer"; }

    bool isOpen;
    ofVec3f position;
    
    ofVec3f patrolCenter;
    float patrolRadius;
    float totalPatrolTime; // in seconds
    float rotation;
};

#endif /* defined(__ludumdare29__ofPuffer__) */
