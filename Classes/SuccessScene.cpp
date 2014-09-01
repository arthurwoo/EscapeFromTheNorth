#include "SuccessScene.h"
#include "GameManager.h"
#include "SimpleAudioEngine.h"
#include "LevelInfoScene.h"
#include "LevelScene.h"

using namespace CocosDenshion;

bool SuccessScene::init()
{
	if(!Scene::init())
		return false;

	Size size = Director::getInstance()->getWinSize();
	GameManager* instance = GameManager::getInstance();
	instance->clear();

	Sprite* sprite = Sprite::create("Plain.png");
	sprite->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(sprite, -1);

	Sprite* nextNormal = Sprite::createWithSpriteFrameName("btnNext.png");
	Sprite* nextOver = Sprite::createWithSpriteFrameName("btnNextOver.png");
	MenuItemSprite* nextItem = MenuItemSprite::create(nextNormal, nextOver, CC_CALLBACK_1(SuccessScene::menuNextCallback, this));

	Sprite* backNormal = Sprite::createWithSpriteFrameName("btnBack.png");
	Sprite* backOver = Sprite::createWithSpriteFrameName("btnBackOver.png");
	MenuItemSprite* backItem = MenuItemSprite::create(backNormal, backOver, CC_CALLBACK_1(SuccessScene::menuCloseCallback, this));

	Menu* menu = Menu::create(backItem, nextItem, NULL);
	menu->alignItemsHorizontally();
	menu->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(menu);

	return true;
}

void SuccessScene::menuNextCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);
	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, LevelInfoScene::createScene()));
}

void SuccessScene::menuCloseCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);
	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, LevelScene::create()));
}