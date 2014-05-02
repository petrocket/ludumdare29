//
//  ofTreasureChest.cpp
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#include "ofTreasureChest.h"

ofTreasureChest::ofTreasureChest() : ofxAssimpModelLoader()
{
    
}

ofTreasureChest::ofTreasureChest(ofVec3f position) :
    ofxAssimpModelLoader(),
    isOpen(false),
    loaded(false),
    position(position)
{

}

void ofTreasureChest::update()
{
    if(this->getNumMeshes() > 0) ofxAssimpModelLoader::update();
}

void ofTreasureChest::draw()
{
    if(!loaded) {
        // prevent crash? too many objects??        
        if(loadModel("chest.dae")) {
            setScale(0.002, 0.002, 0.002);
            setRotation(0, -90, 1, 0, 0);
            setPausedForAllAnimations(true);
            loaded = true;
        }
    }
    
    if(!loaded) return;
    
    ofPushMatrix();
    ofTranslate(position);
    ofRotateY(170);
    ofRotateX(20);
    if(getNumMeshes() > 0) drawFaces();
    ofPopMatrix();
}

void ofTreasureChest::open()
{
    if(isOpen) return;
    
    isOpen = true;
    playAllAnimations();
    setLoopStateForAllAnimations(OF_LOOP_NONE);
}