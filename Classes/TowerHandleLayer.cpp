#include "TowerHandleLayer.h"

bool TowerHandleLayer::init()
{
	if(!Layer::init())
		return false;

	setFuncName(TowerFunc::NOTHING);

	Size size = Director::getInstance()->getWinSize();

	Sprite* upgradeSprite = Sprite::createWithSpriteFrameName("btnUpgrade.png");
	Sprite* upgradeSpriteOver = Sprite::createWithSpriteFrameName("btnUpgradeOver.png");
	MenuItemSprite* upgradeItem = MenuItemSprite::create(upgradeSprite, upgradeSpriteOver, CC_CALLBACK_1(TowerHandleLayer::menuUpgradeCallback, this));

	Sprite* destroySprite = Sprite::createWithSpriteFrameName("btnDestroy.png");
	Sprite* destroySpriteOver = Sprite::createWithSpriteFrameName("btnDestroyOver.png");
	MenuItemSprite* destroyItem = MenuItemSprite::create(destroySprite, destroySpriteOver, CC_CALLBACK_1(TowerHandleLayer::menuDestroyCallback, this));

	Menu* menu = Menu::create(upgradeItem, destroyItem, NULL);
	menu->setPosition(Point(0, 0));
	menu->alignItemsHorizontallyWithPadding(32);
	this->addChild(menu);

	return true;
}

void TowerHandleLayer::menuUpgradeCallback(Ref* pSender)
{
	funcName = UPGRADE;
}

void TowerHandleLayer::menuDestroyCallback(Ref* pSender)
{
	funcName = DESTROY;
}