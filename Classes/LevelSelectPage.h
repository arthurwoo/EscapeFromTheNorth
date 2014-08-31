#ifndef __EscapeFromTheNorth__LevelSelectPage__
#define __EscapeFromTheNorth__LevelSelectPage__

#include "cocos2d.h"

USING_NS_CC;

class LevelSelectPage: public Node
{
public:
	bool initLevelPage(const std::string& bgName, int level);
	static LevelSelectPage* create(const std::string& bgName, int level);
	void menuStartCallback(Ref* pSender);

};

#endif