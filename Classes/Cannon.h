#ifndef __CANNON_H__
#define __CANNON_H__

#include "cocos2d.h"
using namespace cocos2d;

class Cannon : public Sprite
{
public:
	virtual bool init();
	CREATE_FUNC(Cannon);
};

#endif
