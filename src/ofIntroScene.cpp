//
//  ofIntroScene.cpp
//  ludumdare29
//
//  Created by Alex on 4/25/14.
//
//

#include "ofIntroScene.h"


ofIntroScene::ofIntroScene() : ofxScene("intro")
{
    
}

void ofIntroScene::setup()
{
    
}

void ofIntroScene::update()
{
    ofBackground(200);
}

void ofIntroScene::draw()
{
    
}

void ofIntroScene::updateEnter()
{
    ofxScene::updateEnter();
}

void ofIntroScene::updateExit()
{
    ofxScene::updateExit();
}

void ofIntroScene::keyPressed(int key)
{
    done();
}

void ofIntroScene::mousePressed( int x, int y, int button)
{
    done();
}
