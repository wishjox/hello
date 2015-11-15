//
//  Cannon.cpp
//
//
//  Created by wish on 11/9/15.
//
//

#include "Cannon.h"

USING_NS_CC;

CannonSprite* CannonSprite::createCannonSprite(Point position)
{
    CannonSprite *cannon = new CannonSprite();
	if (cannon && cannon->init())
	{
		cannon->autorelease();
		cannon->initCannon(cannon, position);
		//cannon->addRotation(cannon);
		return cannon;
	}
	CC_SAFE_DELETE(cannon);
    
	return NULL;
}

CannonSprite::~CannonSprite()
{
    
}

bool CannonSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    
	return true;
}

void CannonSprite::initCannon(CannonSprite *cannon, Point position)
{

	colorBackground = LayerColor::create(Color4B(0,190,180,250), 80, 250);
	colorBackground->ignoreAnchorPointForPosition(false);
	colorBackground->setAnchorPoint(Point(0.5, 0.0));
	this->addChild(colorBackground);

	cannon->ignoreAnchorPointForPosition(false);
	cannon->setAnchorPoint(Point(0.5, 0.0));
	cannon->setPosition(position);

	ActionInterval* rotateTo = RotateTo::create(3, 60);
	ActionInterval* rotateToSr = RotateTo::create(3, -60);

	//ActionInterval* jumpto = JumpTo ::create(2, ccp(300, 200), 50, 4 );

	auto action = Sequence::create(rotateTo, rotateToSr, NULL);
	auto act = RepeatForever::create((ActionInterval *)action);

	cannon->runAction(act);
}

CardSprite* CannonSprite::fire(int cellSize, Point pos, bool disappear)
{
	CardSprite *bullet = CardSprite::createCardSprite(0, 0, cellSize, cellSize, this->getPosition());
	bullet->setCardSex(CCRANDOM_0_1()*6);
	bullet->setCardMood(1);

	float desX;
	float desY;
	float angle = this->getRotation();
	float radians = MATH_DEG_TO_RAD(angle);

	log("mygame  [Cannon]  disapeear:%d  x:%f y: %f", disappear, pos.x, pos.y);

	if(disappear)
	{
		ActionInterval* moveTo = MoveTo::create(0.5, pos);
		FiniteTimeAction *MoveDone = CallFuncN::create(this, callfuncN_selector(CannonSprite::disappearFinished));
		bullet->runAction(CCSequence::create(moveTo, MoveDone, NULL));
	}
	else
	{
		float firstPosX = 0;
		float firstPosY = pos.y-(float)cellSize;
		float width = abs(firstPosY*tanf(abs(angle)));

		if(angle < 0)
		{
			firstPosX = this->getPosition().x - width;
		}
		else
		{
			firstPosX = this->getPosition().x + width;
		}


		Point firsPos = Point(firstPosX, firstPosY);
		ActionInterval* moveTo1 = MoveTo::create(0.5, firsPos);
		ActionInterval* moveTo2 = MoveTo::create(1.0, pos);
		ActionInterval* sequence = Sequence::create(moveTo1, moveTo2, NULL);
		bullet->runAction(sequence);
	}
	//bullet->schedule(schedule_selector(CardSprite::moveProgress));

	return bullet;
}

void CannonSprite::disappearFinished(cocos2d::Node * sender)
{
	Sprite *sprite = (Sprite*)sender;
	sprite->getParent()->removeChild(sprite, true);
    this->removeChild(sprite, true);
}


