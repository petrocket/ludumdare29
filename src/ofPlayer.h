//
//  ofPlayer.h
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#ifndef __ludumdare29__ofPlayer__
#define __ludumdare29__ofPlayer__

#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

class ofPlayer
{
public:
    ofPlayer();
    
    void draw();
    void update();
    
    ofVec3f position;
    ofVec2f velocity;
    ofxAssimpModelLoader model;
    
    ofVec2f speed;
    bool swimming;
    int swimmingDirection;
    
    float health; // 0 to 100.f
};

#endif /* defined(__ludumdare29__ofPlayer__) */
