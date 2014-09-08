#ifndef __EscapeFromTheNorth__TowerPanelLayer__
#define __EscapeFromTheNorth__TowerPanelLayer__

#include "cocos2d.h"
#include "TowerBase.h"

USING_NS_CC;

typedef enum
{
	SOLDIER_TOWER = 0,
	KNIGHT_TOWER,
	OTHER
} TowerType;

class TowerPanelLayer: public Layer
{
public:
	virtual bool init() override;
	CREATE_FUNC(TowerPanelLayer);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	CC_SYNTHESIZE(TowerType, chooseType, ChooseType);

private:
	Sprite* sprite1;
	Sprite* sprite2;

};

#endif