//
//  ofLevelScene.cpp
//  ludumdare29
//
//  Created by Alex on 4/25/14.
//
//

#include "ofLevelScene.h"

static animation_t idleAnimation =
{	0,	/* .index			(int) - this is the index of the first animation frame. indicies start at 0 and go left to right, top to bottom by tileWidth on the spriteSheet		*/
	0,	/* .frame			(int) - this is the current frame. It's an internal variable and should always be set to 0 at init													*/
	4,	/* .totalframes		(int) - the animation length in frames																												*/
	1,	/* .width			(int) - the width of each animation frame in tiles																									*/
	1,	/* .height			(int) - the height of each animation frame in tiles																									*/
	180,	/* .frameduration	(unsigned int) - how many milliseconds each frame should be on screen. Less = faster																*/
	0,	/* .nexttick		(unsigned int) - the next time the frame should change. based on frame duration. This is an internal variable and should always be set to 0 at init	*/
	-1,	/* .loops			(int) - the number of loops to run. -1 equals infinite. This can be adjusted at runtime to pause animations, etc.									*/
	-1,	/* .finalindex		(int) - the index of the final tile to draw when the animation finishes. -1 is the default so total_frames-1 will be the last frame.				*/
	1	/* .frameskip		(int) - the incrementation of each frame. 1 should be set by default. If you wanted the animation to play backwards you could set this to -1, etc.	*/
};

static animation_t swimAnimation =
{	4,	/* .index			(int) - this is the index of the first animation frame. indicies start at 0 and go left to right, top to bottom by tileWidth on the spriteSheet		*/
	0,	/* .frame			(int) - this is the current frame. It's an internal variable and should always be set to 0 at init													*/
	4,	/* .totalframes		(int) - the animation length in frames																												*/
	1,	/* .width			(int) - the width of each animation frame in tiles																									*/
	1,	/* .height			(int) - the height of each animation frame in tiles																									*/
	60,	/* .frameduration	(unsigned int) - how many milliseconds each frame should be on screen. Less = faster																*/
	0,	/* .nexttick		(unsigned int) - the next time the frame should change. based on frame duration. This is an internal variable and should always be set to 0 at init	*/
	-1,	/* .loops			(int) - the number of loops to run. -1 equals infinite. This can be adjusted at runtime to pause animations, etc.									*/
	-1,	/* .finalindex		(int) - the index of the final tile to draw when the animation finishes. -1 is the default so total_frames-1 will be the last frame.				*/
	1	/* .frameskip		(int) - the incrementation of each frame. 1 should be set by default. If you wanted the animation to play backwards you could set this to -1, etc.	*/
};

ofLevelScene::ofLevelScene() : ofxScene("level")
{
    
}

ofLevelScene::ofLevelScene(std::string name) : ofxScene(name)
{
    
}

void ofLevelScene::setup()
{
    // attempt to load the level
    ofSetFrameRate(30);
    
    int numLayers = 1;
    int numTiles = 1024;
    int defaultLayer = 0;
    int tileSize = 64;
    spriteRenderer = new ofxSpriteSheetRenderer(numLayers, numTiles, defaultLayer, tileSize);
    
    int texWidth = 256;
    spriteRenderer->loadTexture("player.png", texWidth, GL_NEAREST);

    // background
    numLayers = 1;
    numTiles = 1;
    tileSize = 256;
    backgroundRenderer = new ofxSpriteSheetRenderer(numLayers, numTiles, defaultLayer, tileSize);
    texWidth = 512;
    //backgroundRenderer->loadTexture(getName() + "_background.png", texWidth, GL_NEAREST);
    ofImage *bgImage = new ofImage("level0_background.png");
    backgroundRenderer->loadTexture(&bgImage->getTextureReference());
    
    background = new basicSprite();
    background->animation = idleAnimation;
    background->animation.total_frames = 0;
    background->pos.set(0,0);
    
    ofEnableAlphaBlending();
    
    player = new basicSprite();
    player->pos.set(ofGetWidth() / 2, ofGetHeight() / 2);
    player->velocity.set(0,0);
    player->animation = idleAnimation;
    player->animation.index = 0;
    player->flipDirection = F_NONE;
    player->speed = 100.0;
    spriteRenderer->addCenteredTile(&player->animation, player->pos.x, player->pos.y);
}

void ofLevelScene::update()
{
    backgroundRenderer->clear();
    backgroundRenderer->update(ofGetElapsedTimeMillis());
    backgroundRenderer->addTile(&background->animation,background->pos.x,background->pos.y);
    
    
    spriteRenderer->clear();
    spriteRenderer->update(ofGetElapsedTimeMillis());
    
    if(ofGetKeyPressed('d') || ofGetKeyPressed(OF_KEY_RIGHT)) {
        player->velocity.x = player->speed;
    }
    else if(ofGetKeyPressed('a') || ofGetKeyPressed(OF_KEY_LEFT)) {
        player->velocity.x = -player->speed;
    }
    else {
        player->velocity.x = 0;
    }
    
    if(ofGetKeyPressed('w') || ofGetKeyPressed(OF_KEY_UP)) {
        player->velocity.y = -player->speed;
    }
    else if(ofGetKeyPressed('s') || ofGetKeyPressed(OF_KEY_DOWN)) {
        player->velocity.y = player->speed;
    }
    else {
        player->velocity.y = 0;
    }
    
    
    if(player->velocity.x < -0.1) {
        if(player->animation.index != swimAnimation.index) {
            player->animation = swimAnimation;
        }
        player->flipDirection = F_HORIZ;
    }
    else if(player->velocity.x > 0.1) {
        if(player->animation.index != swimAnimation.index) {
            player->animation = swimAnimation;
        }
        player->flipDirection = F_NONE;
    }
    else if(player->animation.index != idleAnimation.index) {
        player->animation = idleAnimation;
    }
    
    
    player->pos.x += player->velocity.x * ofGetLastFrameTime();
    player->pos.y += player->velocity.y * ofGetLastFrameTime();
    
    
    spriteRenderer->addCenteredTile(&player->animation, player->pos.x, player->pos.y,0, player->flipDirection);
    
    //sort(sprites.begin(),sprites.end(), sortBy
    
    /*
    if(sprites.size() > 0) {
        for(int i = 0; i < sprites.size(); i++) {
            
        }
    }
    */
}

void ofLevelScene::draw()
{
    ofBackground(100,0,100);
    backgroundRenderer->draw();
    spriteRenderer->draw();
//    cam.begin();
//    cam.end();
}

void ofLevelScene::updateEnter()
{
    ofxScene::updateEnter();
}

void ofLevelScene::updateExit()
{
    ofxScene::updateExit();
}

void ofLevelScene::keyPressed(int key)
{
}

void ofLevelScene::keyReleased(int key) {
    
}

void ofLevelScene::mousePressed( int x, int y, int button)
{

}