#include "LevelSelectPage.h"
#include "LevelInfoScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define LEVEL_ROW (2)
#define LEVEL_COL (3)

LevelSelectPage* LevelSelectPage::create(const std::string& bgName, int level)
{
	LevelSelectPage* pRet = new LevelSelectPage();
	if(pRet && pRet->initLevelPage(bgName, level))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool LevelSelectPage::initLevelPage(const std::string& bgName, int level)
{
	if(!Node::init())
		return false;

	Size size = Director::getInstance()->getWinSize();
	Sprite* sprite = Sprite::create(bgName);
	sprite->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(sprite, -2);

	Vector<MenuItem*> menuItemVector;
	auto buttonSize = Sprite::create("card_1.png")->getContentSize();
	auto gap = buttonSize.width / 4;
	auto startWidth = 0;//(size.width - LEVEL_ROW * buttonSize.width - (LEVEL_ROW - 1) * gap) / 2;
	auto startHeight = 0;//(size.height + LEVEL_COL * buttonSize.height + (LEVEL_COL - 1) * gap) / 2 - buttonSize.height;

	for(int row = 0; row < LEVEL_ROW; row++)
	{
		auto height = startHeight - (buttonSize.height + gap) * row;
		for(int col = 0; col < LEVEL_COL; col++)
		{
			auto width = startWidth + (buttonSize.width + gap) * col;
			auto item = MenuItemImage::create("card_1.png", "card_2.png", "card_3.png", 
												CC_CALLBACK_1(LevelSelectPage::menuStartCallback, this));
			item->setAnchorPoint(Point(0, 0));
			item->setPosition(Point(width, height));
			item->setTag(row * LEVEL_ROW + col + level * LEVEL_ROW * LEVEL_COL);
			menuItemVector.pushBack(item);

			auto levelNum = UserDefault::getInstance()->getIntegerForKey("levelNum");
			if(levelNum < row * LEVEL_ROW + col + level * LEVEL_ROW * LEVEL_COL)
				item->setEnabled(false);
		}
	}

	auto levelMenu = Menu::createWithArray(menuItemVector);
	levelMenu->setPosition(Point(gap / 3 * 2, buttonSize.height + gap * 2));
	sprite->addChild(levelMenu);

	return true;
}

void LevelSelectPage::menuStartCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);

	auto button = (Sprite*)pSender;

	char buffer[20] = {};
	sprintf(buffer, "levelInfo_%d.plist", button->getTag());
	std::string strName = buffer;
	UserDefault::getInstance()->setStringForKey("nextLevelFile", strName);

	Scene* scene = Scene::create();
	Layer* layer = LevelInfoScene::create();
	scene->addChild(layer);

	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, scene));
}