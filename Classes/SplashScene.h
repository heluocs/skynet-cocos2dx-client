#ifndef _SPLASH_SCENE_H_
#define _SPLASH_SCENE_H_

#include "cocos2d.h"

class SplashScene : public cocos2d::Scene
{
public:
	virtual bool init();

	CREATE_FUNC(SplashScene);

private:
	void replaceScene(float dt);
};

#endif