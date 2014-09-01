#include "FailScene.h"
#include "GameManager.h"
#include "SimpleAudioEngine.h"
#include "LevelScene.h"

USING_NS_CC;
using namespace CocosDenshion;

bool FailScene::init()
{
	if(!Scene::create())
		return false;

	Size size = Director::getInstance()->getWinSize();
	GameManager* instance = GameManager::getInstance();
	instance->clear();

	Sprite* sprite = Sprite::create("Plain.png");
	sprite->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(sprite, -1);

	Sprite* backNormal = Sprite::createWithSpriteFrameName("btnBack.png");
	Sprite* backOver = Sprite::createWithSpriteFrameName("btnBackOver.png");
	MenuItemSprite* backItem = MenuItemSprite::create(backNormal, backOver, CC_CALLBACK_1(FailScene::menuBackCallback, this));
	
	Menu* menu = Menu::create(backItem, NULL);
	menu->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(menu);

	return true;
}

void FailScene::menuBackCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelScene::create()));
}