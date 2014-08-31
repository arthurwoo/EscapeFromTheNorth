#include "EnemyBase.h"

EnemyBase::EnemyBase():pointCounter(0), animationLeft(NULL), animationRight(NULL), animationUp(NULL), animationDown(NULL),
						pointsVector(NULL), runSpeed(0), hp(0), mhp(0), hpPercent(100), hpBar(NULL), enemySuccessful(false)
{
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::init()
{
	if(!Sprite::init())
		return false;

	return true;
}

Node* EnemyBase::curPoint()
{
	if(this->pointsVector.size() > 0)
		return this->pointsVector.at(pointCounter);

	return NULL;
}

Node* EnemyBase::nextPoint()
{
	int maxCount = this->pointsVector.size();
	pointCounter++;
	if(pointCounter < maxCount)
	{
		auto node = this->pointsVector.at(pointCounter);
		return node;
	}
	else
	{
		setEnemySuccessful(true);
	}

	return NULL;
}

Animation* EnemyBase::createAnimation(std::string prefixName, int framesNum, float delay)
{
	Vector<SpriteFrame*> animFrames;

	for(int i = 1; i <= framesNum; i++)
	{
		char buffer[30] = {0};
		sprintf(buffer, "_%i.png", i);
		std::string str = prefixName + buffer;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	return Animation::createWithSpriteFrames(animFrames, delay);
}

void EnemyBase::runFollowPoint()
{
	auto startPoint = curPoint();
	sprite->setPosition(startPoint->getPosition());
	auto endPoint = nextPoint();

	if(endPoint != NULL)
	{
		auto duration = startPoint->getPosition().getDistance(endPoint->getPosition()) / getRunSpeed();
		sprite->runAction(Sequence::create(MoveTo::create(duration, endPoint->getPosition()), 
					CallFuncN::create(CC_CALLBACK_0(EnemyBase::runFollowPoint, this)), 
					NULL));
	}
}

void EnemyBase::setPointsVector(Vector<Node*> points)
{
	this->pointsVector = points;
}

void EnemyBase::createAndSetHpBar()
{
	hpBgSprite = Sprite::createWithSpriteFrameName("hpBg1.png");
	hpBgSprite->setPosition(Point(sprite->getContentSize().width / 2, sprite->getContentSize().height + 5));
	sprite->addChild(hpBgSprite);

	hpBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("hp1.png"));
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Point(0, 0.5f));
	hpBar->setBarChangeRate(Point(1, 0));
	hpBar->setPercentage(hpPercent);
	hpBar->setPosition(Point(hpBgSprite->getContentSize().width / 2, hpBgSprite->getContentSize().height / 2));
	hpBgSprite->addChild(hpBar);
}