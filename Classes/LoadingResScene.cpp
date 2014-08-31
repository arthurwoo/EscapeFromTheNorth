#include "LoadingResScene.h"
#include "UIScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

LoadingResScene::LoadingResScene(): numberOfLoadedRes(0), totalOfLoadedRes(14), progressBar(NULL)
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
	Director::getInstance()->replaceScene(TransitionFade::create(0.1f, UIScene::createScene()));
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
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(FileUtils::getInstance()->fullPathForFilename("sound/theme.mp3").c_str());
	numberOfLoadedRes++;
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("sound/button.wav").c_str());
	numberOfLoadedRes++;
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("sound/appear.wav").c_str());
	numberOfLoadedRes++;
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
	float percent = (float)numberOfLoadedRes / (float)totalOfLoadedRes * 100;
	progressBar->setPercentage(percent);

	if(numberOfLoadedRes == totalOfLoadedRes)
	{
		transitionScene();
		SimpleAudioEngine::getInstance()->playBackgroundMusic(FileUtils::getInstance()->fullPathForFilename("sound/theme.mp3").c_str(), true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
	}
}