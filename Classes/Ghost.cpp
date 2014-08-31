#include "Ghost.h"

bool Ghost::init()
{
	if(!Sprite::init())
		return false;

	setRunSpeed(20);
	sprite = Sprite::createWithSpriteFrameName("ghostRight_1.png");
	this->addChild(sprite);

	animationLeft = createAnimation("ghostLeft", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationLeft, "ghostrunleft");
	animationRight = createAnimation("ghostRight", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationRight, "ghostrunright");
	animationUp = createAnimation("ghostUp", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationUp, "ghostrunup");
	animationDown = createAnimation("ghostDown", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationDown, "ghostrundown");
	animationExplode = createAnimation("ghostExplode", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationExplode, "ghostexplode");

	createAndSetHpBar();
	schedule(schedule_selector(EnemyBase::changeDirection), 0.3f);
	return true;
}

Ghost* Ghost::createGhost(Vector<Node*> points, int hp)
{
	Ghost* pRet = new Ghost();
	if(pRet && pRet->init())
	{
		pRet->setPointsVector(points);
		pRet->setMhp(hp);
		pRet->setHp(hp);
		pRet->runFollowPoint();

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

void Ghost::changeDirection(float dt)
{
	auto cur = curPoint();

	if(cur == NULL)
		return;

	Point spritePosition = sprite->getPosition();

	if(cur->getPositionY() > spritePosition.y)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("ghostrunup")));
	}
	else if(cur->getPositionY() < spritePosition.y)
	{
		sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("ghostrundown")));
	}
	else
	{
		if(cur->getPositionX() > spritePosition.x)
		{
			sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("ghostrunright")));
		}
		else if(cur->getPositionX() < spritePosition.x)
		{
			sprite->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("ghostrunleft")));
		}
	}
}

void Ghost::enemyExplode()
{
	sprite->stopAllActions();
	unschedule(schedule_selector(EnemyBase::changeDirection));
	sprite->setAnchorPoint(Point(0.5f, 0.25f));
	sprite->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("ghostexplode")),
						CallFuncN::create(CC_CALLBACK_0(EnemyBase::removeFromParent, this)),
						NULL));
}