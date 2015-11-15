//
//  Cannon.cpp
//
//
//  Created by wish on 11/9/15.
//
//

#include "CardBg.h"

USING_NS_CC;

CardBg* CardBg::createCardBg(int wight, int height, Point position)
{
	CardBg *card = new CardBg();
	if (card && card->init())
	{
		card->autorelease();
		card->initCardBg(wight, height, position);
		//cannon->addRotation(cannon);
		return card;
	}
	CC_SAFE_DELETE(card);
    
	return NULL;
}

CardBg::~CardBg()
{
    
}

bool CardBg::init()
{
	if (!Sprite::init())
	{
		return false;
	}
    
	return true;
}

void CardBg::initCardBg(int wight, int height, Point position)
{
	colorBackground = LayerColor::create(Color4B(200,190,180,250), wight-5, height-5);

	this->setPosition(position);
	this->addChild(colorBackground);
}

void CardBg::setExistChild(bool exist)
{
	this->existChild = exist;
}

bool CardBg::getExistChild()
{
	return this->existChild;
}




