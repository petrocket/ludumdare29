//
//  ofGameCamera.cpp
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#include "ofxGameCamera.h"

static float ClampAngle (float angle, float min, float max) {
	if (angle < -360.0f)
		angle += 360.f;
	if (angle > 360.0f)
		angle -= 360.0f;
	return ofClamp(angle, min, max);
}


ofxGameCamera::ofxGameCamera() {
	dampen = false;
	unsavedChanges = false;
	sensitivityX = 0.15f;
	sensitivityY = 0.15f;
    
	minimumX = -360.0f;
	maximumX =  360.0f;
    
	minimumY = -60.0f;
	maximumY =  60.0f;
    
	speed = 1.0f;
	
	lastMouse = ofVec2f(0,0);
	
	invertControls = false;
	usemouse = true;
	useArrowKeys = false;
    
	applyRotation = true;
	applyTranslation = true;
    
	rollSpeed = 1.0f;
	justResetAngles = false;
    
	
	cameraPositionFile =  "_gameCameraPosition.xml";
	
    eventsRegistered = false;
}
ofxGameCamera::~ofxGameCamera(){
    if(eventsRegistered){
        ofAddListener(ofEvents().update, this, &ofxGameCamera::update);
        ofAddListener(ofEvents().keyPressed, this, &ofxGameCamera::keyPressed);
        eventsRegistered = false;
    }
}

void ofxGameCamera::setup(){
    if(!eventsRegistered){
        ofAddListener(ofEvents().update, this, &ofxGameCamera::update);
        ofAddListener(ofEvents().keyPressed, this, &ofxGameCamera::keyPressed);
        eventsRegistered = true;
    }
}

void ofxGameCamera::update(ofEventArgs& args){
	bool rotationChanged = false;
	bool positionChanged = false;
	
	//lastRot == lastRot << this is nananers check
	if( (lastRot == lastRot && lastRot != getOrientationQuat()) ||
       (lastPos == lastPos && lastPos != getPosition()))
	{
		movedManually();
	}
    
	if(applyTranslation){
		int multiplier = invertControls ? -1 : 1;
		if(ofGetKeyPressed('w') || (useArrowKeys && ofGetKeyPressed(OF_KEY_UP)) ){
			dolly(-speed * ofGetLastFrameTime());
			positionChanged = true;
		}
		
		if(ofGetKeyPressed('s') || (useArrowKeys && ofGetKeyPressed(OF_KEY_DOWN)) ){
			dolly(speed * ofGetLastFrameTime());
			positionChanged = true;
		}
		
		if(ofGetKeyPressed('a') || (useArrowKeys && ofGetKeyPressed(OF_KEY_LEFT)) ){
			truck(-speed* ofGetLastFrameTime());
			positionChanged = true;
		}
		
		if(ofGetKeyPressed('d') || (useArrowKeys && ofGetKeyPressed(OF_KEY_RIGHT)) ){
			truck(speed* ofGetLastFrameTime());
			positionChanged = true;
		}
		
		if(ofGetKeyPressed('x') || (useArrowKeys && ofGetKeyPressed(OF_KEY_PAGE_DOWN)) ){
			boom(-speed*multiplier* ofGetLastFrameTime());
			positionChanged = true;
		}
		
		if(ofGetKeyPressed('z') || (useArrowKeys && ofGetKeyPressed(OF_KEY_PAGE_UP)) ){
			boom(speed*multiplier* ofGetLastFrameTime());
			positionChanged = true;
		}
	}
	
    
	if(positionChanged){
		currLookTarget = getPosition() + getLookAtDir();
	}
    
    
	ofVec2f mouse( ofGetMouseX(), ofGetMouseY() );
	if(!justResetAngles){
		if(usemouse && applyRotation && ofGetMousePressed(0)){
            
			float dx = (mouse.x - lastMouse.x) * sensitivityX;
			float dy = (mouse.y - lastMouse.y) * sensitivityY;
            
			currLookTarget.rotate(-dx, getPosition(), currentUp);
			ofVec3f sideVec = (currentUp).getCrossed(currLookTarget - getPosition());
			lookAt(currLookTarget, currentUp);
            
			currLookTarget.rotate(dy, getPosition(), sideVec);
			currentUp = ( currLookTarget - getPosition()  ).getCrossed(sideVec);
			lookAt(currLookTarget, currentUp);
            
			rotationChanged = true;
            
		}
        
		if(applyRotation){
			if(ofGetKeyPressed('e')){
				currentUp.rotate(rollSpeed* ofGetLastFrameTime(),getLookAtDir());
				lookAt(currLookTarget, currentUp);
				rotationChanged = true;
			}
			if(ofGetKeyPressed('q')){
				currentUp.rotate(-rollSpeed* ofGetLastFrameTime(),getLookAtDir());
				lookAt(currLookTarget, currentUp);
				rotationChanged = true;
			}
		}
	}
    
	lastMouse = mouse;
	justResetAngles = false;
	lastRot = getOrientationQuat();
	lastPos = getPosition();
    
	//did we make a change?
	unsavedChanges |= positionChanged || rotationChanged;
}

void ofxGameCamera::keyPressed(ofKeyEventArgs& args){
	
}

void ofxGameCamera::movedManually()
{
	currLookTarget = getPosition() + getLookAtDir();
	currentUp = getUpDir();
	justResetAngles = true;
}



void ofxGameCamera::reset(){
    
	currentUp = ofVec3f(0,1,0);
	currLookTarget = ofVec3f(0,0,1);
    
    setPosition(ofVec3f(0,0,0));
    setOrientation(ofQuaternion());
	
	movedManually();
}
