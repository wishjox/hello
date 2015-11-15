//
//  Card.h
//
//  Created by wish on 10/30/15.
//
//

#ifndef ___048__Card__
#define ___048__Card__

#include "cocos2d.h"


typedef enum {
	RABBIT,
	MONKEY,
	DOG,
	PIG,
	SQUIRREL,
	FROZEN,

	SEXMAX
} CardSex;

typedef enum {
	NONE,
	NORMAL,
	HAPPY,
	DISMAY,
	CRAZY,
	MOODMAX

} CardMood;

class CardSprite : public cocos2d::Sprite
{
public:
    static CardSprite* createCardSprite(int sex, int mood, int wight, int height,cocos2d::Point position);
	virtual bool init();
	CREATE_FUNC(CardSprite);
    ~CardSprite();
    void runNewNumberAction();

    void setCardSex(int sex);
    int getCardSex();

    void setCardMood(int mood);
    int getCardMood();

    int getTouchId();
    void setTouchId(int id);

    void killSelf(bool kill);
    bool getAlive();

    bool getVisible();
    void setVisible(bool visible);

    int getCancelCount();
    void setCancelCount(int count);

    void showBg();

    void setMoveToId(int id);
    int getMoveToId();

public:
    cocos2d::Sequence* mySeq = NULL;

private:
    void initCard(int sex, int mood, int wight, int height, cocos2d::Point position);
private:

    int touchId;
    bool killMySelf = false;
    bool visible = true;

    int cancellationCount = 0;
    int moveToId = -1;

    CardSex mySex;
    CardMood myMood;

    cocos2d::Label *labelCardNumber;

    cocos2d::LayerColor *colorBackground;
};

#endif /* defined(___048__Card__) */
