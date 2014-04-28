//
//  ofLevelScene.h
//  ludumdare29
//
//  Created by Alex on 4/25/14.
//
//

#ifndef __ludumdare29__ofLevelScene__
#define __ludumdare29__ofLevelScene__

#include "ofxAppUtils.h"
#include "ofxAssimpModelLoader.h"
#include "ofxSpriteSheetRenderer.h"

struct basicSprite {
	animation_t animation;
	ofPoint pos;
	ofPoint velocity;
    float speed;
    flipDirection flipDirection;
};

class ofLevelScene : public ofxScene
{
public:
    ofLevelScene();
    ofLevelScene(std::string name);
    
    void draw();
    void setup();
    void update();
    
    void updateEnter();
    void updateExit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed( int x, int y, int button);
    
    ofEasyCam cam;
    ofLight light;
    ofxAssimpModelLoader model;
    
    ofxSpriteSheetRenderer *spriteRenderer;
    ofxSpriteSheetRenderer *backgroundRenderer;
    ofxSpriteSheetRenderer *foregroundRenderer;
    
    vector<basicSprite *> sprites;
    
    basicSprite *player;
    basicSprite *background;
};

#endif /* defined(__ludumdare29__ofLevelScene__) */
