//
//  Created by wish jin on 11/14/15
//
//

#ifndef ___ActionLayer___
#define ___ActionLayer___

#include "cocos2d.h"

USING_NS_CC;

class ActionLayer : public LayerColor
{
public:
    static PopLayer* create(Point position);
    virtual bool initWithColor(const Color4B& color);
    
    //瑙���镐��浠剁��������璋���芥��
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
private:
    void onRestart(Ref* pSender);
    void onExit(Ref* pSender);
    void onContinue(Ref* pSender);
};

#endif /* defined(___048__PopLayer__) */
