#include "SoldierTower.h"
#include "GameManager.h"

bool SoldierTower::init()
{
	if(!TowerBase::init())
		return false;

	setRange(100);
	setPower(20);
	setRate(2);

	tower = Sprite::createWithSpriteFrameName("soldierDown_2.png");
	this->addChild(tower);

	animationLeft = createAnimation("soldierLeft", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationLeft, "soldierrunleft");
	animationRight = createAnimation("soldierRight", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationRight, "soldierrunright");
	animationUp = createAnimation("soldierUp", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationUp, "soldierrunup");
	animationDown = createAnimation("soldierDown", 3, 0.1f);
	AnimationCache::getInstance()->addAnimation(animationDown, "soldierrundown");

	schedule(schedule_selector(SoldierTower::attack), 1.0f);
	schedule(schedule_selector(TowerBase::changeDirection), 0.3f);
	return true;
}

Sprite* SoldierTower::SoldierTowerBullet()
{
	Sprite* bullet = Sprite::createWithSpriteFrameName("bullet_1.png");
	bullet->setPosition(0, tower->getContentSize().height / 2);
	this->addChild(bullet);

	return bullet;
}

void SoldierTower::attack(float dt)
{
	GameManager* instance = GameManager::getInstance();

	checkNearestEnemy();
	if(nearestEnemy)
	{
		auto curBullet = SoldierTowerBullet();
		instance->bulletVector.pushBack(curBullet);

		auto moveDuration = getRate();
		Point shootVector = nearestEnemy->sprite->getPosition() - this->getPosition();
		Point normalizedShootVector = -shootVector.normalize();

		auto farthestDistance = Director::getInstance()->getWinSize().width;
		Point overshotVector = normalizedShootVector * farthestDistance;
		Point offscreenPoint = curBullet->getPosition() - overshotVector;

		curBullet->runAction(Sequence::create(MoveTo::create(moveDuration, offscreenPoint),
								CallFuncN::create(CC_CALLBACK_1(SoldierTower::removeBullet, this)),
								NULL));

		curBullet = NULL;
	}
}

void SoldierTower::removeBullet(Node* pSender)
{
	GameManager* instance = GameManager::getInstance();

	Sprite* sprite = (Sprite*)pSender;
	instance->bulletVector.eraseObject(sprite);
	sprite->removeFromParent();
}

void SoldierTower::changeDirection(float dt)
{
	Point enemyPosition = Point(0, 0);
	checkNearestEnemy();
	if(nearestEnemy && nearestEnemy->sprite)
		enemyPosition = nearestEnemy->sprite->getPosition();

	if(this->getPositionY() > enemyPosition.y)
	{
		tower->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("soldierrundown")));
	}
	else if(this->getPositionY() < enemyPosition.y)
	{
		tower->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("soldierrunup")));
	}
	else
	{
		tower->runAction(Animate::create(AnimationCache::getInstance()->getAnimation("soldierrundown")));
	}
}