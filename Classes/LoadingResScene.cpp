#include "LoadingResScene.h"
#include "UIScene.h"

LoadingResScene::LoadingResScene(): numberOfLoadedRes(0), totalOfLoadedRes(11), progressBar(NULL)
{
}

LoadingResScene::~LoadingResScene()
{
	unschedule(schedule_selector(LoadingResScene::logic));
	progressBar = NULL;
}

Scene* LoadingResScene::createScene()
{
	Scene* scene = Scene::create();
	LoadingResScene* layer = LoadingResScene::create();
	scene->addChild(layer);

	return scene;
}

bool LoadingResScene::init()
{
	if(!Layer::init())
		return false;

	progressBar = addProgress();
	loadResources();
	schedule(schedule_selector(LoadingResScene::logic));

	return true;
}

void LoadingResScene::transitionScene()
{
	Director::getInstance()->replaceScene(TransitionFadeBL::create(0.5f, UIScene::createScene()));
}

ProgressTimer* LoadingResScene::addProgress()
{
	Size size = Director::getInstance()->getWinSize();
	Sprite* progressBarBg = Sprite::create("loadingBg.png");
	progressBarBg->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(progressBarBg);

	progressBar = ProgressTimer::create(Sprite::create("loadingBar.png"));
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Point(0, 0.5f));
	progressBar->setBarChangeRate(Point(1, 0));
	progressBar->setPercentage(0);
	progressBar->setAnchorPoint(Point(0.5f, 0.5f));
	progressBar->setPosition(Point(progressBarBg->getContentSize().width / 2, progressBarBg->getContentSize().height / 2));
	progressBarBg->addChild(progressBar);

	return progressBar;
}

void LoadingResScene::loadResources()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Play.plist");
	numberOfLoadedRes++;

	Director::getInstance()->getTextureCache()->addImageAsync("Plain.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("btnBackIcon.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("btnBackIconOver.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("card_1.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("card_2.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("card_3.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("LevelInfoPanel.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("selectLevelBg.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("start_1.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
    Director::getInstance()->getTextureCache()->addImageAsync("start_2.png", CC_CALLBACK_1(LoadingResScene::loadingCallback, this));
}

void LoadingResScene::loadingCallback(Texture2D* texture)
{
	numberOfLoadedRes++;
}

void LoadingResScene::logic(float dt)
{
	float percent = (float)numberOfLoadedRes / (float)totalOfLoadedRes;
	progressBar->setPercentage(percent);

	if(numberOfLoadedRes == totalOfLoadedRes)
	{
		transitionScene();
	}
}