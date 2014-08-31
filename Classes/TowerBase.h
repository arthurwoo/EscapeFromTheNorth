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

	CC_SYNTHESIZE(int, range, Range); //��Χ
	CC_SYNTHESIZE(int, power, Power); //����
	CC_SYNTHESIZE(int, rate, Rate); //

	virtual void changeDirection(float dt){};
	Animation* createAnimation(std::string prefixName, int framesNum, float delay);

protected:
	EnemyBase* nearestEnemy;
	Animation* animationLeft;
	Animation* animationRight;
	Animation* animationUp;
	Animation* animationDown;

};

#endif