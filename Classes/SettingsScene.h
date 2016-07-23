#ifndef __SETTINGSCENE_H__
#define __SETTINGSCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class SettingsScene : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(SettingsScene);
	static Scene* createScene();

	void musicCallback(Ref* sender);
	void sfxCallback(Ref* sender);
	void backCallback(Ref* sender);
};

#endif