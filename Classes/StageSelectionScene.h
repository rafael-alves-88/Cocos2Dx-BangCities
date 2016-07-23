#ifndef __STAGESELECTIONSCENE_H__
#define __STAGESELECTIONSCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class StageSelectionScene : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(StageSelectionScene);
	static Scene* createScene();

	void stage1Callback(Ref* sender);
	void stage2Callback(Ref* sender);
	void stage3Callback(Ref* sender);
};

#endif