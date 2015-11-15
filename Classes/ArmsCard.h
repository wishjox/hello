//
//  Card.h
//
//  Created by wish on 10/30/15.
//
//

#ifndef __ArmsCard__
#define __ArmsCard__

#include "cocos2d.h"


typedef enum {
	ARMSRABBIT,
	ARMSMONKEY,
	ARMSDOG,
	ARMSPIG,
	ARMSSQUIRREL,
	ARMSFROZEN,

	ARMSMAX
} ArmsCardSex;

class ArmsCardSprite : public cocos2d::Sprite
{
public:
    static ArmsCardSprite* createCardSprite(int sex, int wight, int height,cocos2d::Point position);
	virtual bool init();
	CREATE_FUNC(ArmsCardSprite);
    ~ArmsCardSprite();

    void setCardSex(int sex);
    int getCardSex();
    bool getVisible();
    void setVisible(bool visible);

private:
    void initCard(int sex, int wight, int height, cocos2d::Point position);
private:

    int touchId;
    bool killMySelf = false;
    bool visible = true;

    ArmsCardSex mySex;

    cocos2d::Label *labelCardNumber;
    cocos2d::LayerColor *colorBackground;
};

#endif /* defined(___048__Card__) */
