//
//  ofGameCamera.h
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#ifndef __ludumdare29__ofGameCamera__
#define __ludumdare29__ofGameCamera__

#include "ofMain.h"

class ofxGameCamera : public ofCamera {
public:
	
	ofxGameCamera();
    ~ofxGameCamera();
    
	void setup();
    void update(ofEventArgs& args);
    void keyPressed(ofKeyEventArgs& args);
	
	//tweak controls
	bool invertControls;
	float sensitivityX;
	float sensitivityY;
	float rollSpeed;
	float speed;
	
	//enable, disable
	bool usemouse;
	bool useArrowKeys;
	bool applyRotation;
	bool applyTranslation;
	   
	void movedManually(); //call this whenever you update orientation or position manually
    void reset(); //sets the camera back to zero
	
	float minimumX;
	float maximumX;
	
	float minimumY;
	float maximumY;
	
	bool dampen;
protected:
    
	bool eventsRegistered;
    
	ofVec3f currentUp;
	ofVec3f currLookTarget;
	
	ofQuaternion lastRot;
	ofVec3f lastPos;
    
	void updateRotation();
    
	ofVec2f lastMouse;
	bool justResetAngles;
	bool unsavedChanges;
	
	string cameraPositionFile;
	
};

#endif /* defined(__ludumdare29__ofGameCamera__) */
