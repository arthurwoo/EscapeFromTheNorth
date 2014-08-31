#ifndef __EscapeFromTheNorth__UIScene__
#define __EscapeFromTheNorth__UIScene__

#include "cocos2d.h"

USING_NS_CC;

class UIScene: public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void menuStartCallback(Ref* pSender);
	CREATE_FUNC(UIScene);

};

#endif