#ifndef __HUDLAYER_H__
#define __HUDLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;

class HudLayer : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(HudLayer);

	void initScore();
	void initPauseMenu();
	void initTimeBar();

	void pauseCallback(Ref* sender);
	void collectFishAnimation();

	void updateScore(int value);

	Label* scoreLabel;
	Label* lifeLabel;

	Sprite* coinSprite;
	Sprite* hudBase;
	Sprite* greenLifeBar;
	Sprite* sailorSprite;
	Sprite* skullSprite;
	ProgressTimer* progressBar;

private:
	Size size;
};

#endif