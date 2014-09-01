#ifndef __EscapeFromTheNorth__FailScene__
#define __EscapeFromTheNorth__FailScene__

#include "cocos2d.h"

USING_NS_CC;

class FailScene: public Scene
{
public:
	virtual bool init();
	CREATE_FUNC(FailScene);

	void menuBackCallback(Ref* pSender);

};

#endif