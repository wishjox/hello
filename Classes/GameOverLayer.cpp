//
//  GameOverLayer.cpp
//  2048
//
//  Created by ZeroYang on 5/8/14.
//
//

#include "GameOverLayer.h"

#include "GameOverLayer.h"
#include "GameScene.h"
#include "HighScore.h"

GameOverLayer* GameOverLayer::create(const Color4B& color)
{
    GameOverLayer *pRet = new GameOverLayer();
    if(pRet && pRet->initWithColor(color))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool GameOverLayer::initWithColor(const Color4B& color)
{
    if (!LayerColor::initWithColor(color)) {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    Point centerPos = Point(winSize.width / 2, winSize.height / 2);
    
    auto gameOverTitle = Label::createWithSystemFont("GAME OVER","Consolas",80);
    gameOverTitle->setPosition(Point(centerPos.x, centerPos.y + 150));
    addChild(gameOverTitle);
    
    char score[64];
    sprintf(score, "%d", HighScore::getInstance()->getScore());
    auto scoreTitle = Label::createWithSystemFont(score,"Consolas",60);
    scoreTitle->setPosition(Point(centerPos.x, centerPos.y + 50));
    addChild(scoreTitle);
    
    char temp[64];
    sprintf(temp, "BEST:%d", HighScore::getInstance()->getHighScore());
    auto highScoreTitle = Label::createWithSystemFont(temp,"Consolas",40);
    highScoreTitle->setPosition(Point(centerPos.x, centerPos.y - 50));
    addChild(highScoreTitle);
    
    MenuItemFont::setFontName("Consolas");
    MenuItemFont::setFontSize(80);
    auto menuItemRestart = MenuItemFont::create("RESTART", CC_CALLBACK_1(GameOverLayer::onRestart, this));

    
    auto menu = Menu::create(menuItemRestart, NULL);
    addChild(menu);
    menu->setPosition(Point(centerPos.x, centerPos.y - 150));
    

    //设置触摸事件监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameOverLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(GameOverLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameOverLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    touchListener->setSwallowTouches(true);
    
    return true;
}

void GameOverLayer::onRestart(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameScene::createScene());
    Director::getInstance()->resume();
}

bool GameOverLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void GameOverLayer::onTouchMoved(Touch* touch, Event* event)
{
}

void GameOverLayer::onTouchEnded(Touch* touch, Event* event)
{
}