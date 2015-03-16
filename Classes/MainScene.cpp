//
//  MainScene.cpp
//  BullfightGame
//
//  Created by 张 恒 on 15/3/16.
//
//

#include "MainScene.h"
CCScene* MainScene::scene()
{
    CCScene *scene = CCScene::create();
    MainScene *layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}