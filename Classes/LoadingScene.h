#ifndef _LOADING_SCENE_H_
#define _LOADING_SCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class LoadingScene : public cocos2d::Scene
{
public:
	virtual bool init();

	CREATE_FUNC(LoadingScene);

private:
	void addOperateLayer(float dt);
	void replaceScene(Ref *pSender, Widget::TouchEventType type);
	void setting(Ref *pSender, Widget::TouchEventType type);
};

#endif