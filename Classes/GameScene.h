//
//  GameScene.h
//  2048
//
//  Created by ZeroYang on 5/7/14.
//
//

#ifndef ___048__GameScene__
#define ___048__GameScene__

#include "cocos2d.h"

class CardSprite;
class ArmsCardSprite;
class SimpleRecognizer;
class CannonSprite;
class CardBg;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(GameScene);
    ~GameScene();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    
    //virtual void onEnter() override;
    
private:
    
    void createCardSprite(cocos2d::Size size);
    void createCardNumber(bool animation = true);
    void newNumber(int i, int j, int num);
    void setScore(int score);
    void doCheck();
    int getRandom();
    bool shouldCreateCardNumber();

    bool isWin();
    void removeSuccessLayer();
    
    void saveStatus();
    void resumeStatus();
    
    void onPause(Ref* pSender);

    void spriteMove(CardSprite *target, int dir);
    void spriteMoveEffect(CardSprite *target, cocos2d::Point position);
    void addSpriteTouchEvent(CardSprite *sprite);

    
    cocos2d::Point getPosition(int i, int j);
    cocos2d::Point getBufferPosition(int i, int j);

    void spriteMoveFinished(cocos2d::Node * sender);
    bool isTap(cocos2d::Point srcPoint, cocos2d::Point desPoint);
    void getBeginPoint(CardSprite** card);
    void swapSpritPos(CardSprite *srcCard, CardSprite *desCard);

    void updateSameBox();
    void clearKillBox();
    void spriteDisappearFinished(cocos2d::Node * sender);
    void spriteDisappearEffect(CardSprite *target);
    void spriteDropDown();
    void spriteDropEffect(int srcTouchId, int desTouchId);
    void spriteDropFinished(cocos2d::Node * sender);

    void spriteBufferDropEffect(cocos2d::Point pos, int desTouchId);
    void spriteBufferDropFinished(cocos2d::Node * sender);

    void updateClearBox(float dt);
    void updateBufferBox(int i);

    bool isExsitKilledCard();

    void infectCard();

    // control function
    void initControlModule();
    void createArms();

    // move effect
    void armsPowerAddEffect(CardSprite *sprite);
    void armsPowerMoveEffect(CardSprite *target);
    void armsPowerMoveFinished(cocos2d::Node * sender);
    void armsPowerMoveToEndEffect(CardSprite *target);
    void armsPowerMoveToEndFinished(cocos2d::Node * sender);

    void createCannon();
    void cannonFire();

    void spriteMoveUp(int column);
    void spriteMoveAction(CardSprite *target, cocos2d::Point position);
    void spriteMoveActionFinished(cocos2d::Node * sender);

    void spriteMoveAndDisappear(CardSprite *target, cocos2d::Point position);
    void spriteMoveAndDisappearFinished(cocos2d::Node * sender);

    void checkSameFinished(cocos2d::Node * sender);
    void updateKilledBox();

    void spriteDropDownWithColumn(int i);
    void spriteDropDoneFinished(cocos2d::Node * sender);

    void loopDropDown(float dt);

private:

    //cellSize/cellSpace
    int cellSize = 0;
    int cellSpace = 0;

    int score;

    int gDropX = -1;

    int FocusingId = -1;
    int beginTouchId = -1;

    // for get latest box which need removed
    int removedMaxId = -1;

    cocos2d::Label *cardNumberTTF;
    cocos2d::Point beginPoint;
    
    const static int cardSize = 8;
    CardSprite *cardArr[cardSize][cardSize];

    CardBg *cardBg[cardSize][cardSize];

    // for buffer box define
    const static int cardBufferH = 1;
    CardSprite *cardBufferArr[cardSize][cardBufferH];

    // control area
    ArmsCardSprite *armsCardArr[8];

    // cannon
    CannonSprite *cannon;

    CardSprite* bullet;

    SimpleRecognizer *recognizer;
    
    cocos2d::LayerColor *successLayer;
    cocos2d::LayerColor *controlLayer;

    const static int controlLayerHeight = 500;
    const static int layerBorder = 10;
};

#endif /* defined(___048__GameScene__) */
