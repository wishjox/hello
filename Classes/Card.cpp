//
//  Card.cpp
//
//
//  Created by wish on 10/23/15.
//
//

#include "Card.h"

USING_NS_CC;

CardSprite* CardSprite::createCardSprite(int sex, int mood, int wight, int height, Point position)
{
	CardSprite *card = new CardSprite();
	if (card && card->init())
	{
		card->autorelease();
		card->initCard(sex,mood,wight,height,position);
		return card;
	}
	CC_SAFE_DELETE(card);
    
	return NULL;
}

CardSprite::~CardSprite()
{
    
}

bool CardSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    
	return true;
}

void CardSprite::runNewNumberAction()
{
    auto action = ScaleBy::create(0.05,0.99);
    runAction(Sequence::create(action, action->reverse(), nullptr));
}

void CardSprite::setCardSex(int sex)
{
	this->mySex = (CardSex)sex;

	Point bgPoint = colorBackground->getPosition();

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/chicken.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/cat.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/bear.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/horse.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/fox.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/frog.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/flash/mapBaseItem.plist");

	if(sex >= (int)SEXMAX)
	{
		sex = 0;
	}

	CCSprite *sprite = CCSprite::createWithSpriteFrameName(__String::createWithFormat("StaticItem0%i.png", sex+1)->getCString());
	sprite->setScale(1.6);
	sprite->setPosition(Point(colorBackground->getContentSize().width/2, colorBackground->getContentSize().height/2));

	colorBackground->removeChildByTag(100, true);
	colorBackground->addChild(sprite);
	sprite->setTag(100);
}

int CardSprite::getCardSex()
{
	return this->mySex;
}

void CardSprite::setCardMood(int mood)
{
	this->myMood = (CardMood)mood;

	labelCardNumber->setString(__String::createWithFormat("%i",mood)->getCString());

	if(this->myMood == HAPPY)
	{
		colorBackground->setColor(Color3B(0,0,0));
		this->mySex = FROZEN;
	}
}

int CardSprite::getCardMood()
{
	return this->myMood;
}

void CardSprite::setMoveToId(int id)
{
	this->moveToId = id;
}

int CardSprite::getMoveToId()
{
	return this->moveToId;
}

int CardSprite::getTouchId()
{
	return this->touchId;
}

void CardSprite::setTouchId(int id)
{

	this->touchId = id;
}

void CardSprite::killSelf(bool kill)
{

	this->killMySelf = kill;
}

int CardSprite::getCancelCount()
{
	return this->cancellationCount;
}

void CardSprite::setCancelCount(int count)
{

	this->cancellationCount = count;
}

bool CardSprite::getAlive()
{
	return this->killMySelf;
}

bool CardSprite::getVisible()
{
	return this->visible;
}

void CardSprite::setVisible(bool visible)
{

	colorBackground->setVisible(visible);

	this->visible = visible;
}

void CardSprite::initCard(int sex, int mood, int wight, int height, Point position)
{
	this->mySex = (CardSex)sex;
	this->myMood = (CardMood)mood;

	this->setPosition(position);

	colorBackground = LayerColor::create(Color4B(200,190,180,0),wight-5,height-5); //Color4B(200,190,180,255)
	//colorBackground->setVisible(false);

	labelCardNumber = Label::createWithSystemFont(__String::createWithFormat("%i",mood)->getCString(),"Consolas",40);
	labelCardNumber->setPosition(Point(colorBackground->getContentSize().width/2, colorBackground->getContentSize().height/2));
	labelCardNumber->setTag(8);
	labelCardNumber->setVisible(false);
	colorBackground->addChild(labelCardNumber);
    
	this->addChild(colorBackground);
}

void CardSprite::showBg()
{
	colorBackground->setVisible(true);
}

//void CardSprite::loadPlist()
//{
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("../Resources/res/flash/chicken.plist");
//
//	CCSprite * sprite = CCSprite::createWithSpriteFrameName("chicken_click_0.png");
//
//}
