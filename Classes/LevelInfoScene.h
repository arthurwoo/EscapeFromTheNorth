#ifndef __EscapeFromTheNorth__LevelInfoScene__
#define __EscapeFromTheNorth__LevelInfoScene__

#include "cocos2d.h"
#include "LoadLevelInfo.h"

USING_NS_CC;

class LevelInfoScene: public Layer
{
public:
	LevelInfoScene();
	CREATE_FUNC(LevelInfoScene);
	static Scene* createScene();
	bool init();
	void addBackGround();

	void initLoadInfo(int groupLv, int lv);
	void menuBackCallback(Ref* pSender);
	void menuStartCallback(Ref* pSender);


private:
	LoadLevelInfo* info;
	std::string fileName;

};

#endif