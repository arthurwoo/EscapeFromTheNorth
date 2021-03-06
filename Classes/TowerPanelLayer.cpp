#include "TowerPanelLayer.h"

bool TowerPanelLayer::init()
{
	if(!Layer::init())
		return false;

	setChooseType(TowerType::OTHER);

	auto sprite = Sprite::createWithSpriteFrameName("towerPos.png");
	sprite->setPosition(Point(0, 0));
	this->addChild(sprite);

	sprite1 = Sprite::createWithSpriteFrameName("soldierBtn.png");
	sprite1->setAnchorPoint(Point(0, 0));
	sprite1->setPosition(Point(-sprite->getContentSize().width, sprite->getContentSize().height));
	sprite->addChild(sprite1);

	sprite2 = Sprite::createWithSpriteFrameName("knightBtn.png");
	sprite2->setAnchorPoint(Point(0, 0));
	sprite2->setPosition(Point(sprite->getContentSize().width, sprite->getContentSize().height));
	sprite->addChild(sprite2);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(TowerPanelLayer::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(TowerPanelLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, sprite1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), sprite2);

	return true;
}

bool TowerPanelLayer::onTouchBegan(Touch* touch, Event* event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	Point locationInNode = target->convertTouchToNodeSpace(touch);
	Size size = target->getContentSize();
	Rect rect = Rect(0, 0, size.width, size.height);

	if(rect.containsPoint(locationInNode))
	{
		target->setOpacity(180);
		return true;
	}

	return false;
}

void TowerPanelLayer::onTouchEnded(Touch* touch, Event* event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	if(target == sprite1)
		chooseType = SOLDIER_TOWER;
	else if(target == sprite2)
		chooseType = KNIGHT_TOWER;
	else
		chooseType = OTHER;
}