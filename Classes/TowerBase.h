#ifndef __EscapeFromTheNorth__TowerBase__
#define __EscapeFromTheNorth__TowerBase__

#include "cocos2d.h"
#include "EnemyBase.h"

USING_NS_CC;

class TowerBase: public Sprite
{
public:
	TowerBase();
	~TowerBase();

	virtual bool init();
	CREATE_FUNC(TowerBase);

	void setTowerInfo(std::string towerName);

	void checkNearestEnemy();

	CC_SYNTHESIZE(std::string, towerName, TowerName); //塔的名字
	CC_SYNTHESIZE(int, range, Range); //范围
	CC_SYNTHESIZE(int, power, Power); //攻击
	CC_SYNTHESIZE(int, rate, Rate); //攻击速率
	CC_SYNTHESIZE(int, lv, Lv); //等级
	CC_SYNTHESIZE(int, maxLv, MaxLv); //最大等级

	virtual void changeDirection(float dt);
	Animation* createAnimation(std::string prefixName, int framesNum, float delay);

	void upgradeTower();
	void attack(float dt);
	void removeBullet(Node* pSender);
	Sprite* towerBullet();

protected:
	EnemyBase* nearestEnemy;
	Animation* animationLeft;
	Animation* animationRight;
	Animation* animationUp;
	Animation* animationDown;

private:
	Sprite* tower;
	ValueMap towerInfo;

};

#endif