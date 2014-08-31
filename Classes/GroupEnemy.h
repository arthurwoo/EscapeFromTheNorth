#ifndef __EscapeFromTheNorth__GroupEnemy__
#define __EscapeFromTheNorth__GroupEnemy__

#include "cocos2d.h"
#include "EnemyBase.h"

class GroupEnemy: public cocos2d::Node
{
public:
	virtual bool init();
	GroupEnemy* initGroupEnemy(int type1Num, int type1Hp);
	CREATE_FUNC(GroupEnemy);

	CC_SYNTHESIZE(int, type1Num, Type1Num);
	CC_SYNTHESIZE(int, type1Hp, Type1Hp);
	CC_SYNTHESIZE(int, enemyTotal, EnemyTotal);
	CC_SYNTHESIZE(int, isFinishedAddedGroup, IsFinishAddedGroup);

};

#endif