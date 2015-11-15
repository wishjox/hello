//
//  Card.h
//
//  Created by wish on 10/30/15.
//
//

#ifndef ___DestorySprite___
#define ___DestorySprite___

#include "cocos2d.h"


class DestroySprite : public cocos2d::Sprite
{
public:
    static DestroySprite* createDestroySprite(int wight, int height,cocos2d::Point position);
	virtual bool init();
	CREATE_FUNC(DestroySprite);
    ~DestroySprite();

    void spriteDestroyFinished(cocos2d::Node * sender);

private:
    void initCard(int wight, int height, cocos2d::Point position);
private:

    cocos2d::LayerColor *colorBackground;
};

#endif
