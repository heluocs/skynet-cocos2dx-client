#include "LoadingScene.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace cocos2d::ui;

bool LoadingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("i-2.png");

	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(sprite, 0);

	this->scheduleOnce(CC_SCHEDULE_SELECTOR(LoadingScene::addOperateLayer), 2.0f);

	return true;
}

void LoadingScene::addOperateLayer(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto layer = Layer::create();
	auto startBtn = Button::create("ui/gamestart.png", "ui/angamestart.png");
	startBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 6));
	startBtn->addTouchEventListener(CC_CALLBACK_2(LoadingScene::replaceScene, this));
	layer->addChild(startBtn);

	auto settingBtn = Button::create("ui/option.png", "ui/anoption.png");
	settingBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 6 - 100));
	settingBtn->addTouchEventListener(CC_CALLBACK_2(LoadingScene::setting, this));
	layer->addChild(settingBtn);

	this->addChild(layer);
}

void LoadingScene::replaceScene(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		CCLOG("---start game---");
		break;
	default:
		break;
	}
}

void LoadingScene::setting(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		CCLOG("---setting---");
		break;
	default:
		break;
	}
}