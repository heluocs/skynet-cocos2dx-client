#include "SplashScene.h"
#include "LoadingScene.h"

USING_NS_CC;

bool SplashScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("goomegame.png");

	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(sprite, 0);

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::replaceScene), 2.0f);

	return true;
}

void SplashScene::replaceScene(float dt)
{
	CCLOG("---hello---");
	auto scene = LoadingScene::create();
	Director::getInstance()->replaceScene(scene);
}