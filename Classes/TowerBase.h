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

	void checkNearestEnemy();

	CC_SYNTHESIZE(int, range, Range); //范围
	CC_SYNTHESIZE(int, power, Power); //攻击
	CC_SYNTHESIZE(int, rate, Rate); //
	CC_SYNTHESIZE(int, lv, Lv); //等级
	CC_SYNTHESIZE(int, maxLv, MaxLv); //最大等级

	virtual void changeDirection(float dt){};
	Animation* createAnimation(std::string prefixName, int framesNum, float delay);

	void upgradeTower();

protected:
	EnemyBase* nearestEnemy;
	Animation* animationLeft;
	Animation* animationRight;
	Animation* animationUp;
	Animation* animationDown;

};

#endif