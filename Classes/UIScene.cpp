#include "UIScene.h"
#include "LevelScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* UIScene::createScene()
{
	Scene* scene = Scene::create();
	UIScene* layer = UIScene::create();
	scene->addChild(layer);

	return scene;
}

bool UIScene::init()
{
	if(!Layer::init())
		return false;

	Size size = Director::getInstance()->getWinSize();
	Sprite* sprite = Sprite::create("Plain.png");
	sprite->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(sprite, -1);

	Sprite* titleSprite = Sprite::createWithSpriteFrameName("title.png");
	titleSprite->setPosition(Point(size.width / 3, size.height / 3 * 2));
	this->addChild(titleSprite);
	auto move = MoveBy::create(1.0f, Point(0, 10));
	titleSprite->runAction(RepeatForever::create(Sequence::create(move, move->reverse(), NULL)));

	auto startItem = MenuItemImage::create("start_1.png", "start_2.png",
											CC_CALLBACK_1(UIScene::menuStartCallback, this));
	startItem->setPosition(Point((size.width - startItem->getContentSize().width) / 2, size.height / 6));
	startItem->setAnchorPoint(Point(0, 0));

	auto menu = Menu::create(startItem, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 1);

	return true;
}

void UIScene::menuStartCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);

	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, LevelScene::create()));
}