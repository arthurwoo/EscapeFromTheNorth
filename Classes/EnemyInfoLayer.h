#ifndef __EscapeFromTheNorth__EnemyInfoLayer__
#define __EscapeFromTheNorth__EnemyInfoLayer__

#include "cocos2d.h"

USING_NS_CC;

class EnemyInfoLayer: public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(EnemyInfoLayer);

	void setEnemyInfo(std::string enemyName, ValueMap info);

private:
	Sprite* spriteBg;

};

#endif