#ifndef __EscapeFromTheNorth__GameManager__
#define __EscapeFromTheNorth__GameManager__

#include "cocos2d.h"
#include "EnemyBase.h"
#include "TowerBase.h"
#include "GroupEnemy.h"

USING_NS_CC;

class GameManager
{
public:
	Vector<EnemyBase*> enemyVector;
	Vector<TowerBase*> towerVector;
	Vector<Sprite*> bulletVector;
	Vector<GroupEnemy*> groupVector;

	CC_SYNTHESIZE(float, money, Money);
	CC_SYNTHESIZE(int, groupNum, GroupNum);
	CC_SYNTHESIZE(std::string, curMapName, CurMapName);
	CC_SYNTHESIZE(std::string, curLevelFile, CurLevelFile);
	CC_SYNTHESIZE(std::string, nextLevelFile, NextLevelFile);
	CC_SYNTHESIZE(bool, isFinishedAddedGroup, IsFinishedAddedGroup);
	CC_SYNTHESIZE(std::string, levelDesc, LevelDesc);

	bool init();
	void clear();

	static GameManager* getInstance();

private:
	static GameManager* instance;

};

#endif