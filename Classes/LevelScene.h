#ifndef __EscapeFromTheNorth__LevelScene__
#define __EscapeFromTheNorth__LevelScene__

#include "cocos2d.h"

USING_NS_CC;

class LevelScene: public Scene
{
public:
	virtual bool init();
	CREATE_FUNC(LevelScene);
	void menuCloseCallback(Ref* pSender);
	static LevelScene* getInstance();

private:
	static LevelScene* instance;

};

#endif