//
//  Cannon.h
//
//  Created by wish on 11/9/15.
//
//

#ifndef __CardBg__
#define __CardBg__

#include "cocos2d.h"


class CardBg : public cocos2d::Sprite
{
public:
    static CardBg* createCardBg(int wight, int height, cocos2d::Point position);
	virtual bool init();
	CREATE_FUNC(CardBg);
    ~CardBg();

    void setExistChild(bool exist);
    bool getExistChild();
private:
    void initCardBg(int wight, int height, cocos2d::Point position);
private:

    bool existChild = false;
    cocos2d::LayerColor *colorBackground;
};

#endif
