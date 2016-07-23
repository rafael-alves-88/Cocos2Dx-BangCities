#ifndef __GAMESCENE01_H__
#define __GAMESCENE01_H__

#include "cocos2d.h"
#include "PauseLayer.h"
#include "HudLayer.h"

using namespace cocos2d;

class GameScene01 : public Layer
{
public:
	GameScene01();
	~GameScene01();
	static GameScene01* instance;
	static GameScene01* getInstance();

	virtual bool init();
	CREATE_FUNC(GameScene01);
	static Scene* createScene();
	bool onTouchBegan(Touch* touch, Event* pEvent);
	void onTouchEnded(Touch* touch, Event* pEvent);
	void onTouchMoved(Touch* touch, Event* pEvent);
	void onTouchCancelled(Touch *touch, Event* pEvent);
	virtual void update(float dt);

	void initInput();
	void initBackground();
	void initGameObjects();

	void pauseGame();
	void quitGame();

	void updateSwipeInput(float dt);
	void updateGameLogic(float dt);
	void updateScore(int pointsToAdd);

	static HudLayer* hudLayer;
	static PauseLayer* pauseLayer;
	
	void gameOverCallback(Ref* sender);
	void victoryCallback(Ref* sender);

private:
	Size size;

	float currentTouchPosition[1];
	float initialTouchPosition[1];
	bool isTouchDown;

	bool isPaused;
};

#endif