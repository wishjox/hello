//
//  Cannon.h
//
//  Created by wish on 11/9/15.
//
//

#ifndef __Cannon__
#define __Cannon__

#include "cocos2d.h"
#include "card.h"


class CannonSprite : public cocos2d::Sprite
{
public:
    static CannonSprite* createCannonSprite(cocos2d::Point position);
	virtual bool init();
	//virtual bool initWithFile(const std::string& filename);
	CREATE_FUNC(CannonSprite);
    ~CannonSprite();

    CardSprite* fire(int cellSize, cocos2d::Point pos, bool disappear);
    void disappearFinished(cocos2d::Node * sender);
private:
    void initCannon(CannonSprite *cannon, cocos2d::Point position);
private:

    cocos2d::LayerColor *colorBackground;
};

#endif
