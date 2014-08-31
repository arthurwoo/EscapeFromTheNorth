#include "TowerBase.h"
#include "GameManager.h"

TowerBase::TowerBase(): range(0), power(0), rate(0), nearestEnemy(NULL)
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