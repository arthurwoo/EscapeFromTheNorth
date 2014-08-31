#include "GroupEnemy.h"

USING_NS_CC;

bool GroupEnemy::init()
{
	if(!Node::init())
		return false;

	return true;
}

GroupEnemy* GroupEnemy::initGroupEnemy(int type1Num, int type1Hp)
{
	this->type1Num = type1Num;
	this->type1Hp = type1Hp;
	this->enemyTotal = type1Num;
	this->isFinishedAddedGroup = false;

	return this;
}