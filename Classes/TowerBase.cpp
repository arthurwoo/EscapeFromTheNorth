#include "TowerBase.h"
#include "GameManager.h"

TowerBase::TowerBase(): range(0), power(0), rate(0), nearestEnemy(NULL), lv(1), maxLv(1), info(NULL), atkType("")
{
}

TowerBase::~TowerBase()
{
}

bool TowerBase::init()
{
	if(!Sprite::init())
		return false;

	scheduleUpdate();
	return true;
}

void TowerBase::setTowerInfo(std::string towerName)
{
	ValueMap towerInfo = FileUtils::getInstance()->getValueMapFromFile("tower.plist")[towerName].asValueMap();

	ValueMap lvInfo = towerInfo["lv" + std::to_string(getLv()) + "Info"].asValueMap();

	setTowerName(towerName);
	setRange(lvInfo["range"].asInt());
	setPower(lvInfo["power"].asInt());
	setRate(lvInfo["rate"].asInt());
	setLv(towerInfo["lv"].asInt());
	setMaxLv(towerInfo["maxLv"].asInt());
	setAtkType(towerInfo["attackType"].asString());
	setInfo(towerInfo);

	tower = Sprite::createWithSpriteFrameName(towerInfo["defaultImage"].asString());
	this->addChild(tower);	

	animationLeft = createAnimation(lvInfo["animationLeft"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationLeft, towerName + "runleft" + std::to_string(getLv()));
	animationRight = createAnimation(lvInfo["animationRight"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationRight, towerName + "runright" + std::to_string(getLv()));
	animationUp = createAnimation(lvInfo["animationUp"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationUp, towerName + "runup" + std::to_string(getLv()));
	animationDown = createAnimation(lvInfo["animationDown"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationDown, towerName + "rundown" + std::to_string(getLv()));

	schedule(schedule_selector(TowerBase::attack), 1.0f);
	schedule(schedule_selector(TowerBase::changeDirection), 0.3f);
}

void TowerBase::checkNearestEnemy()
{
	GameManager* instance = GameManager::getInstance();
	auto enemyVector = instance->enemyVector;

	auto minDistance = this->range;

	EnemyBase* enemyTemp = NULL;
	for(int i = 0; i < enemyVector.size(); i++)
	{
		auto enemy = enemyVector.at(i);
		double distance = this->getPosition().getDistance(enemy->sprite->getPosition());

		if(distance < minDistance)
		{
			minDistance = distance;
			enemyTemp = enemy;
		}
	}
	
	nearestEnemy = enemyTemp;
}

Animation* TowerBase::createAnimation(std::string prefixName, int framesNum, float delay)
{
	Vector<SpriteFrame*> animFrames;

	for(int i = 1; i <= framesNum; i++)
	{
		char buffer[30] = {0};
		sprintf(buffer, "_%i.png", i);
		std::string str = prefixName + buffer;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	return Animation::createWithSpriteFrames(animFrames, delay);
}

bool TowerBase::upgradeTower()
{
	if(lv >= maxLv)
		return false;

	setLv(lv + 1);

	ValueMap lvInfo = getInfo()["lv" + std::to_string(getLv()) + "Info"].asValueMap();

	setRange(lvInfo["range"].asInt());
	setPower(lvInfo["power"].asInt());
	setRate(lvInfo["rate"].asInt());

	animationLeft = createAnimation(lvInfo["animationLeft"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationLeft, towerName + "runleft" + std::to_string(getLv()));
	animationRight = createAnimation(lvInfo["animationRight"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationRight, towerName + "runright" + std::to_string(getLv()));
	animationUp = createAnimation(lvInfo["animationUp"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationUp, towerName + "runup" + std::to_string(getLv()));
	animationDown = createAnimation(lvInfo["animationDown"].asString(), 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationDown, towerName + "rundown" + std::to_string(getLv()));

	return true;
}

Sprite* TowerBase::towerBullet()
{
	Sprite* bullet = Sprite::createWithSpriteFrameName(getInfo()["bulletImage"].asString());
	bullet->setPosition(0, tower->getContentSize().height / 2);
	this->addChild(bullet);

	return bullet;
}

void TowerBase::attack(float dt)
{
	GameManager* instance = GameManager::getInstance();

	checkNearestEnemy();
	if(nearestEnemy)
	{
		auto curBullet = towerBullet();
		instance->bulletVector.pushBack(curBullet);

		auto attackType = getAtkType();
		if(attackType == ATK_TYPE_NEAR)
		{
			curBullet->setPosition(nearestEnemy->sprite->getPosition() - this->getPosition());
		}
		else if(attackType == ATK_TYPE_FAR)
		{
			auto moveDuration = getRate();
			Point shootVector = nearestEnemy->sprite->getPosition() - this->getPosition();
			Point normalizedShootVector = -shootVector.normalize();

			auto farthestDistance = Director::getInstance()->getWinSize().width;
			Point overshotVector = normalizedShootVector * farthestDistance;
			Point offscreenPoint = curBullet->getPosition() - overshotVector;

			curBullet->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint),
									CallFuncN::create(CC_CALLBACK_1(TowerBase::removeBullet, this)),
									NULL));
		}

		curBullet = NULL;
	}
}

void TowerBase::removeBullet(Node* pSender)
{
	GameManager* instance = GameManager::getInstance();

	Sprite* sprite = (Sprite*)pSender;
	instance->bulletVector.eraseObject(sprite);
	sprite->removeFromParent();
}

void TowerBase::changeDirection(float dt)
{
	Point enemyPosition = Point(0, 0);
	checkNearestEnemy();
	if(nearestEnemy && nearestEnemy->sprite)
		enemyPosition = nearestEnemy->sprite->getPosition();

	if(this->getPositionY() > enemyPosition.y)
	{
		tower->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(getTowerName() + "rundown" + std::to_string(getLv()))));
	}
	else if(this->getPositionY() < enemyPosition.y)
	{
		tower->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(getTowerName() + "runup" + std::to_string(getLv()))));
	}
	else
	{
		tower->runAction(Animate::create(AnimationCache::getInstance()->getAnimation(getTowerName() + "rundown" + std::to_string(getLv()))));
	}
}