#ifndef __EscapeFromTheNorth__SoldierTower__
#define __EscapeFromTheNorth__SoldierTower__

#include "TowerBase.h"

class SoldierTower: public TowerBase
{
public:
	virtual bool init();
	CREATE_FUNC(SoldierTower);

	void attack(float dt);
	void removeBullet(Node* pSender);
	Sprite* SoldierTowerBullet();

	void changeDirection(float dt);

private:
	Sprite* tower;

};

#endif