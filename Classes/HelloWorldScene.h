#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PauseLayer.h"
#include "HudLayer.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);
    
    void restartScence(cocos2d::Ref* pSender);
    
    bool onContactBegan(cocos2d::PhysicsContact &contact);
    
    bool canExplodeTarget();

	// layer
	static HudLayer* hudLayer;
	static PauseLayer* pauseLayer;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    // label para msg de vitoria!!
    cocos2d::Label* _labelWin;
    
    // label com porcentabem de vida
    cocos2d::Label* _labelLifeCannon2;
    
    // terreno
    cocos2d::Sprite* _ground;
    
    // canhao 1
    cocos2d::Sprite* _cannon_gun;
    cocos2d::Sprite* _cannon;
    
    // canhao 2
    cocos2d::Sprite* _cannon_gun2;
    cocos2d::Sprite* _cannon2;
    
    int _countHitTarget1;
};

#endif // __HELLOWORLD_SCENE_H__
