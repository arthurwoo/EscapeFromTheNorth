#include "EnemyInfoLayer.h"

bool EnemyInfoLayer::init()
{
	if(!Layer::init())
		return false;

	Size size = Director::getInstance()->getWinSize();

	spriteBg = Sprite::create("selectLevelBg.png");
	spriteBg->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(spriteBg);

	return true;
}

void EnemyInfoLayer::setEnemyInfo(std::string enemyName, ValueMap info)
{
	Sprite* graph = Sprite::createWithSpriteFrameName(info["defaultImage"].asString());
	graph->setPosition(Point(20, spriteBg->getContentSize().height - 20));
	spriteBg->addChild(graph);
	graph->runAction(RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(enemyName + "rundown"))));

	ValueMap chineseDict = FileUtils::getInstance()->getValueMapFromFile("chinese.plist");
	TTFConfig config("fonts/trends.ttf", 12);
	auto nameLabel = Label::createWithTTF(config, info["name"].asString());
	nameLabel->setAnchorPoint(Point(0, 0.5));
	nameLabel->setPosition(Point(graph->getPositionX() - graph->getContentSize().width / 2, graph->getPositionY() - graph->getContentSize().height));
	spriteBg->addChild(nameLabel);

	auto hpLabel = Label::createWithTTF(config, chineseDict["hp"].asString() + info["mhp"].asString());
	hpLabel->setAnchorPoint(Point(0, 0.5));
	hpLabel->setPosition(Point(graph->getPositionX() + graph->getContentSize().width + 10, graph->getPositionY()));
	spriteBg->addChild(hpLabel);

	auto descLabel = Label::createWithTTF(config, info["desc"].asString());
	descLabel->setWidth(spriteBg->getContentSize().width - 20);
	descLabel->setAnchorPoint(Point(0, 0.5));
	descLabel->setPosition(Point(nameLabel->getPositionX(), nameLabel->getPositionY() - nameLabel->getContentSize().height - 10));
	spriteBg->addChild(descLabel);
}