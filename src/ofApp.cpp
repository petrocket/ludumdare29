#include "ofApp.h"

// scenes
#include "ofIntroScene.h"
#include "ofLevelScene.h"
#include "of3dLevelScene.h"

//--------------------------------------------------------------
void ofApp::setup()
{
//    ofSetDataPathRoot("../../data/");
    
    sceneManager.add(new ofIntroScene());
    sceneManager.add(new of3dLevelScene("level0"));
    
    sceneManager.gotoScene("level0");
    
    setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(sceneManager.getCurrentScene()->isDone()) {
        if(oldScene != sceneManager.getCurrentScene()) {
            oldScene = sceneManager.getCurrentScene();
            sceneManager.nextScene();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
