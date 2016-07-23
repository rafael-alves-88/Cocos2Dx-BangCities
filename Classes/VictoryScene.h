#ifndef __VICTORYSCENE_H__
#define __VICTORYSCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class VictoryScene : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(VictoryScene);
	static Scene* createScene();

	void updateScene();
};

#endif