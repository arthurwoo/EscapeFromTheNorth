#ifndef __EscapeFromTheNorth__GroupEnemy__
#define __EscapeFromTheNorth__GroupEnemy__

#include "cocos2d.h"
#include "EnemyBase.h"

class GroupEnemy: public cocos2d::Node
{
public:
	virtual bool init();
	GroupEnemy* initGroupEnemy(ValueMap group);
	CREATE_FUNC(GroupEnemy);

	CC_SYNTHESIZE(ValueMap, typeMap, TypeMap);
	CC_SYNTHESIZE(int, enemyTotal, EnemyTotal);
	CC_SYNTHESIZE(int, isFinishedAddedGroup, IsFinishAddedGroup);

};

#endif