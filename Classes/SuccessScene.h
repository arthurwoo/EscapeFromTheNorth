#ifndef __EscapeFromTheNorth__SuccessScene__
#define __EscapeFromTheNorth__SuccessScene__

#include "cocos2d.h"

USING_NS_CC;

class SuccessScene: public Scene
{
public:
	virtual bool init();
	CREATE_FUNC(SuccessScene);

	void menuNextCallback(Ref* pSender);
	void menuCloseCallback(Ref* pSender);

};

#endif