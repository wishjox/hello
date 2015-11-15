//
//  Card.cpp
//
//
//  Created by wish on 10/23/15.
//
//

#include "ArmsCard.h"

USING_NS_CC;

ArmsCardSprite* ArmsCardSprite::createCardSprite(int sex, int wight, int height, Point position)
{
	ArmsCardSprite *card = new ArmsCardSprite();
	if (card && card->init())
	{
		card->autorelease();
		card->initCard(sex,wight,height,position);
		return card;
	}
	CC_SAFE_DELETE(card);
    
	return NULL;
}

ArmsCardSprite::~ArmsCardSprite()
{
    
}

bool ArmsCardSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    
	return true;
}

void ArmsCardSprite::setCardSex(int sex)
{
	this->mySex = (ArmsCardSex)sex;

	switch(sex)
	{
	case ARMSRABBIT:
		colorBackground->setColor(Color3B(240,230,220));
		break;
	case ARMSMONKEY:
		colorBackground->setColor(Color3B(140,220,200));
		break;
	case ARMSDOG:
		colorBackground->setColor(Color3B(240,80,120));
		break;
	case ARMSPIG:
		colorBackground->setColor(Color3B(140,40,90));
		break;
	case ARMSSQUIRREL:
		colorBackground->setColor(Color3B(40,20,190));
		break;
	default:
		colorBackground->setColor(Color3B(40,230, 120));
		break;
	}
}

int ArmsCardSprite::getCardSex()
{
	return this->mySex;
}

bool ArmsCardSprite::getVisible()
{
	return this->visible;
}

void ArmsCardSprite::setVisible(bool visible)
{

	colorBackground->setVisible(visible);

	this->visible = visible;
}

void ArmsCardSprite::initCard(int sex, int wight, int height, Point position)
{
	this->mySex = (ArmsCardSex)sex;
	this->setPosition(position);

	colorBackground = LayerColor::create(Color4B(200,190,180,255),wight-15,height-15);
    
	this->addChild(colorBackground);
}
