#ifndef __EscapeFromTheNorth__Enemy__
#define __EscapeFromTheNorth__Enemy__

#include "cocos2d.h"

USING_NS_CC;

class EnemyBase : public Sprite
{
public:
	EnemyBase();
	~EnemyBase();

	virtual bool init() override;
	CREATE_FUNC(EnemyBase);

	Animation* createAnimation(std::string prefixName, int framesNum, float delay);
	Node* curPoint();
	Node* nextPoint();
	void runFollowPoint();
	void setPointsVector(Vector<Node*> points);
	virtual void changeDirection(float dt);
	virtual void enemyExplode();
	void setEnemyInfo(std::string enemyName, Vector<Node*> points);

	void createAndSetHpBar();

private:
	Vector<Node*> pointsVector;

protected:
	int pointCounter;
	Animation* animationLeft;
	Animation* animationRight;
	Animation* animationUp;
	Animation* animationDown;
	Animation* animationExplode;
	CC_SYNTHESIZE(float, runSpeed, RunSpeed);
	CC_SYNTHESIZE(int, hp, Hp);
	CC_SYNTHESIZE(int, mhp, Mhp);
	CC_SYNTHESIZE(float, hpPercent, HpPercent);
	CC_SYNTHESIZE(ProgressTimer*, hpBar, HpBar);
	CC_SYNTHESIZE(bool, enemySuccessful, EnemySuccessful);
	CC_SYNTHESIZE(std::string, enemyName, EnemyName);
	CC_SYNTHESIZE(int, money, Money);
	Sprite* sprite;
	Sprite* hpBgSprite;

};

#endif