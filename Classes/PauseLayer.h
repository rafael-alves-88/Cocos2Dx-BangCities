#ifndef __PAUSELAYER_H__
#define __PAUSELAYER_H__

#include "cocos2d.h"
using namespace cocos2d;

class PauseLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(PauseLayer);

	void resumeGameCallback(Ref* sender);
	void quitGameCallback(Ref* sender);
private:
	Size size;
};

#endif