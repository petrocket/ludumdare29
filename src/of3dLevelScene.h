//
//  of3dLevelScene.h
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#ifndef __ludumdare29__of3dLevelScene__
#define __ludumdare29__of3dLevelScene__

#include "ofxAppUtils.h"
#include "ofxAssimpModelLoader.h"
#include "ofPlayer.h"
#include "ofTreasureChest.h"
#include "ofPuffer.h"
#include "ofxGameCamera.h"
#include "ofxPostProcessing.h"
#include "ofxRayTriangleIntersection.h"
#include "ofxUI.h"

class of3dLevelScene : public ofxScene
{
public:
    of3dLevelScene();
    of3dLevelScene(std::string name);
    
    void draw();
    void setup();
    void update();
    
    void updateEnter();
    void updateExit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed( int x, int y, int button);
    
    void load();
    void save();
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofxGameCamera cam;
    ofxPostProcessing post;
    ofLight light;
    
    // level
    ofxAssimpModelLoader level;
    ofxAssimpModelLoader collision;
    ofxRayTriangleIntersection hitTest;
    bool gameOver;

    // game objects
    ofPlayer *player;
    vector<ofTreasureChest *> chests;
    vector<ofPuffer *> enemies;
    
    // ui
    ofxUICanvas *gui;
    ofxUIImageButton *okButton;
    ofxUIImage *gameOverImage;
    ofImage *gameOverWin;
    ofImage *gameOverLose;
    
    ofSoundPlayer backgroundMusic;
    
    // edit mode
    bool editMode;
    std::string currentObject;
    ofVec3f startPos;
    
    // effects
    shared_ptr<ConvolutionPass> convolutionPass;
    
private:
    void setupCollision();
    
    vector<FaceTri> collisionFaces;
};

#endif /* defined(__ludumdare29__of3dLevelScene__) */
