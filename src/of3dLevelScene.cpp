//
//  of3dLevelScene.cpp
//  ludumdare29
//
//  Created by Alex on 4/26/14.
//
//

#include "of3dLevelScene.h"
#include "ofxXmlSettings.h"

of3dLevelScene::of3dLevelScene() : ofxScene("3dlevel"),
    editMode(false),
    currentObject("chest")
{
    
}

of3dLevelScene::of3dLevelScene(std::string name) : ofxScene(name),
    editMode(false),
    currentObject("chest")
{
    
}

void of3dLevelScene::setup()
{
    if(level.loadModel(getName() + ".dae")) {
        level.setRotation(0, 180, 1, 0, 0);
    }
    
    setupCollision();
    
    // fog
    GLfloat fogCol[4] = {0,0,0.5,1.f};
    glFogi(GL_FOG_MODE, 1); // exp2
    glFogfv(GL_FOG_COLOR, fogCol);
    glFogf(GL_FOG_DENSITY, 0.02f);
    glFogf(GL_FOG_START, 10.f);
    glFogf(GL_FOG_END, 300.f);
    
    cam.setup();
    cam.setNearClip(1.0f);
    cam.setPosition(0,6.f,-10.f);
    cam.lookAt(ofVec3f(0,0,100),ofVec3f(0,1,0));
    cam.setFov(38.f);
    cam.usemouse = false;
//    cam.enableOrtho();
    
    ofEnableAlphaBlending();
    ofEnableLighting();
    ofEnableDepthTest();
    
    // lighting
    light.setPosition(0, 400, 20);
    light.setDiffuseColor(ofFloatColor(0.95,1.0,1.0));
    light.enable();
    light.setParent(cam);
    
    // gui
    gui = new ofxUICanvas();
    gui->setFont("GUI/DejaVuSans.ttf");
    gui->setColorBack(ofColor(0,0,0,0));
    
    float guiWidth = 512.f;
    gameOverLose = new ofImage("GUI/game-over-lose.png");
    gameOverWin = new ofImage("GUI/game-over-win.png");
    gameOverImage = gui->addImage("game_over", gameOverLose, 512,128);
    okButton = gui->addImageButton("OK", "GUI/ok-btn.png", true, 256, 64);
    // center horizontally
    okButton->getRect()->setX(gameOverImage->getRect()->getHalfWidth() - okButton->getRect()->getHalfWidth());
    gui->autoSizeToFitWidgets();
    // center horizontally
    gui->setPosition(ofGetWidth()/2 - gui->getRect()->getHalfWidth(),
                     ofGetHeight()/2 - gui->getRect()->getHalfHeight());
    ofAddListener(gui->newGUIEvent, this, &of3dLevelScene::guiEvent);
    
    gui->setVisible(false);
    gui->setAutoDraw(false);
    
    // effects
    post.init(ofGetWidth(),ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(true);
    convolutionPass = post.createPass<ConvolutionPass>();
    convolutionPass->setEnabled(false);

    // player
    player = new ofPlayer();
    
    backgroundMusic.loadSound("ludumdare29.mp3");
    backgroundMusic.setVolume(0.7);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
    
    // load level
    load();
}

void of3dLevelScene::setupCollision()
{
    if(collision.loadModel(getName() + "_collision.dae")) {
        collision.setRotation(0, 180, 1, 0, 0);
        collision.setPosition(0,0.5,0);
        collision.setScale(0.977, 0.977, 0.977);
        
        vector<ofVboMesh> meshes;
        for(int i = 0; i < collision.getNumMeshes(); i++) {
            meshes.push_back(collision.getMesh(i));
        }
        
        collisionFaces.clear();
        
        for (int i = 0; i < meshes.size(); i++) {
            vector<ofIndexType>& indices = meshes.at(i).getIndices();
            ofVec3f* vertsPtr = meshes.at(i).getVerticesPointer();
            ofMatrix4x4 mat = collision.getMeshHelper(i).matrix * collision.getModelMatrix();
            for (int j = 0; j < indices.size(); j+=3) {
                FaceTri tri;
                tri.v0 = vertsPtr[indices.at(j + 0)] * mat;
                tri.v1 = vertsPtr[indices.at(j + 1)] * mat;
                tri.v2 = vertsPtr[indices.at(j + 2)] * mat;
                collisionFaces.push_back(tri);
            }
        }
    }
}

void of3dLevelScene::update()
{
    ofVec3f lastPosition = player->position;
    
    // update animations
    player->update();

    
    if(player->health > 0.f) {
        // clamp player pos
        /*
        if(player->position.y > 14.f) {
            player->position.y = 14.f;
        }
        else if(player->position.y < -20.f) {
            player->position.y = -20.f;
        }
        */
        
        if(player->position.x < -220.f) {
            player->position.x = -220.f;
            // player wins
            gameOver =  true;
        }
        else if(player->position.x > 240.f) {
            player->position.x = 240.f;
        }
        
        vector<Ray> rays;
        Ray ray;
        ray.rayOrig.set(player->position);
        ray.rayEnd.set(player->position + ofVec3f(0,0,100));
        rays.push_back(ray);
        vector<IntersectInfo> ii = hitTest.checkMeshIntersection(rays, collisionFaces);
        if(!ii.size()) {
            player->position = lastPosition;
        }
    }
    
    // chests
    for(int i = 0; i < chests.size(); i++) {
        float dist = chests.at(i)->position.distanceSquared(player->position);
        if(dist < 10.0) {
            chests.at(i)->open();
        }
        chests.at(i)->update();
    }
    
    // enemies
    for(int i = 0; i < enemies.size(); i++) {
        float dist = enemies.at(i)->position.distanceSquared(player->position);
        if(dist < 0.25) {
            // hurt player
            player->health = -1.f;
            gameOver = true;
        }
        enemies.at(i)->update();
    }
    
    if(editMode) {
        // fly around
        cam.speed = 20.0f;
        cam.usemouse = true;
        cam.applyRotation = true;
    }
    else if(gameOver) {
        gui->setVisible(true);
        convolutionPass->setEnabled(true);
        
        if(player->health <= 0.f) {
            gameOverImage->setImage(gameOverLose);
        }
        else {
            gameOverImage->setImage(gameOverWin);
        }
    }
    else if(player->health > 0.f) {
        cam.speed = 0.f;
        cam.setPosition(player->position + ofVec3f(0,0,-15.f));
        cam.usemouse = false;
        cam.applyRotation = false;
    }
}

void of3dLevelScene::draw()
{
    post.begin(cam);
    ofEnableLighting();
//    cam.begin();
    light.enable();
    glEnable(GL_FOG);
    ofBackground(0,0,127);
    
    // level
    if(level.getNumMeshes()) {
        level.drawFaces();
    }
    
    // player
    player->draw();
    
    // chests
    for(int i = 0; i < chests.size(); i++) {
        chests.at(i)->draw();
    }    
    
    // enemies
    for(int i = 0; i < enemies.size(); i++) {
        enemies.at(i)->draw();
    }
    
    glDisable(GL_FOG);
//    ofDrawAxis(20);
//    light.draw();
    //    if(collision.getNumMeshes()) {
    //        collision.drawFaces();
    //    }

    
    if(editMode) {
        ofPushMatrix();
        ofTranslate(cam.getGlobalPosition().x, cam.getGlobalPosition().y, 0.f);
        ofDrawAxis(1);
        ofPopMatrix();
    }
    
    cam.end();
    post.end();
    light.disable();
    ofDisableLighting();
    
    ofSetColor(255,255,255);
    if(gui->isVisible()) {
        gui->draw();
    }
    
    if(editMode) {
        ofSetColor(255,255,255);
        ofDrawBitmapString("edit mode active", 20, 20);
        ofSetColor(200,200,255);
        ofDrawBitmapString("object: " + currentObject, 20, 40);
        ofSetColor(255,255,255);
    }
}

void of3dLevelScene::load()
{
    // clear first
    for(int i = 0; i < chests.size(); i++) {
        delete chests.at(i);
    }
    chests.clear();
    
    for(int i = 0; i < enemies.size(); i++) {
        delete enemies.at(i);
    }
    enemies.clear();
    
    ofxXmlSettings load;
    
    if(load.loadFile(getName() + ".xml")) {
        // PLAYER
        startPos.x = load.getValue("player_start:X", 0);
        startPos.y = load.getValue("player_start:Y", 0);
        startPos.z = load.getValue("player_start:Z", 0);

        
        // CHESTS
        load.pushTag("chests");
        int numChests = load.getNumTags("chest");
        for(int i = 0; i < numChests; i++) {
            load.pushTag("chest",i);
            ofVec3f pos ;
            pos.x = load.getValue("X", 0.f);
            pos.y = load.getValue("Y", 0.f);
            pos.z = load.getValue("Z", 0.f);
            chests.push_back(new ofTreasureChest(pos));
            load.popTag(); // chest
        }
        load.popTag();
        
        
        // ENEMIES
        load.pushTag("enemies");
        int numEnemies = load.getNumTags("enemy");
        for(int i = 0; i < numEnemies; i++) {
            load.pushTag("enemy",i);
            std::string type = load.getValue("type", "");
            
            ofVec3f pos ;
            pos.x = load.getValue("X", 0.f);
            pos.y = load.getValue("Y", 0.f);
            pos.z = load.getValue("Z", 0.f);
            if(type == "puffer") {
                enemies.push_back(new ofPuffer(pos));
            }
            load.popTag(); // enemy
        }
        load.popTag();
    }
    else {
        ofLogError("Could not load " + getName() + ".xml");
    }
    
    // reset player
    player->health = 100.f;
    player->position = startPos;
    convolutionPass->setEnabled(false);
    gui->setVisible(false);
    gameOver = false;
}

void of3dLevelScene::save()
{
    ofxXmlSettings save;
    save.addTag("player_start");
    save.pushTag("player_start");
    save.addValue("X", startPos.x);
    save.addValue("Y", startPos.y);
    save.addValue("Z", startPos.z);
    save.popTag();
    
    save.addTag("chests");
    save.pushTag("chests");
    for(int i = 0; i < chests.size(); i++) {

        save.addTag("chest");
        save.pushTag("chest",i);
        {
            save.addValue("X", chests.at(i)->position.x);
            save.addValue("Y", chests.at(i)->position.y);
            save.addValue("Z", chests.at(i)->position.z);
        }
        save.popTag(); // chest
    }
    save.popTag(); // chests
    
    
    save.addTag("enemies");
    save.pushTag("enemies");
    for(int i = 0; i < enemies.size(); i++) {
        save.addTag("enemy");
        save.pushTag("enemy",i);
        {
            save.addValue("type", enemies.at(i)->getType());
            if(enemies.at(i)->getType() == "puffer") {
                save.addValue("X", enemies.at(i)->patrolCenter.x);
                save.addValue("Y", enemies.at(i)->patrolCenter.y);
                save.addValue("Z", enemies.at(i)->patrolCenter.z);
            }
            else {
                save.addValue("X", enemies.at(i)->position.x);
                save.addValue("Y", enemies.at(i)->position.y);
                save.addValue("Z", enemies.at(i)->position.z);
            }
        }
        save.popTag(); // enemy
    }
    save.popTag(); // chests
    
    if(!save.saveFile(getName() + ".xml")) {
        ofLogError("Failed to save " + getName() + ".xml");
    }
}

void of3dLevelScene::updateEnter()
{
    ofxScene::updateEnter();
}

void of3dLevelScene::updateExit()
{
    ofxScene::updateExit();
}

void of3dLevelScene::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget == okButton &&
       okButton->isHit(ofGetMouseX(), ofGetMouseY())) {
        cout << "OK Button clicked" << endl;
        load();
    }
}

void of3dLevelScene::keyPressed(int key)
{

}

void of3dLevelScene::keyReleased(int key)
{
    switch (key) {
        case OF_KEY_TAB:
            editMode = !editMode;
            break;
        case '=':
            if(editMode) {
                // add
                if(currentObject == "chest") {
                    chests.push_back(new ofTreasureChest(ofVec3f(cam.getGlobalPosition().x, cam.getGlobalPosition().y, 3.0)));
                }
                else if(currentObject == "puffer") {
                    enemies.push_back(new ofPuffer(ofVec3f(cam.getGlobalPosition().x, cam.getGlobalPosition().y, 0.0)));
                }
                else if(currentObject == "start_position") {
                    startPos = ofVec3f(cam.getGlobalPosition().x, cam.getGlobalPosition().y, 0.f);
                }
                save();
            }
            break;
        case '-':
            if(editMode) {
                // remove
                ofVec3f pos(cam.getGlobalPosition().x, cam.getGlobalPosition().y, 0.0);
                if(currentObject == "chest") {
                    for(int i = 0; i < chests.size(); i++) {
                        if(chests.at(i)->position.distanceSquared(pos) < 10.f) {
                            delete chests.at(i);
                            chests.erase(chests.begin() + i);
                            break;
                        }
                    }
                    save();
                }
                else if(currentObject == "puffer") {
                    for(int i = 0; i < enemies.size(); i++) {
                        if(enemies.at(i)->position.distanceSquared(pos) < 10.f) {
                            delete enemies.at(i);
                            enemies.erase(enemies.begin() + i);
                            break;
                        }
                    }
                    save();
                }
            }
            break;
        case ']':
            if(editMode) {
                // cycle through objects to edit
                if(currentObject == "chest") {
                    currentObject = "start_position";
                }
                else if(currentObject == "start_position") {
                    currentObject = "puffer";
                }
                else {
                    currentObject = "chest";
                }
            }
            break;
        case 'k':
            if(editMode) {
                save();
            }
            break;
        case 'l':
            if(editMode) {
                load();
            }
            break;
        default:
            break;
    }
}

void of3dLevelScene::mousePressed( int x, int y, int button)
{

}