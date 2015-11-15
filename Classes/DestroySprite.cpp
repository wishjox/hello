//
//  Card.cpp
//
//
//  Created by wish on 10/23/15.
//
//

#include "DestroySprite.h"

USING_NS_CC;

DestroySprite* DestroySprite::createDestroySprite(int wight, int height, Point position)
{
	DestroySprite *card = new DestroySprite();
	if (card && card->init())
	{
		card->autorelease();
		card->initCard(wight,height,position);
		return card;
	}
	CC_SAFE_DELETE(card);
    
	return NULL;
}

DestroySprite::~DestroySprite()
{
    
}

bool DestroySprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    
	return true;
}

void DestroySprite::initCard(int wight, int height, Point position)
{
	this->setPosition(position);

	colorBackground = LayerColor::create(Color4B(200,190,180,0),wight-5,height-5);


	CCSpriteFrameCache * cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("res/flash/destroy_effect.plist");

	Sprite* sp = CCSprite::createWithSpriteFrameName("destroy_effect_0.png");
	sp->setPosition(Point(colorBackground->getContentSize().width/2, colorBackground->getContentSize().height/2));

	int count = 20;
	char str[20] = {0};
	Vector<SpriteFrame*> sfme = Vector<SpriteFrame*>(20);

	for(int i = 0; i < count; i++)
	{
		SpriteFrame *fname = cache->spriteFrameByName(__String::createWithFormat("destroy_effect_%i.png",i)->getCString());
		sfme.pushBack(fname);
	}

	Animation *animation = CCAnimation::createWithSpriteFrames(sfme , 0.1f);
   // animation->setDelayPerUnit(2.8f / 14.0f);
    animation->setRestoreOriginalFrame(true);
    animation->setLoops(1);
    FiniteTimeAction * animate = Animate::create(animation);

	FiniteTimeAction *actionDestroyDone = CallFuncN::create(this, callfuncN_selector(DestroySprite::spriteDestroyFinished));
	sp->runAction(Sequence::create(animate,actionDestroyDone,NULL));

	this->addChild(colorBackground);
	colorBackground->addChild(sp);
}

void DestroySprite::spriteDestroyFinished(cocos2d::Node * sender)
{
	Sprite* sp = (Sprite *)sender;

	this->removeChild	(sp, true);
	this->getParent()->removeChild(colorBackground->getParent(), true);
}
