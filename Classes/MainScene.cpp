//
//  MainScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "MainScene.h"
MainScene::MainScene(){
}
MainScene::~MainScene(){
    CCLog("~ <<%s>>",__PRETTY_FUNCTION__);
}
CCScene* MainScene::scene()
{
    CCScene *scene = CCScene::create();
    MainScene *layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}