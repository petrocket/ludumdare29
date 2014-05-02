//
//  ofPuffer.cpp
//  ludumdare29
//
//  Created by Alex on 4/27/14.
//
//

#include "ofPuffer.h"

ofPuffer::ofPuffer() : ofxAssimpModelLoader()
{
    
}

ofPuffer::ofPuffer(ofVec3f position) :
ofxAssimpModelLoader(),
totalPatrolTime(30.f),
patrolRadius(3.f),
loaded(false),
rotation(90.f),
isOpen(false),
position(position),
patrolCenter(position)
{
    // causes crash!
//    if(loadModel("puffer.dae")) {
//        setScale(0.0015, 0.0015, 0.0015);
//        setRotation(0, 180, 0, 0, 1);
//        playAllAnimations();
//    }
}

void ofPuffer::update()
{
    if(!loaded) {
        
        return;
    }
    
    if(this->getNumMeshes() > 0) {
        ofxAssimpModelLoader::update();
    }
    
    float elapsedTime = ofGetElapsedTimef() - floorf(ofGetElapsedTimef() / totalPatrolTime) * totalPatrolTime;
    float currentPatrolProgress = elapsedTime / totalPatrolTime;
    float lastX = position.x;
    position.x = patrolCenter.x + (sin(currentPatrolProgress * PI * 2.f) * patrolRadius);
    rotation = position.x > lastX ? 90 : -90;
}

void ofPuffer::draw()
{
    if(!loaded) {
        // prevent crash? too many objects??
        if(loadModel("puffer.dae")) {
            setScale(0.0015, 0.0015, 0.0015);
            setRotation(0, 180, 0, 0, 1);
            loaded = true;
//            playAllAnimations();
        }
    }
    
    if(!loaded) return;
    ofPushMatrix();
    ofTranslate(position);
    ofRotateY(rotation);
    if(getNumMeshes() > 0) drawFaces();
    ofPopMatrix();
}