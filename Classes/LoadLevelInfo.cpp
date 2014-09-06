#include "LoadLevelInfo.h"
#include "GameManager.h"

LoadLevelInfo::~LoadLevelInfo()
{
	clearAll();
}

LoadLevelInfo* LoadLevelInfo::createLoadLevelInfo(const std::string& plistPath)
{
	LoadLevelInfo* pRet = new LoadLevelInfo();
	if(pRet && pRet->initPlist(plistPath))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool LoadLevelInfo::initPlist(const std::string& plistPath)
{
	bool pRet = false;

	do
	{
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistPath);
		ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);

		resources = dict["resources"].asValueMap();
		levelInfo = dict["levelInfo"].asValueMap();

		pRet = true;
	}while(0);

	return pRet;
}

void LoadLevelInfo::clearAll()
{
	resources.clear();
	levelInfo.clear();
}

void LoadLevelInfo::readLevelInfo()
{
	GameManager* instance = GameManager::getInstance();
	auto money = levelInfo["money"].asFloat();
	instance->setMoney(money);
	auto curLevel = levelInfo["curLevel"].asString();
	instance->setCurLevelFile(curLevel);
	auto nextLevel = levelInfo["nextLevel"].asString();
	instance->setNextLevelFile(nextLevel);

	ValueMap& groupDict = levelInfo["group"].asValueMap();
	auto groupTotal = groupDict.size();
	instance->setGroupNum(groupTotal);

	for(auto it = groupDict.begin(); it != groupDict.end(); it++)
	{
		ValueMap& group = it->second.asValueMap();
		GroupEnemy* groupEnemy = GroupEnemy::create()->initGroupEnemy(group);
		instance->groupVector.pushBack(groupEnemy);
	}

	auto curMapName = resources["map"].asString();
	instance->setCurMapName(curMapName);
}