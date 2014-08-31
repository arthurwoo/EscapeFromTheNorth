#ifndef __EscapeFromTheNorth__Ghost__
#define __EscapeFromTheNorth__Ghost__

#include "cocos2d.h"
#include "EnemyBase.h"

USING_NS_CC;

class Ghost : public EnemyBase
{
public:
	virtual bool init() override;
	static Ghost* createGhost(Vector<Node*> points, int hp);
	void changeDirection(float dt);
	void enemyExplode();

};

#endif