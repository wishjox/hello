//
//  GameScene.cpp
//
//
//  Created by wish on 10/23/15.
//
//

#include "GameScene.h"
#include "Card.h"
#include "ArmsCard.h"
#include "SimpleRecognizer.h"
#include "GameOverLayer.h"
#include "HighScore.h"
#include "PopLayer.h"
#include "Cannon.h"
#include "CardBg.h"
#include "DestroySprite.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene::~GameScene()
{
    delete recognizer;
    
    HighScore::destroyInstance();
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto layerColorBG = LayerColor::create(Color4B(180, 170, 160, 255));
    this->addChild(layerColorBG);
    
    //pause
    MenuItemFont::setFontName("Consolas");
    MenuItemFont::setFontSize(80);
    auto menuItemPause = MenuItemFont::create("PAUSE", CC_CALLBACK_1(GameScene::onPause, this));
    auto menu = Menu::create(menuItemPause, NULL);
    //addChild(menu);
    menu->setPosition(Point(200, visibleSize.height - 200));
    
    auto cardNumberTitle = Label::createWithSystemFont("top info","Consolas",80);
    
    auto topLayerColorBG = LayerColor::create(Color4B(180, 0, 160, 255));
    topLayerColorBG->setPosition(Point(0, visibleSize.height - 300));
    topLayerColorBG->addChild(cardNumberTitle);
    this->addChild(topLayerColorBG);

    auto controlTitle = Label::createWithSystemFont("Control area","Consolas",80);
    controlLayer = LayerColor::create(Color4B(180, 0, 10, 255));
    controlTitle->setPosition(Point(visibleSize.width/2, controlLayerHeight/2));
    controlLayer->setPosition(Point(0, 0));
    controlLayer->setContentSize(CCSizeMake(visibleSize.width, controlLayerHeight));
    controlLayer->addChild(controlTitle);
    this->addChild(controlLayer);

    createCannon();

    score = 0;
    cardNumberTTF = Label::createWithSystemFont("0", "Consolas", 70);
    cardNumberTTF->setPosition(Point(400, visibleSize.height - 100));
   // addChild(cardNumberTTF);
    
    createCardSprite(visibleSize);
    initControlModule();
    
    if (UserDefault::getInstance()->getBoolForKey("history")) {
        resumeStatus();
    }
    else
    {
    	createCardNumber(false);
    }

    updateSameBox();

    //this->scheduleOnce(schedule_selector(GameScene::updateTouched), 2.0f);

    recognizer = new SimpleRecognizer();
    
    return true;
}

void GameScene::initControlModule()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int armsCount = (int)ARMSMAX;
	int height = 100;
	int width = (visibleSize.width-200)/armsCount;
	for(int i=0; i<armsCount; i++)
	{
		Point pos(100+width*i, controlLayerHeight-height);

		ArmsCardSprite *card = ArmsCardSprite::createCardSprite(0, width, height, pos);
		card->setVisible(false);
		armsCardArr[i] = card;

		controlLayer->addChild(card);
	}
	// load data;
	createArms();

}

void GameScene::createArms()
{
	int armsCount = (int)ARMSMAX;

	for(int i=0; i<armsCount; i++)
	{
		armsCardArr[i]->setCardSex(CCRANDOM_0_1()*ARMSMAX);
	}
}

void GameScene::createCannon()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point ctrlPos = controlLayer->getPosition();
	Point position = Point(visibleSize.width/2, controlLayerHeight/2);

	cannon = CannonSprite::createCannonSprite(position);

	this->addChild(cannon);
}

void GameScene::onPause(Ref* pSender)
{
    PopLayer *poplayer = PopLayer::create(Color4B(0,0,0,180));
    getParent()->addChild(poplayer);
    Director::getInstance()->pause();
}

void GameScene::addSpriteTouchEvent(CardSprite *sprite)
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
//    touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
//    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::isTap(Point srcPoint, Point desPoint)
{
	float sX = srcPoint.x;
	float sY = srcPoint.y;

	float dX = desPoint.x;
	float dY = desPoint.y;

   // log("mygame beginPoint:  x:%f y:%f  dX:%f dY:%f  ", sX, sY, dX, dY);

    if((dX>=sX && dX<=sX+cellSize) && (dY>=sY && dY<=sY+cellSize))
    {
    	return true;
    }

    return false;

}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	beginPoint = touch->getLocation();
	recognizer->beginPoint(beginPoint);

	cannonFire();

    return true;
}

void GameScene::cannonFire()
{
	float angle = cannon->getRotation();
	float radians = MATH_DEG_TO_RAD(angle);
	float touchedX = 0;
	int touchColumn = -1;
	bool disappear = false;
	float movePosX = 0;
	float movePosY = 0;
	Point movePos;

	Point cannonPos = cannon->getPosition();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	movePosY = (visibleSize.width/2)/(tanf(abs(angle))) + cannonPos.y;

	// Get touched card X pix with bullet road
	float bulletTouchedWidth = abs(tanf(abs(angle))*abs(cardBg[0][0]->getPosition().y-cannonPos.y));
	log("mygame [Cannon] cardArr[0][0].y :%f  cannonPos.Y:%f", cardBg[0][0]->getPosition().y, cannonPos.y);
	if(angle < 0)
	{
		touchedX = cannonPos.x - bulletTouchedWidth;
		movePosX = 0;
	}
	else
	{
		touchedX = cannonPos.x + bulletTouchedWidth;
		movePosX = visibleSize.width;
	}

	log("mygame [Cannon] angle:%f  tochedX:%f touchedWith:%f", angle, touchedX, bulletTouchedWidth);

	for(int i = 0; i < cardSize; i++)
	{
		float cardX = cardBg[i][0]->getPosition().x;

		if(touchedX >= cardX && (touchedX <= cardX+cellSize))
		{
			touchColumn = i;
			break;
		}
	}

	if(touchColumn == -1)
	{
		disappear = true;
		movePos = Point(movePosX, movePosY);
	}
	else
	{
		movePos = cardBg[touchColumn][0]->getPosition();
		spriteMoveUp(touchColumn);
	}

	bullet = cannon->fire(cellSize, movePos, disappear);

	this->addChild(bullet);
}

void GameScene::spriteMoveUp(int column)
{
	log("mygame  spriteMoveUp");
	for(int i = 0; i < cardSize; i++)
	{
		int j = i+1;
		Point desPos;
		if(j < cardSize)
		{
			desPos = cardArr[column][j]->getPosition();
			spriteMoveAction(cardArr[column][i],desPos);
		}
		else
		{
			Point pos = cardArr[column][i]->getPosition();
			desPos = Point(pos.x, pos.y+cellSize);
			spriteMoveAndDisappear(cardArr[column][i], desPos);
		}

	}
}

void GameScene::spriteMoveAndDisappear(CardSprite *target, Point position)
{
	float minDuration = 0.5;

	FiniteTimeAction *actionMove = MoveTo::create(minDuration, position);
	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveAndDisappearFinished));
	target->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
}

void GameScene::spriteMoveAndDisappearFinished(cocos2d::Node * sender)
{
	Sprite *sprite = (Sprite*)sender;
	this->removeChild(sprite);
}

void GameScene::spriteMoveAction(CardSprite *target, Point position)
{
	float minDuration = 0.5;

	FiniteTimeAction *actionMove = MoveTo::create(minDuration, position);
	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveActionFinished));
	target->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
}

void GameScene::spriteMoveActionFinished(cocos2d::Node * sender)
{
	CardSprite *sprite = (CardSprite*)sender;

	int touchId = sprite->getTouchId();
	int lastId = touchId % cardSize;
	int cardColumn = touchId / cardSize;

	if(lastId == 0)
	{
		for(int i=cardSize-1; i>0; i--)
		{
			cardArr[cardColumn][i] = cardArr[cardColumn][i-1];
			cardArr[cardColumn][i]->setTouchId(cardColumn*cardSize+i);
		}
		cardArr[cardColumn][0] = bullet;
		cardArr[cardColumn][0]->setTouchId(cardColumn*cardSize);

		updateSameBox();
	}
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{
    Point pos = touch->getLocation();
    recognizer->movePoint(pos);
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    Point pos = touch->getLocation();
    SimpleGestures rtn = recognizer->endPoint(pos);

    CardSprite *target = NULL;

    getBeginPoint(&target);

	if(target == NULL)
	{
		log("mygame target null");
	}
	else
	{
		spriteMove(target, rtn);
	}
}

void GameScene::createCardSprite(Size size)
{
    cellSize = (size.width - layerBorder*2 - 3*cellSpace - 40)/cardSize;

    for (int i = 0; i < cardSize; i++)
    {
        for (int j = 0; j < cardSize; j++)
        {
            //add for Bg;
        	CardBg *bgCard = CardBg::createCardBg(cellSize, cellSize, getPosition(i, j));
			cardBg[i][j] = bgCard;

			addChild(bgCard);
        }
    }

    for (int i = 0; i < cardSize; i++)
    {
        for (int j = 0; j < cardSize; j++)
        {
            //add card
            CardSprite *card = CardSprite::createCardSprite(0, 0, cellSize, cellSize, getPosition(i, j));
            addSpriteTouchEvent(card);
            card->setTouchId(i*cardSize+j);
            cardArr[i][j] = card;

            addChild(card);


        }
    }

//    // for buffer box
//    for (int i = 0; i < cardSize; i++)
//    {
//        for (int j = 0; j < cardBufferH; j++)
//        {
//            CardSprite *card = CardSprite::createCardSprite(0, 0, cellSize, cellSize, getBufferPosition(i, j));
//            //card->setVisible(false);
//            card->setTouchId(i);
//            cardBufferArr[i][j] = card;
//
//            addChild(card);
//        }
//    }

}

void GameScene::updateSameBox()
{
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
			int curSex = cardArr[i][j]->getCardSex();
			int mood = cardArr[i][j]->getCardMood();

			int topCount = 1;
			int bottomCount = 1;
			int leftCount = 1;
			int rightCount = 1;

			int si = i;
			int sj = j;

			bool killed = cardArr[i][j]->getAlive();
			bool exist = cardBg[i][j]->getExistChild();
			if(exist && !killed)
			{
				// bottom direction
				while(((sj-1)>0) &&
						cardBg[si][sj-1]->getExistChild() &&
							(cardArr[si][sj-1]->getCardSex() == curSex))
				{
					bottomCount++;
					sj--;
				}

				// top direction
				sj = j;
				while(((sj+1)<cardSize) &&
						cardBg[si][sj+1]->getExistChild() &&
							(cardArr[si][sj+1]->getCardSex() == curSex))
				{
					topCount++;
					sj++;
				}

				// left direction
				sj = j;

				while(((si-1)>0) &&
						cardBg[si-1][sj]->getExistChild() &&
							(cardArr[si-1][sj]->getCardSex() == curSex))
				{
					leftCount++;
					si--;
				}

				// right direction
				si = i;
				while(((si+1)<cardSize) &&
						cardBg[si+1][sj]->getExistChild() &&
							(cardArr[si+1][sj]->getCardSex() == curSex))
				{
					rightCount++;
					si++;
				}

				int total = 0;
				if(topCount >= 3)
				{
					total+=(topCount-2);
					for(int k = 0; k < topCount; k++)
					{
						cardArr[i][j+k]->killSelf(true);
						cardBg[i][j+k]->setExistChild(false);
					}
				}
				if(bottomCount >= 3)
				{
					total+=(bottomCount-2);
					for(int k = 0; k < bottomCount; k++)
					{
						cardArr[i][j-k]->killSelf(true);
						cardBg[i][j-k]->setExistChild(false);
					}
				}
				if(leftCount >= 3)
				{
					total+=(leftCount-2);
					for(int k = 0; k < leftCount; k++)
					{
						cardArr[i-k][j]->killSelf(true);
						cardBg[i-k][j]->setExistChild(false);
					}
				}
				if(rightCount >= 3)
				{
					total+=(leftCount-2);
					for(int k = 0; k < rightCount; k++)
					{
						cardArr[i+k][j]->killSelf(true);
						cardBg[i+k][j]->setExistChild(false);
					}
				}
				cardArr[i][j]->setCancelCount(total);

				if(total != 0)
				{
					updateKilledBox();
					break;
				}
			}
		}
	}
}

void GameScene::updateKilledBox()
{
	float dt = 3.0;

	clearKillBox();

	ActionInterval* delay = DelayTime::create(dt);
	FiniteTimeAction *checkSameDone = CallFuncN::create(this, callfuncN_selector(GameScene::checkSameFinished));
	this->runAction(Sequence::create(delay, checkSameDone, NULL));
}

void GameScene::checkSameFinished(cocos2d::Node * sender)
{
	//updateSameBox();
}

void GameScene::clearKillBox()
{
	removedMaxId = -1;
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
			bool killed = cardArr[i][j]->getAlive();
			if(killed)
			{
				//cardBg[i][j]->setExistChild(false);
				spriteDisappearEffect(cardArr[i][j]);
				int id = i*cardSize+j;
				if(id > removedMaxId)
				{
					removedMaxId = id;
				}
			}
		}
	}
}

void GameScene::spriteDisappearFinished(cocos2d::Node * sender)
{
	CardSprite *sprite = (CardSprite*)sender;
    //this->removeChild(sprite, true);

    log("mygame  remove touchid: %d", sprite->getTouchId());

    if(removedMaxId == sprite->getTouchId())
    {
    	spriteDropDown();

    	// show add power sprite
    	if(sprite->getCancelCount() != 0)
    	{
    		armsPowerAddEffect(sprite);
    		sprite->setCancelCount(0);
    	}
    }
}

void GameScene::armsPowerAddEffect(CardSprite *sprite)
{
	int width = 100;
	int height = 100;
	CardSprite *armsCard = CardSprite::createCardSprite(0, 0, width, height, sprite->getPosition());
	armsCard->setCardSex((int)sprite->getCardSex());

	armsPowerMoveEffect(armsCard);

	this->addChild(armsCard);
}

void GameScene::armsPowerMoveEffect(CardSprite *target)
{
	float minDuration = 2.0;
	float x, y =0;

	Point spritePoint = target->getPosition();
	Point pos(spritePoint.x+200, spritePoint.y+100);

	FiniteTimeAction *actionMove = MoveTo::create(minDuration, pos);
	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::armsPowerMoveFinished));
	target->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
}

void GameScene::armsPowerMoveFinished(cocos2d::Node * sender)
{
	CardSprite *sprite = (CardSprite*)sender;
    //this->removeChild(sprite, true);

	armsPowerMoveToEndEffect(sprite);
}

void GameScene::armsPowerMoveToEndEffect(CardSprite *target)
{
	float minDuration = 0.5;

	int sex = (int)target->getCardSex();
	Point pos = armsCardArr[sex]->getPosition();

	FiniteTimeAction *actionMove = MoveTo::create(minDuration, pos);
	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::armsPowerMoveToEndFinished));
	target->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
}

void GameScene::armsPowerMoveToEndFinished(cocos2d::Node * sender)
{
	CardSprite *sprite = (CardSprite*)sender;
    this->removeChild(sprite, true);
}

void GameScene::spriteDropDown()
{
	log("mygame  spriteDropDown");
	for(int i = 0; i < cardSize; i++)
	{
		spriteDropDownWithColumn(i);
	}

	updateSameBox();
}

void GameScene::spriteDropDownWithColumn(int i)
{
	log("mygame spriteDropDownWithColumn i:%d", i);
	int ret = false;
	int maxId = -1;

	int removedCount = 0;

	int tempArr[cardSize];
	for(int j = 0; j < cardSize; j++)
	{
		bool exsitChild = cardBg[i][j]->getExistChild();
		if(!exsitChild)
		{
			tempArr[j] = 0;
		}
		else
		{
			tempArr[j] = 1;
		}
	}

	for(int j = 1; j< cardSize; j++)
	{
		int exsitChild = tempArr[j];
		if(exsitChild == 1)
		{
			// check whether exist child before j
			for(int k = 0; k < j; k++)
			{
				int child = tempArr[k];
				if(child == 0)
				{
					//cardArr[i][i*cardSize+j]->setMoveToId(i*cardSize+k);

					CardSprite *desCard = cardArr[i][k];
					CardSprite *srcCard = cardArr[i][j];
					cardArr[i][k] = srcCard;
					cardArr[i][k]->setTouchId(i*cardSize+k);
					cardArr[i][j] = desCard;
					cardArr[i][j]->setTouchId(i*cardSize+j);
					cardBg[i][k]->setExistChild(true);
					cardBg[i][j]->setExistChild(false);

					spriteDropEffect(i*cardSize+j, i*cardSize+k);
					tempArr[j] = 0;
					tempArr[k] = 1;
					break;
				}
			}
		}
	}

	// add new card from top

	for(int k = 0; k < cardSize; k++)
	{
		int existChild = tempArr[k];
		if(existChild == 0)
		{
			log("k: %d", k);

			CardSprite *card = CardSprite::createCardSprite(0, 0, cellSize, cellSize, getBufferPosition(i, 0));
			card->setTouchId(i);
			card->setCardSex(CCRANDOM_0_1()*SEXMAX);
			cardBufferArr[i][0] = card;
			this->addChild(card);

			CardSprite *desCard = cardArr[i][k];
			CardSprite *srcCard = cardBufferArr[i][0];
			cardArr[i][k] = srcCard;
			cardArr[i][k]->setTouchId(i*cardSize+k);
			cardBg[i][k]->setExistChild(true);

			Point desPoint = cardBg[i][k]->getPosition();

			spriteBufferDropEffect(desPoint, i*cardSize+k);
			this->removeChild(desCard, true);
			tempArr[k] = 1;
		}
	}
}


void GameScene::loopDropDown(float dt)
{
	log("loopDropDown  dt:%f,", dt);
	if(cardBufferArr[gDropX][0]->mySeq->isDone() && gDropX != -1)
	{
		log("loopDropDown  gDropX:%d,", gDropX);
		spriteDropDownWithColumn(gDropX);
	}

}

void GameScene::spriteDisappearEffect(CardSprite *target)
{
	float minDuration = 1.0;
	float x, y =0;

	Point pos = target->getPosition();

	int arrX = target->getTouchId() / cardSize;
	int arrY = target->getTouchId() % cardSize;
	Point destroyPos = cardBg[arrX][arrY]->getPosition();

	target->setVisible(false);

	DestroySprite *destroySprite = DestroySprite::createDestroySprite(cellSize, cellSize, destroyPos);
	this->addChild(destroySprite);

	FadeOut* fadeout = FadeOut::create(minDuration);
	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::spriteDisappearFinished));
	target->runAction(Sequence::create(fadeout,actionMoveDone,NULL));
}

void GameScene::updateClearBox(float dt)
{
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
			bool killed = cardArr[i][j]->getAlive();

			if(killed)
			{
				Point srcPos = cardBufferArr[i][0]->getPosition();
				Point desPos = cardBg[i][j]->getPosition();

				log("mygame clearBox  killed: i:%d, j:%d", i, j);
				//spriteBufferDropEffect(cardBufferArr[i][0], desPos);
				cardArr[i][j]->setPosition(srcPos);

				CardSprite *srcSprite = cardArr[i][j];
				CardSprite *desSprite = cardBufferArr[i][0];
				cardArr[i][j] = desSprite;
				cardBufferArr[i][0] = srcSprite;

				cardArr[i][j]->setTouchId(i*cardSize+j);
				cardBufferArr[i][0]->setTouchId(i*cardBufferH);

				updateBufferBox(i);
			}
		}
	}
}

bool GameScene::isExsitKilledCard()
{
	bool ret = false;
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
			bool killed = cardArr[i][j]->getAlive();

			if(killed)
			{
				ret = true;
			}
		}
	}

	return ret;
}


void GameScene::updateBufferBox(int i)
{
		int j = 0;
		bool killed = cardBufferArr[i][j]->getAlive();

		if(killed)
		{
			cardBufferArr[i][j]->setCardSex(CCRANDOM_0_1()*SEXMAX);
			cardBufferArr[i][j]->setCardMood(1);
			cardBufferArr[i][j]->setVisible(false);
			cardBufferArr[i][j]->killSelf(false);
		}
}

void GameScene::createCardNumber(bool animation)
{
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
			cardArr[i][j]->setCardSex(CCRANDOM_0_1()*SEXMAX);
			cardArr[i][j]->setCardMood(1);
			cardBg[i][j]->setExistChild(true);
//	            if(animation)
//	            {
//	                cardArr[i][j]->runNewNumberAction();
//	            }
		}
    }

//	for(int i = 0; i < cardSize; i++)
//	{
//		for(int j = 0; j < cardBufferH; j++)
//		{
//			cardBufferArr[i][j]->setCardSex(CCRANDOM_0_1()*SEXMAX);
//			cardBufferArr[i][j]->setCardMood(1);
//		}
//    }
}

bool GameScene::shouldCreateCardNumber()
{
    bool isCreate = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if( 0 == cardArr[i][j]->getCardMood() )
            {
                isCreate = true;
                break;
            }
        }
    }
    
    return isCreate;
}

bool GameScene::isWin()
{
    bool win = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if( 2048 == cardArr[i][j]->getCardMood() )
            {
                win = true;
                break;
            }
        }
    }
    
    return win;
}

int GameScene::getRandom()
{
	int ret = CCRANDOM_0_1() * 10;

	return ret;
}

void GameScene::getBeginPoint(CardSprite** card)
{
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
			Point targetPoint = getPosition(i, j);

			bool tap = this->isTap(targetPoint, beginPoint);
			if(tap)
			{
			    log("mygame getBeginPoint:  x:%d y:%d ",
			    		i, j);
				*card = cardArr[i][j];

				return;
			}
		}
	}
	return;
}


void GameScene::spriteMove(CardSprite *card, int dir)
{
	if(card == NULL)
	{
		log("mygame null");
		return;
	}

	int touchId = card->getTouchId();
	int startX = touchId / cardSize;
	int startY = touchId % cardSize;

	int DesX = startX;
	int DesY = startY;

    switch (dir)
    {
        case SimpleGesturesLeft:
        	DesX--;
            break;
        case SimpleGesturesRight:
        	DesX++;
            break;
        case SimpleGesturesUp:
        	DesY++;
            break;
        case SimpleGesturesDown:
        	DesY--;
            break;
        default:
        	return;
    }

    log("mygame: touchId: %d  %d %d    %d %d   %d" , touchId, startX, startY, DesX, DesY, dir);

    if((DesX >=0 && DesX < cardSize) && (DesY >=0 && DesY < cardSize))
    {
		Point desPos = cardBg[DesX][DesY]->getPosition();
		Point startPos = cardBg[startX][startY]->getPosition();
		float desPosX = desPos.x;
		float desPosY = desPos.y;
		float startPosX = startPos.x;
		float startPosY = startPos.y;

    	log("mygame: move position [%d  %d]  %f  %f    %f  %f" , cardArr[startX][startY]->getCardMood(), cardArr[DesX][DesY]->getCardMood(), startPosX, startPosY, desPosX, desPosY);

    	if(1)
    	{
    		spriteMoveEffect(cardArr[startX][startY], desPos);
    		spriteMoveEffect(cardArr[DesX][DesY], startPos);
    	}
    	else
    	{
    		cardArr[startX][startY]->setPosition(desPos);
    		cardArr[DesX][DesY]->setPosition(startPos);
    	}

    	swapSpritPos(cardArr[startX][startY], cardArr[DesX][DesY]);
    }
}

void GameScene::swapSpritPos(CardSprite *srcCard, CardSprite *desCard)
{

	int srcTouchId = srcCard->getTouchId();
	int startX = srcTouchId / cardSize;
	int startY = srcTouchId % cardSize;

	int desTouchId = desCard->getTouchId();
	int desX = desTouchId / cardSize;
	int desY = desTouchId % cardSize;

	CardSprite *temp = srcCard;
	cardArr[startX][startY] = desCard;
	cardArr[desX][desY] = srcCard;

	cardArr[startX][startY]->setTouchId(srcTouchId);
	cardArr[desX][desY]->setTouchId(desTouchId);

	log("mygame: !!!!!! number [%d  %d]  touchid %d   %d " , cardArr[startX][startY]->getCardMood(),
				cardArr[desX][desY]->getCardMood(), cardArr[startX][startY]->getTouchId(), cardArr[desX][desY]->getTouchId());
}

void GameScene::spriteMoveFinished(cocos2d::Node * sender)
{
	updateSameBox();
	//infectCard();
}

void GameScene::spriteDropFinished(cocos2d::Node * sender)
{
	CardSprite *sprite = (CardSprite*)sender;

	int touchId = sprite->getTouchId();
	int cardX = touchId / cardSize;
	int cardY = touchId % cardSize;

	if((cardY-1) >= 0)
	{
		log("mygame: touchId:%d  x:%d  y: %d", touchId, cardX, cardY);
		cardArr[cardX][cardY-1] = cardArr[cardX][cardY];
		cardArr[cardX][cardY-1]->setTouchId(touchId-1);
		cardBg[cardX][cardY-1]->setExistChild(true);
	}
}

void GameScene::spriteBufferDropFinished(cocos2d::Node * sender)
{
	log("mygame: spriteBufferDropFinished");
	CardSprite *sprite = (CardSprite*)sender;

	int touchId = sprite->getTouchId();
	int cardX = touchId;

//	log("mygame: spriteBufferDropFinished   X:%d", cardX);
//	cardArr[cardX][cardSize-1] = sprite;
//	cardArr[cardX][cardSize-1]->setTouchId(cardX*cardSize+cardSize-1);
//	cardBg[cardX][cardSize-1]->setExistChild(true);

    log("mygame: spriteBufferDropFinished   done");
}

void GameScene::spriteDropDoneFinished(cocos2d::Node * sender)
{
	log("mygame: spriteDropDoneFinished");
	CardSprite *sprite = (CardSprite*)sender;
	int touchId = sprite->getTouchId();
	int cardX = touchId/cardSize;

	log("mygame: spriteDropDoneFinished touchId:%d", touchId);
}

void GameScene::infectCard()
{
	for(int i = 0; i < cardSize; i++)
	{
		for(int j = 0; j < cardSize; j++)
		{
				int mood = cardArr[i][j]->getCardMood();

				int happyMood = (int)HAPPY;

				if(mood == happyMood)
				{
					int topInfect = -1;
					int leftInfect = -1;
					int rightInfect = -1;
					int bottomInfect = -1;

					// check top infected
					if((j+1) < cardSize && cardArr[i][j+1]->getCardMood() != happyMood)
					{
						topInfect = 0;
					}
					// check bottom infected
					if((j-1) > 0 && cardArr[i][j-1]->getCardMood() != happyMood)
					{
						bottomInfect = 1;
					}
					// check right infected
					if((i+1) < cardSize && cardArr[i+1][j]->getCardMood() != happyMood)
					{
						rightInfect = 2;
					}
					if((i-1) > 0 && cardArr[i-1][j]->getCardMood() != happyMood)
					{
						leftInfect = 3;
					}

					int infectDir = CCRANDOM_0_1()*4;
					if(infectDir == topInfect)
					{
						cardArr[i][j+1]->setCardMood(happyMood);
					}
					else if(infectDir == bottomInfect)
					{
						cardArr[i][j-1]->setCardMood(happyMood);
					}
					else if(infectDir == rightInfect)
					{
						cardArr[i+1][j]->setCardMood(happyMood);
					}
					else if(infectDir == leftInfect)
					{
						cardArr[i-1][j]->setCardMood(happyMood);
					}
				}
		}
    }
}

void GameScene::spriteMoveEffect(CardSprite *target, Point position)
{
	float minDuration = 0.2;

	FiniteTimeAction *actionMove = MoveTo::create(minDuration, position);
	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::spriteMoveFinished));
	target->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
}

void GameScene::spriteBufferDropEffect(Point pos, int desTouchId)
{
	float minDuration = 0.3;

	int desX = desTouchId / cardSize;
	int desY = desTouchId % cardSize;

	Point jumpPos = Point(0, 0);
	FiniteTimeAction *actionMove = MoveTo::create(minDuration, pos);
	FiniteTimeAction *actionJump = JumpBy::create(0.3, jumpPos, 10, 1);
	cardArr[desX][desY]->runAction(Sequence::create(actionMove,actionJump,NULL));
}

void GameScene::spriteDropEffect(int srcTouchId, int desTouchId)
{
	float minDuration = 0.3;

	int srcX = srcTouchId / cardSize;
	int srcY = srcTouchId % cardSize;
	int desX = desTouchId / cardSize;
	int desY = desTouchId % cardSize;
	int mvCount = srcTouchId-desTouchId;

	Point pos = cardBg[desX][desY]->getPosition();
	Point jumpPos = Point(0, 0);
	FiniteTimeAction *actionMove = MoveTo::create(minDuration, pos);
	FiniteTimeAction *actionJump = JumpBy::create(0.3, jumpPos, 10, 1);
	cardArr[desX][desY]->runAction(Sequence::create(actionMove,actionJump,NULL));

//	FiniteTimeAction *actionMoveDone = CallFuncN::create(this, callfuncN_selector(GameScene::spriteDropFinished));
//	target->runAction(Sequence::create(actionMove,actionMoveDone,NULL));
}

void GameScene::setScore(int score)
{
    cardNumberTTF->setString(__String::createWithFormat("%i",score)->getCString());
}

void GameScene::doCheck()
{
    bool isGameOver = true;

    return;


    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (cardArr[x][y]->getCardMood() == 0 ||
                (x<3 && cardArr[x][y]->getCardMood() == cardArr[x+1][y]->getCardMood()) ||
                (x>0 && cardArr[x][y]->getCardMood() == cardArr[x-1][y]->getCardMood()) ||
                (y<3 && cardArr[x][y]->getCardMood() == cardArr[x][y+1]->getCardMood()) ||
                (y>0 && cardArr[x][y]->getCardMood() == cardArr[x][y-1]->getCardMood()) )
            {
                isGameOver = false;
            }
        }
    }

    if (isWin()) {
        
        successLayer = LayerColor::create(Color4B(0, 0, 0, 180));
        Size winSize = Director::getInstance()->getWinSize();
        Point centerPos = Point(winSize.width / 2, winSize.height / 2);
        auto gameOverTitle = Label::createWithSystemFont("YOU WIN","Consolas",80);
        gameOverTitle->setPosition(centerPos);
        successLayer->addChild(gameOverTitle);
        
        getParent()->addChild(successLayer,1);
        
        scheduleOnce(SEL_SCHEDULE(&GameScene::removeSuccessLayer), 2);
        return;
    }
    
    //isGameOver = true;
    if (isGameOver)
    {
        log("game over");
        UserDefault::getInstance()->setBoolForKey("history", false);

        HighScore::getInstance()->setScore(score);
        GameOverLayer *gameoverLayer = GameOverLayer::create(Color4B(0, 0, 0, 180));
        getParent()->addChild(gameoverLayer,1);
        
        Director::getInstance()->pause();
    }
    else
    {
        if (shouldCreateCardNumber()) {
            createCardNumber();
            
            saveStatus();
        }
    }
    
}

Point GameScene::getPosition(int i, int j)
{
    float pX = layerBorder + cellSpace/2 + i*(cellSize+cellSpace);
    float pY = controlLayerHeight + cellSize/2 + j*(cellSize+cellSpace);
    
    return Point(pX,pY);
}

Point GameScene::getBufferPosition(int i, int j)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	int h = controlLayerHeight + cellSize/2 + cardSize*(cellSize+cellSpace);

    float pX = layerBorder + cellSpace/2 + i*(cellSize+cellSpace);
    float pY = h + cellSize/2 + j*(cellSize+cellSpace);

    return Point(pX,pY);
}


void GameScene::saveStatus()
{
    char temp[10];
    //4*4
    for (int i = 0; i<4; i++) {
        for(int j = 0; j<4; j++)
        {
            sprintf(temp,"%d%d",i,j);
            UserDefault::getInstance()->setIntegerForKey(temp, cardArr[i][j]->getCardMood());
        }
    }
    
    UserDefault::getInstance()->setIntegerForKey("score", score);
    
    UserDefault::getInstance()->setBoolForKey("history", true);
}

void GameScene::resumeStatus()
{
    char temp[10];
    //4*4
    for (int i = 0; i<4; i++) {
        for(int j = 0; j<4; j++)
        {
            sprintf(temp,"%d%d",i,j);
            int number = UserDefault::getInstance()->getIntegerForKey(temp);
            //cardArr[i][j]->setNumber(number);
        }
    }
    
    score = UserDefault::getInstance()->getIntegerForKey("score");
    setScore(score);
    
    UserDefault::getInstance()->setBoolForKey("history", false);
}

void GameScene::removeSuccessLayer()
{
    successLayer->removeFromParent();
}
