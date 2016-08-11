#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(Color4B::ORANGE)))
    {
        return false;
    }
    
    //this->setColor(Color3B(51, 153, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Bang Cities - 9MOB", "fonts/Marker Felt.ttf", 48);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // Adicionando terreno
    Sprite *ground = Sprite::create("res/game_scene_01_floor.png");
    ground->setAnchorPoint(Vec2(0,0));
    ground->setPosition(Vec2(0,0));
    this->addChild(ground, 0);
    
    // Adicionando arma do canhao (Player 1)
    auto cannon_gun = Sprite::create("res/cannon_gun.png");
    cannon_gun->setPosition(Vec2(150 + origin.x, ground->getBoundingBox().size.height + 80));
    this->addChild(cannon_gun, 0);
    
    // Adicionando canhao (Player 1)
    Sprite *cannon = Sprite::create("res/cannon.png");
    cannon->setPosition(Vec2(100 + origin.x, ground->getBoundingBox().size.height + 35));
    this->addChild(cannon, 0);
    
    // Adicionando arma do canhao (Player 2 - Maquina)
    auto cannon_gun2 = Sprite::create("res/cannon_gun.png");
    cannon_gun2->setPosition(Vec2(visibleSize.width - 150, ground->getBoundingBox().size.height + 80));
    cannon_gun2->setFlippedX(true);
    this->addChild(cannon_gun2, 0);

    // Adicionando canhao (Player 2 - Maquina)
    Sprite *cannon2 = Sprite::create("res/cannon.png");
    cannon2->setPosition(Vec2(visibleSize.width - 100, ground->getBoundingBox().size.height + 35));
    cannon2->setFlippedX(true);
    this->addChild(cannon2, 0);
    
    // Para travar o evento de touch
    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    cocos2d::log("touch began");
    return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    cocos2d::log("touch ended");
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    cocos2d::log("touch moved");
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
    cocos2d::log("touch cancelled");
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
