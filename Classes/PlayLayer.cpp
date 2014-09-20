#define MAP_WIDTH (23)
#define MAP_HEIGHT (15)

#include "PlayLayer.h"
#include "GameManager.h"
#include "LoadLevelInfo.h"
#include "SimpleAudioEngine.h"
#include "LevelScene.h"
#include "SuccessScene.h"
#include "FailScene.h"

using namespace CocosDenshion;

PlayLayer::PlayLayer():spriteSheet(NULL), map(NULL), objects(NULL), pointsVector(NULL), money(0), 
						chooseTowerPanel(NULL), towerMatrix(NULL), groupCounter(0), isSuccessful(false), playerHp(100),
						playerHpBar(NULL), moneyLabel(NULL), groupLabel(NULL), playerHpBg(NULL), handleTowerPanel(NULL),
						enemyInfoPanel(NULL), rangeSprite(NULL)
{
}

PlayLayer::~PlayLayer()
{
	if(towerMatrix)
		free(towerMatrix);

	pointsVector.clear();
}

Scene* PlayLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = PlayLayer::create();
	scene->addChild(layer);
	return scene;
}

bool PlayLayer::init()
{
	if(!Layer::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();
	instance = GameManager::getInstance();

	map = TMXTiledMap::create(instance->getCurMapName());
	bgLayer = map->getLayer("bg");
	bgLayer->setAnchorPoint(Point(0.5f, 0.5f));
	bgLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
	ftLayer = map->getLayer("ft");
	ftLayer->setAnchorPoint(Point(0.5f, 0.5f));
	ftLayer->setPosition(Point(winSize.width / 2, winSize.height / 2));
	objects = map->getObjectGroup("obj");
	this->addChild(map, -1);

	initToolLayer();

	offX = (map->getContentSize().width - winSize.width) / 2;
	initPointsVector(offX);
	schedule(schedule_selector(PlayLayer::logic), 2.0f);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	scheduleUpdate();

	int arraySize =	sizeof(TowerBase*) * MAP_WIDTH * MAP_HEIGHT;
	towerMatrix = (TowerBase**)malloc(arraySize);
	memset((void*)towerMatrix, 0, arraySize);

	for(int row = 0; row < MAP_HEIGHT; row++)
		for(int col = 0; col < MAP_WIDTH; col++)
			towerMatrix[row * MAP_WIDTH + col] = NULL;

	return true;
}

void PlayLayer::initPointsVector(float offX)
{
	Node* runOfPoint = NULL;
	int count = 0;
	ValueMap point = objects->getObject(std::to_string(count));
	while(point.begin() != point.end())
	{
		float x = point.at("x").asFloat();
		float y = point.at("y").asFloat();

		runOfPoint = Node::create();
		runOfPoint->setPosition(Point(x - offX, y));
		this->pointsVector.pushBack(runOfPoint);
		count++;
		point = objects->getObject(std::to_string(count));
	}
	runOfPoint = NULL;
}

void PlayLayer::addEnemy()
{
	GroupEnemy* groupEnemy = currentGroup();

	if(!groupEnemy)
		return;

	auto restEnemyNum = groupEnemy->getEnemyTotal();
	if(restEnemyNum <= 0)
	{
		groupEnemy->setIsFinishAddedGroup(true);
		return;
	}

	restEnemyNum--;
	groupEnemy->setEnemyTotal(restEnemyNum);

	EnemyBase* enemy = NULL;

	ValueMap typeMap = groupEnemy->getTypeMap();
	for(auto it = typeMap.begin(); it != typeMap.end(); it++)
	{
		ValueMap& gInfo = it->second.asValueMap();
		int typeNum = gInfo["typeNum"].asInt();
		std::string typeName = gInfo["typeName"].asString();
		if(typeNum > 0)
		{
			SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/appear.wav").c_str(), false);

			enemy = EnemyBase::create();
			enemy->setEnemyInfo(typeName, pointsVector);

			gInfo["typeNum"] = typeNum - 1;
			break;
		}
	}
	groupEnemy->setTypeMap(typeMap);

	this->addChild(enemy, 10);
	instance->enemyVector.pushBack(enemy);
}

void PlayLayer::addTower()
{
	if(!chooseTowerPanel)
		return;

	auto type = chooseTowerPanel->getChooseType();
	if(type == TowerType::OTHER)
		return;

	Point matrixCoord = convertToMatrixCoord(towerPos);
	int matrixIndex = static_cast<int>(matrixCoord.y * MAP_WIDTH + matrixCoord.x);
	TowerBase* tower = NULL;

	int price = 0;
	if(type == TowerType::SOLDIER_TOWER)
	{
		price = FileUtils::getInstance()->getValueMapFromFile("tower.plist")["soldier"].asValueMap()["lv1Info"].asValueMap()["price"].asInt();
		if(money >= price)
		{
			tower = TowerBase::create();
			tower->setTowerInfo("soldier");
		}
	}
	else if(type == TowerType::KNIGHT_TOWER)
	{
		price = FileUtils::getInstance()->getValueMapFromFile("tower.plist")["knight"].asValueMap()["lv1Info"].asValueMap()["price"].asInt();
		if(money >= price)
		{
			tower = TowerBase::create();
			tower->setTowerInfo("knight");
		}
	}
	
	money -= price;
	ValueMap chineseDict = FileUtils::getInstance()->getValueMapFromFile("chinese.plist");
	moneyLabel->setString(chineseDict["money"].asString() + std::to_string(money));

	if(tower)
	{
		tower->setPosition(towerPos);
		tower->runAction(Sequence::create(FadeIn::create(1.0f), NULL));
		this->addChild(tower);
		towerMatrix[matrixIndex] = tower;
	}

	type = TowerType::OTHER;
	chooseTowerPanel->setChooseType(type);
	this->removeChild(chooseTowerPanel);
	chooseTowerPanel = NULL;
}

bool PlayLayer::onTouchBegan(Touch* touch, Event* event)
{
	if(chooseTowerPanel != NULL)
	{
		this->removeChild(chooseTowerPanel);
		chooseTowerPanel = NULL;
	}

	if(handleTowerPanel != NULL)
	{
		this->removeChild(handleTowerPanel);
		handleTowerPanel = NULL;
	}

	if(enemyInfoPanel != NULL)
	{
		this->removeChild(enemyInfoPanel);
		enemyInfoPanel = NULL;
	}

	if(rangeSprite != NULL)
	{
		this->removeChild(rangeSprite);
		rangeSprite = NULL;
	}

	auto size = toolLayer->getChildByTag(1)->getContentSize();
	Rect toolRect = Rect(offX,  toolLayer->getChildByTag(1)->getPositionY() - size.height,
							size.width, size.height);

	if(toolRect.containsPoint(convertTouchToNodeSpace(touch)))
		return false;

	for(auto it = instance->enemyVector.begin(); it != instance->enemyVector.end(); it++)
	{
		EnemyBase* enemy = static_cast<EnemyBase*>(*it);
		if(enemy->sprite->getBoundingBox().containsPoint(convertTouchToNodeSpace(touch)))
		{
			enemyInfoPanel = EnemyInfoLayer::create();
			ValueMap enemyInfo = FileUtils::getInstance()->getValueMapFromFile("enemy.plist")[enemy->getEnemyName()].asValueMap();
			enemyInfoPanel->setEnemyInfo(enemy->getEnemyName(), enemyInfo);
			this->addChild(enemyInfoPanel, 99);
			return false;
		}
	}

	auto location = touch->getLocation();
	checkAndAddTowerPanel(location);

	return true;
}

void PlayLayer::checkAndAddTowerPanel(Point position)
{
	Point towerCoord = convertToTileCoord(position);
	Point matrixCoord = convertToMatrixCoord(position);

	if(towerCoord.x < 0 || towerCoord.x >= MAP_WIDTH || towerCoord.y < 0 || towerCoord.y >= MAP_HEIGHT)
		return ;

	int gid = bgLayer->getTileGIDAt(towerCoord);
	auto tileTemp = map->getPropertiesForGID(gid).asValueMap();
	int matrixIndex = static_cast<int>(matrixCoord.y * MAP_WIDTH + matrixCoord.x);

	int touchValue = 0;
	if(!tileTemp.empty())
		touchValue = tileTemp.at("canTouch").asInt();

	auto tileWidth = map->getContentSize().width / map->getMapSize().width;
	auto tileHeight = map->getContentSize().height / map->getMapSize().height;
	towerPos = Point(towerCoord.x * tileWidth + tileWidth / 2 - offX, 
						map->getContentSize().height - towerCoord.y * tileHeight - tileHeight / 2);

	if(touchValue)
	{
		if(!towerMatrix[matrixIndex])
		{
			addTowerChoosePanel(towerPos);
		}
		else
		{
			drawRange(towerMatrix[matrixIndex]);
			addTowerHandlePanel(towerPos);
		}
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/cancel.wav").c_str(), false);

		auto tips = Sprite::createWithSpriteFrameName("no.png");
		tips->setPosition(towerPos);
		this->addChild(tips);
		tips->runAction(Sequence::create(DelayTime::create(0.8f),
						CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tips)),
						NULL));
	}
}

Point PlayLayer::convertToTileCoord(Point position)
{
	int x = (position.x + offX) / map->getContentSize().width * map->getMapSize().width;
	int y = map->getMapSize().height - position.y / map->getContentSize().height * map->getMapSize().height;
	
	return Point(x, y);
}

Point PlayLayer::convertToMatrixCoord(Point position)
{
	int x = (position.x + offX) / map->getContentSize().width * map->getMapSize().width;
	int y = position.y / map->getContentSize().height * map->getMapSize().height;

	return Point(x, y);
}

void PlayLayer::addTowerChoosePanel(Point position)
{
	chooseTowerPanel = TowerPanelLayer::create();
	chooseTowerPanel->setPosition(position);
	this->addChild(chooseTowerPanel, 99);
}

void PlayLayer::addTowerHandlePanel(Point position)
{
	handleTowerPanel = TowerHandleLayer::create();
	handleTowerPanel->setPosition(position);
	this->addChild(handleTowerPanel, 99);
}

void PlayLayer::update(float dt)
{
	addTower();
	handleTower();
	collideDetection();
	enemyIntoHouse();

	if(isSuccessful)
	{
		isSuccessful = false;
		auto star = 0;
		auto hp = getPlayerHp();
		if(hp > 0 && hp <= 30)
			star = 1;
		else if(hp > 30 && hp <= 60)
			star = 2;
		else
			star = 3;

		if(star > UserDefault::getInstance()->getIntegerForKey(instance->getCurLevelFile().c_str()))
		{
			UserDefault::getInstance()->setIntegerForKey(instance->getCurLevelFile().c_str(), star);
			auto levelNum = UserDefault::getInstance()->getIntegerForKey("levelNum") + 1;
			UserDefault::getInstance()->setIntegerForKey("levelNum", levelNum);
		}

		auto nextLevel = instance->getNextLevelFile();
		UserDefault::getInstance()->setStringForKey("nextLevelFile", nextLevel);

		instance->clear();
		Director::getInstance()->replaceScene(TransitionFade::create(0.1f, SuccessScene::create()));
	}
}

void PlayLayer::collideDetection()
{
	auto bulletVector = instance->bulletVector;
	auto enemyVector = instance->enemyVector;

	if(bulletVector.empty() || enemyVector.empty())
		return;

	Vector<EnemyBase*> enemyToDelete;
	Vector<Sprite*> bulletToDelete;

	for(int i = 0; i < bulletVector.size(); i++)
	{
		auto bullet = bulletVector.at(i);
		if(!bullet->getParent())
			continue;

		auto bulletRect = Rect(bullet->getParent()->getPositionX() + bullet->getPositionX() - bullet->getContentSize().width / 2,
								bullet->getParent()->getPositionY() + bullet->getPositionY() - bullet->getContentSize().height / 2,
								bullet->getContentSize().width,
								bullet->getContentSize().height);

		for(int j = 0; j < enemyVector.size(); j++)
		{
			auto enemy = enemyVector.at(j);
			auto enemyRect = Rect(enemy->sprite->getPositionX() - enemy->sprite->getContentSize().width / 4,
									enemy->sprite->getPositionY() - enemy->sprite->getContentSize().height / 4,
									enemy->sprite->getContentSize().width / 2,
									enemy->sprite->getContentSize().height / 2);

			if(bulletRect.intersectsRect(enemyRect))
			{
				auto curHp = enemy->getHp();
				TowerBase* tower = static_cast<TowerBase*>(bullet->getParent());
				auto power = tower->getPower();

				curHp -= power;
				enemy->setHp(curHp);

				auto curHpPercent = enemy->getHpPercent();
				auto offHp = power * 100 / enemy->getMhp();
				curHpPercent -= offHp;
				if(curHpPercent < 0)
					curHpPercent = 0;

				enemy->setHpPercent(curHpPercent);
				enemy->getHpBar()->setPercentage(curHpPercent);

				if(curHp <= 0)
				{
					money += enemy->getMoney();
					ValueMap chineseDict = FileUtils::getInstance()->getValueMapFromFile("chinese.plist");
					moneyLabel->setString(chineseDict["money"].asString() + std::to_string(money));

					enemyToDelete.pushBack(enemy);
				}
				
				bulletToDelete.pushBack(bullet);
			}
		}

		for(EnemyBase* enemyTemp: enemyToDelete)
		{
			enemyTemp->enemyExplode();
			instance->enemyVector.eraseObject(enemyTemp);
		}
		enemyToDelete.clear();
	}

	for(Sprite* bulletTemp: bulletToDelete)
	{
		instance->bulletVector.eraseObject(bulletTemp);
		bulletTemp->removeFromParent();
	}
	bulletToDelete.clear();
}

GroupEnemy* PlayLayer::currentGroup()
{
	GroupEnemy* groupEnemy = NULL;
	if(!instance->groupVector.empty())
		groupEnemy = (GroupEnemy*)instance->groupVector.at(groupCounter);

	return groupEnemy;
}

GroupEnemy* PlayLayer::nextGroup()
{
	if(instance->groupVector.empty())
		return NULL;

	if(groupCounter < instance->getGroupNum() - 1)
		groupCounter++;
	else
		this->isSuccessful = true;

	GroupEnemy* groupEnemy = (GroupEnemy*)instance->groupVector.at(groupCounter);

	return groupEnemy;
}

void PlayLayer::logic(float dt)
{
	GroupEnemy* groupEnemy = currentGroup();

	if(!groupEnemy)
		return;

	if(groupEnemy->getIsFinishAddedGroup() && instance->enemyVector.size() == 0 && groupCounter < instance->getGroupNum())
	{
		groupEnemy = nextGroup();

		ValueMap chineseDict = FileUtils::getInstance()->getValueMapFromFile("chinese.plist");
		std::string groupText = chineseDict["wave"].asString();
		char buffer[128] = {0};
		sprintf(buffer, groupText.c_str(), (groupCounter + 1), instance->getGroupNum());
		groupLabel->setString(buffer);
	}

	addEnemy();
}

void PlayLayer::enemyIntoHouse()
{
	auto enemyVector = instance->enemyVector;

	for(int i = 0; i < enemyVector.size(); i++)
	{
		auto enemy = enemyVector.at(i);
		if(!enemy->getEnemySuccessful())
			continue;

		instance->enemyVector.eraseObject(enemy);
		enemy->removeFromParent();

		auto hp = getPlayerHp() - 10;
		if(hp > 0)
		{
			setPlayerHp(hp);
			playerHpBar->setPercentage(hp);
		}
		else
		{
			instance->clear();
			Director::getInstance()->replaceScene(TransitionFade::create(0.1f, FailScene::create()));
		}
	}
}

void PlayLayer::initToolLayer()
{
	Size winSize = Director::getInstance()->getWinSize();
	toolLayer = Layer::create();
	this->addChild(toolLayer);

	//toolbar bg
	Sprite* spriteTool = Sprite::createWithSpriteFrameName("toolbg.png");
	spriteTool->setAnchorPoint(Point(0, 1));
	spriteTool->setPosition(Point(0, winSize.height));
	spriteTool->setTag(1);
	toolLayer->addChild(spriteTool);

	//money label
	money = instance->getMoney();
	TTFConfig config("fonts/trends.ttf", 20);
	ValueMap chineseDict = FileUtils::getInstance()->getValueMapFromFile("chinese.plist");
	moneyLabel = Label::createWithTTF(config, chineseDict["money"].asString() + std::to_string(money));
	moneyLabel->setAnchorPoint(Point(0.5, 0.5f));
	moneyLabel->setPosition(Point(spriteTool->getContentSize().width / 6, spriteTool->getContentSize().height / 2));
	spriteTool->addChild(moneyLabel);

	//playerHp bar
	playerHpBg = Sprite::createWithSpriteFrameName("hpBg1.png");
	playerHpBg->setPosition(Point(spriteTool->getContentSize().width / 6 * 5, spriteTool->getContentSize().height / 2));
	playerHpBg->setScale(2);
	spriteTool->addChild(playerHpBg);

	playerHpBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("playerHp.png"));
	playerHpBar->setType(ProgressTimer::Type::BAR);
	playerHpBar->setMidpoint(Point(0, 0.5f));
	playerHpBar->setBarChangeRate(Point(1, 0));
	playerHpBar->setPercentage(playerHp);
	playerHpBar->setPosition(Point(playerHpBg->getContentSize().width / 2, playerHpBg->getContentSize().height / 2));
	playerHpBg->addChild(playerHpBar);

	//group label
	std::string groupText = chineseDict["wave"].asString();
	char buffer[128] = {0};
	sprintf(buffer, groupText.c_str(), (groupCounter + 1), instance->getGroupNum());
	groupLabel = Label::createWithTTF(config, buffer);
	groupLabel->setAnchorPoint(Point(0.5f, 0.5f));
	groupLabel->setPosition(Point(spriteTool->getContentSize().width / 2, spriteTool->getContentSize().height / 2));
	spriteTool->addChild(groupLabel);

	//back button
	Sprite* back = Sprite::createWithSpriteFrameName("backNormal.png");
	Sprite* backOver = Sprite::createWithSpriteFrameName("backOver.png");
	MenuItemSprite* pauseItem = MenuItemSprite::create(back, backOver, CC_CALLBACK_1(PlayLayer::menuBackCallback, this));
	pauseItem->setAnchorPoint(Point(0, 0));
	pauseItem->setPosition(Point(0, 0));
	Menu* menu = Menu::create(pauseItem, NULL);
	menu->setPosition(Point(spriteTool->getContentSize().width, 0));
	spriteTool->addChild(menu);
}

void PlayLayer::menuBackCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str(), false);

	instance->clear();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelScene::create()));
}

void PlayLayer::handleTower()
{
	if(!handleTowerPanel)
		return;

	auto funcName = handleTowerPanel->getFuncName();
	if(funcName == TowerFunc::NOTHING)
		return;

	Point matrixCoord = convertToMatrixCoord(towerPos);
	int matrixIndex = static_cast<int>(matrixCoord.y * MAP_WIDTH + matrixCoord.x);
	TowerBase* tower = towerMatrix[matrixIndex];
	if(!tower)
		return;

	if(funcName == TowerFunc::UPGRADE)
	{
		auto lv = tower->getLv();
		int price = tower->getInfo()["lv" + std::to_string(lv + 1) + "Info"].asValueMap()["price"].asInt();
		if(money >= price)
		{
			bool succ = tower->upgradeTower();
			if(succ)
			{
				money -= price;
				ValueMap chineseDict = FileUtils::getInstance()->getValueMapFromFile("chinese.plist");
				moneyLabel->setString(chineseDict["money"].asString() + std::to_string(money));
			}
		}
		drawRange(tower);
	}
	else if(funcName == TowerFunc::DESTROY)
	{
		this->removeChild(tower);
		towerMatrix[matrixIndex] = NULL;

		if(rangeSprite != NULL)
		{
			this->removeChild(rangeSprite);
			rangeSprite = NULL;
		}
	}

	funcName = TowerFunc::NOTHING;
	handleTowerPanel->setFuncName(funcName);
	this->removeChild(handleTowerPanel);
	handleTowerPanel = NULL;
}

void PlayLayer::drawRange(TowerBase* tower)
{
	if(rangeSprite != NULL)
	{
		this->removeChild(rangeSprite);
		rangeSprite = NULL;
	}

	rangeSprite = Sprite::create("circle.png");
	float radius = tower->getRange() * 2.0;
	rangeSprite->setScale(radius / rangeSprite->getContentSize().width);
	rangeSprite->setAnchorPoint(Point(0.5, 0.5));
	rangeSprite->setPosition(towerPos);
	this->addChild(rangeSprite, tower->getZOrder() - 1);
}