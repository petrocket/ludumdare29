//
//  ofTreasureChest.h
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#ifndef __ludumdare29__ofTreasureChest__
#define __ludumdare29__ofTreasureChest__

#include "ofxAssimpModelLoader.h"

class ofTreasureChest : public ofxAssimpModelLoader
{
public:
    ofTreasureChest();
    ofTreasureChest(ofVec3f position);
    
    void update();
    void draw();
    void open();
    
    bool isOpen;
    ofVec3f position;
    bool loaded;
};

#endif /* defined(__ludumdare29__ofTreasureChest__) */
