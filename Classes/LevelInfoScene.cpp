#include "LevelInfoScene.h"
#include "PlayLayer.h"
#include "LevelScene.h"
#include "GameManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define STAR_NUMBER (3)

LevelInfoScene::LevelInfoScene(): info(NULL), fileName("")
{
}

Scene* LevelInfoScene::createScene()
{
	Scene* scene = Scene::create();
	LevelInfoScene* layer = LevelInfoScene::create();
	scene->addChild(layer);
	
	return scene;
}

bool LevelInfoScene::init()
{
	if(!Layer::init())
		return false;

	auto instance = GameManager::getInstance();
	instance->clear();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Play.plist");
	fileName = UserDefault::getInstance()->getStringForKey("nextLevelFile");
	if(fileName == "")
		fileName = "levelInfo_0.plist";

	Size size = Director::getInstance()->getWinSize();

	//TODO:Ìí¼ÓÐÇ¼¶

	addBackGround();

	return true;
}

void LevelInfoScene::addBackGround()
{
	Size size = Director::getInstance()->getWinSize();
	GameManager* instance = GameManager::getInstance();

	Sprite* spriteBg = Sprite::create("Plain.png");
	spriteBg->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(spriteBg, -2);

	Sprite* spritePanel = Sprite::create("levelInfoPanel.png");
	spritePanel->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(spritePanel, -1);

	ValueMap levelInfo = FileUtils::getInstance()->getValueMapFromFile(fileName.c_str())["levelInfo"].asValueMap();
	auto levelDesc = levelInfo["levelDesc"].asString();
	TTFConfig config("fonts/trends.ttf", 20);
	auto labelDesc = Label::createWithTTF(config, levelDesc);
	labelDesc->setPosition(size.width / 2, size.height / 2 + labelDesc->getContentSize().height / 2);
	this->addChild(labelDesc);

	Sprite* start = Sprite::createWithSpriteFrameName("btnStart.png");
	Sprite* startOver = Sprite::createWithSpriteFrameName("btnStartOver.png");
	MenuItemSprite* startItem = MenuItemSprite::create(start, startOver, CC_CALLBACK_1(LevelInfoScene::menuStartCallback, this));
	Sprite* back = Sprite::createWithSpriteFrameName("btnBack.png");
	Sprite* backOver = Sprite::createWithSpriteFrameName("btnBackOver.png");
	MenuItemSprite* backItem = MenuItemSprite::create(back, backOver, CC_CALLBACK_1(LevelInfoScene::menuBackCallback, this));

	Menu* menu = Menu::create(startItem, backItem, NULL);
	menu->alignItemsHorizontally();
	menu->setPosition(Point(size.width / 2, size.height / 2 - start->getContentSize().height));
	this->addChild(menu);
}

void LevelInfoScene::menuStartCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);

	info = LoadLevelInfo::createLoadLevelInfo(fileName.c_str());
	info->readLevelInfo();
	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, PlayLayer::createScene()));

	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName.c_str());
	ValueMap resource = FileUtils::getInstance()->getValueMapFromFile(fullPath)["resources"].asValueMap();
	SimpleAudioEngine::getInstance()->playBackgroundMusic(FileUtils::getInstance()->fullPathForFilename(resource["bgm"].asString()).c_str(), true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
}

void LevelInfoScene::menuBackCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);

	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, LevelScene::create()));
}