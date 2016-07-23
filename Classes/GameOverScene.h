#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class GameOverScene : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(GameOverScene);
	static Scene* createScene();

	void updateScene();
};

#endif