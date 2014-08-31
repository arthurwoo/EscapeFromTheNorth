#ifndef __EscapeFromTheNorth__LoadingResScene__
#define __EscapeFromTheNorth__LoadingResScene__

#include "cocos2d.h"
#include "LoadLevelInfo.h"

USING_NS_CC;

class LoadingResScene: public Layer
{
public:
	LoadingResScene();
	~LoadingResScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoadingResScene);

	ProgressTimer* addProgress();
	void loadResources();
	void transitionScene();
	void loadingCallback(Texture2D* texture);
	void logic(float dt);

private:
	ProgressTimer* progressBar;
	int numberOfLoadedRes;
	int totalOfLoadedRes;

};

#endif