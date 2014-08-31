#ifndef __EscapeFromTheNorth__LevelLayer__
#define __EscapeFromTheNorth__LevelLayer__

#include "cocos2d.h"

USING_NS_CC;

#define SHORTEST_SLIDE_LENGTH (20)

class LevelLayer: public Layer
{
public:
	LevelLayer();
	~LevelLayer();

	virtual bool init();
	static Scene* createScene();
	CREATE_FUNC(LevelLayer);

	void menuCloseCallback(Ref* pSender);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void addNode(Node* level);

private:
	int pageNode;
	int curPageNode;
	Point touchDownPoint;
	Point touchUpPoint;
	Point touchCurPoint;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;
	void gotoCurNode();

};

#endif