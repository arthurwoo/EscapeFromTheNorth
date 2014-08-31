#include "LevelScene.h"
#include "LevelLayer.h"
#include "LevelSelectPage.h"
#include "UIScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define LAYER_NUM (3)

LevelScene* LevelScene::instance;

LevelScene* LevelScene::getInstance()
{
	if(instance != NULL)
		return instance;

	return NULL;
}

bool LevelScene::init()
{
	if(!Scene::init())
		return false;

	instance = this;

	Size size = Director::getInstance()->getWinSize();

	LevelLayer* scrollView = LevelLayer::create();

	for(int i = 0; i < LAYER_NUM; i++)
	{
		auto page = LevelSelectPage::create("selectLevelBg.png", i);
		page->setTag(i);
		scrollView->addNode(page);
	}

	Sprite* spriteBg = Sprite::create("Plain.png");
	spriteBg->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(spriteBg);

	auto closeItem = MenuItemImage::create("btnBackIcon.png", "btnBackIconOver.png", 
											CC_CALLBACK_1(LevelScene::menuCloseCallback, this));

	closeItem->setPosition(Point(40, 40));
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 1);

	this->addChild(scrollView);

	auto width = Sprite::createWithSpriteFrameName("pageMarkerBg.png")->getContentSize().width;
	auto posX = (size.width - width * LAYER_NUM) / 2;
	for(int i = 0; i < LAYER_NUM; i++)
	{
		Sprite* sprite = Sprite::createWithSpriteFrameName("pageMarkerBg.png");
		sprite->setPosition(Point(posX + i * width, size.height / 7));
		addChild(sprite);
	}

	Sprite* pageMarker = Sprite::createWithSpriteFrameName("pageMarker.png");
	pageMarker->setPosition(Point(posX, size.height / 7));
	pageMarker->setTag(888);
	addChild(pageMarker);

	return true;
}

void LevelScene::menuCloseCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);

	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, UIScene::createScene()));
}