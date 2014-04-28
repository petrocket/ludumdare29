//
//  ofIntroScene.h
//  ludumdare29
//
//  Created by Alex on 4/25/14.
//
//

#ifndef __ludumdare29__ofIntroScene__
#define __ludumdare29__ofIntroScene__

#include "ofxAppUtils.h"



class ofIntroScene : public ofxScene
{
public:
    ofIntroScene();
    
    void draw();
    void setup();
    void update();
    
    void updateEnter();
    void updateExit();
    
    void keyPressed(int key);
    void mousePressed( int x, int y, int button);
};

#endif /* defined(__ludumdare29__ofIntroScene__) */
