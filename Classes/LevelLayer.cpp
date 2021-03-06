#include "LevelLayer.h"
#include "LevelScene.h"

USING_NS_CC;

LevelLayer::LevelLayer(): pageNode(0), curPageNode(0)
{
}

LevelLayer::~LevelLayer()
{
}

bool LevelLayer::init()
{
	if(!Layer::init())
		return false;

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(LevelLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(LevelLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LevelLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	WINDOW_WIDTH = Director::getInstance()->getWinSize().width;
	WINDOW_HEIGHT = Director::getInstance()->getWinSize().height;

	return true;
}

bool LevelLayer::onTouchBegan(Touch* touch, Event* event)
{
	touchDownPoint = touch->getLocation();
	touchCurPoint = touchDownPoint;

	return true;
}

void LevelLayer::onTouchMoved(Touch* touch, Event* event)
{
	Point touchPoint = touch->getLocation();
	auto curX = this->getPositionX() + touchPoint.x - touchCurPoint.x;
	Point posPoint = Point(curX, this->getPositionY());
	auto dis = fabsf(touchPoint.x - touchCurPoint.x);

	if(dis >= SHORTEST_SLIDE_LENGTH)
		setPosition(posPoint);

	touchCurPoint = touchPoint;
}

void LevelLayer::onTouchEnded(Touch* touch, Event* event)
{
	touchUpPoint = touch->getLocation();
	auto dis = touchUpPoint.getDistance(touchDownPoint);
	auto spriteMarker = Sprite::createWithSpriteFrameName("pageMarker.png");
	auto width = spriteMarker->getContentSize().width;

	if(dis >= SHORTEST_SLIDE_LENGTH)
	{
		int offset = getPositionX() - curPageNode * (-WINDOW_WIDTH);
		if(offset > width)
		{
			if(curPageNode > 0)
			{
				--curPageNode;
				Sprite* sprite = (Sprite*)LevelScene::getInstance()->getChildByTag(888);
				sprite->setPosition(Point(sprite->getPositionX() - width, sprite->getPositionY()));
			}
		}
		else if(offset < -width)
		{
			if(curPageNode < (pageNode - 1))
			{
				++curPageNode;
				Sprite* sprite = (Sprite*)LevelScene::getInstance()->getChildByTag(888);
				sprite->setPosition(Point(sprite->getPositionX() + width, sprite->getPositionY()));
			}
		}

		gotoCurNode();
	}
}

void LevelLayer::gotoCurNode()
{
	this->runAction(MoveTo::create(0.4f, Point(-curPageNode * WINDOW_WIDTH, 0)));
}

void LevelLayer::addNode(Node* level)
{
	if(!level)
		return;

	level->setContentSize(Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	level->setPosition(Point(pageNode * WINDOW_WIDTH, 0));
	this->addChild(level);
	pageNode++;
}