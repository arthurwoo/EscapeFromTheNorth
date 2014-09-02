#ifndef __EscapeFromTheNorth__TowerHandleLayer__
#define __EscapeFromTheNorth__TowerHandleLayer__

#include "cocos2d.h"

USING_NS_CC;

typedef enum
{
	UPGRADE = 0,
	DESTROY,
	NOTHING
} TowerFunc;

class TowerHandleLayer: public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(TowerHandleLayer);

	CC_SYNTHESIZE(int, funcName, FuncName);

	void menuUpgradeCallback(Ref* pSender);
	void menuDestroyCallback(Ref* pSender);

};

#endif