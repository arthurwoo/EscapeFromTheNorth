#include "GroupEnemy.h"

USING_NS_CC;

bool GroupEnemy::init()
{
	if(!Node::init())
		return false;

	return true;
}

GroupEnemy* GroupEnemy::initGroupEnemy(ValueMap group)
{
	this->enemyTotal = 0;
	for(auto git = group.begin(); git != group.end(); git++)
	{
		this->typeMap[git->first] = git->second.asValueMap();
		this->enemyTotal += git->second.asValueMap()["typeNum"].asInt();
	}
	
	this->isFinishedAddedGroup = false;

	return this;
}