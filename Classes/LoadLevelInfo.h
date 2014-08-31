#ifndef __EscapeFromTheNorth__LoadLevelInfo__
#define __EscapeFromTheNorth__LoadLevelInfo__

#include "cocos2d.h"

USING_NS_CC;

class LoadLevelInfo: public Ref
{
public:
	~LoadLevelInfo();
	static LoadLevelInfo* createLoadLevelInfo(const std::string& plistPath);

	bool initPlist(const std::string& plistPath);
	void readLevelInfo();
	void clearAll();

private:
	ValueMap resources;
	ValueMap levelInfo;

};

#endif