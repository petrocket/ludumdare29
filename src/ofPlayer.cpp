//
//  ofPlayer.cpp
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#include "ofPlayer.h"


ofPlayer::ofPlayer() :
    health(100.f),
    swimmingDirection(1),
    swimming(false),
    position(0,5.f,0.f),
    speed(4.f,4.f)
{
    if(model.loadModel("robot.dae")) {
        model.setRotation(0, -90, 1, 0, 0);
        model.setScale(0.0015, 0.0015, 0.0015);
        model.playAllAnimations();
        model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
        model.getAnimation(0).setSpeed(0.3);
    }
}

void ofPlayer::update()
{
    if(health > 0.f) {
        if(ofGetKeyPressed('d') || ofGetKeyPressed(OF_KEY_RIGHT)) {
            swimmingDirection = 1;
            swimming = true;
            velocity.x = -speed.x;
        }
        else if(ofGetKeyPressed('a') || ofGetKeyPressed(OF_KEY_LEFT)) {
            swimmingDirection = -1;
            swimming = true;
            velocity.x = speed.x;
        }
        else {
            velocity.x = 0.f;
            swimming = false;
        }
        
        if(ofGetKeyPressed('w') || ofGetKeyPressed(OF_KEY_UP)) {
            swimming = true;
            velocity.y = speed.y;
        }
        else if(ofGetKeyPressed('s') || ofGetKeyPressed(OF_KEY_DOWN)) {
            swimming = true;
            velocity.y = -speed.y;
        }
        else {
            velocity.y = 0.f;
        }
    }
    else {
        velocity.x = 0.f;
        if(position.y > -100.f) {
            velocity.y = -4.f;
        }
        else {
            velocity.y = 0.f;
        }
    }
    
    position += velocity * ofGetLastFrameTime();
    if(model.getNumMeshes()) {
        model.update();
        float pos = model.getAnimation(0).getPosition();
        if(swimming) {
            model.getAnimation(0).setSpeed(1.0);
            // swimming is last 16 frames of 32 frame anim
            if(pos < 16.5f/32.f) {
                pos += 16.5f/32.f;
            }
        }
        else {
            model.getAnimation(0).setSpeed(0.3);
            // idle is the first 16 frames
            pos = fmodf(pos, 15.f/32.f);
        }
        model.getAnimation(0).setPosition(pos);
        model.update();
    }
}

void ofPlayer::draw()
{
    ofPushMatrix();
    ofTranslate(position);
    if(swimmingDirection == -1) {
        ofRotateY(180);
    }
    if(health <= 0.f) {
        ofRotateX(180);
    }
    if(model.getNumMeshes()) model.drawFaces();
    ofPopMatrix();
}