#ifndef __EscapeFromTheNorth__PlayLayer__
#define __EscapeFromTheNorth__PlayLayer__

#include "cocos2d.h"
#include "TowerPanelLayer.h"
#include "GameManager.h"
#include "TowerHandleLayer.h"
#include "EnemyInfoLayer.h"

USING_NS_CC;

class PlayLayer : public Layer
{
public:
	PlayLayer();
	~PlayLayer();

	virtual bool init() override;
	static Scene* createScene();
	CREATE_FUNC(PlayLayer);

	virtual void update(float dt) override;
	virtual bool onTouchBegan(Touch* touch, Event* event) override;

	GroupEnemy* currentGroup();
	GroupEnemy* nextGroup();
	void logic(float dt);

private:
	SpriteBatchNode* spriteSheet;
	TMXTiledMap* map;
	TMXLayer* bgLayer;
	TMXLayer* ftLayer;
	TMXObjectGroup* objects;
	Vector<Node*> pointsVector;
	float offX;
	int money;
	int groupCounter;
	bool isSuccessful;
	Point towerPos;
	TowerBase** towerMatrix;
	GameManager* instance;
	TowerPanelLayer* chooseTowerPanel;
	TowerHandleLayer* handleTowerPanel;
	EnemyInfoLayer* enemyInfoPanel;
	Layer* toolLayer;
	Label* moneyLabel;
	Label* groupLabel;
	Sprite* playerHpBg;
	CC_SYNTHESIZE(float, playerHp, PlayerHp);
	CC_SYNTHESIZE_READONLY(ProgressTimer*, playerHpBar, PlayerHpBar);

	void initPointsVector(float offX);
	void addEnemy();
	void addTower();
	void handleTower();

	Point convertToTileCoord(Point position);
	Point convertToMatrixCoord(Point position);
	void checkAndAddTowerPanel(Point position);
	void addTowerChoosePanel(Point position);
	void addTowerHandlePanel(Point position);
	void collideDetection();
	void enemyIntoHouse();

	void initToolLayer();
	void menuBackCallback(Ref* pSender);
};

#endif