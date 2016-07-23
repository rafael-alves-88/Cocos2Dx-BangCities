#ifndef __SPLASHSCENE_H__
#define __SPLASHSCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class SplashScene : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(SplashScene);
	static Scene* createScene();

	void loadTextures();
	void loadSounds();

	void updateScene();
};

#endif