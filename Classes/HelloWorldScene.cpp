#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include "MenuScene.h"
#include "StageSelectionScene.h"

USING_NS_CC;

// contagem de acertos no tanque 2
int _countHitTarget2;

// cooldown do tiro
boolean isCooldown;
float cooldownTime = 1.5f;

// pause
boolean isPaused;

HelloWorld* HelloWorld::instance = NULL;
HudLayer* HelloWorld::hudLayer = NULL;
PauseLayer* HelloWorld::pauseLayer = NULL;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    auto layer = HelloWorld::create();

    scene->addChild(layer);

	auto hLayer = HudLayer::create();
	hudLayer = hLayer;

	auto pLayer = PauseLayer::create();
	pauseLayer = pLayer;
	
	scene->addChild(hudLayer);
	scene->addChild(pauseLayer);
	pauseLayer->setVisible(false);
    
    return scene;
}

HelloWorld* HelloWorld::getInstance()
{
	if (instance == NULL)
		instance = new HelloWorld();

	return instance;
}

enum class PhysicsCategory {
    None = 0,
    Cannon = (1 << 0),             // 1
    Projectile = (1 << 1),         // 2
    All = PhysicsCategory::Cannon | PhysicsCategory::Projectile // 3
};

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(Color4B::ORANGE)))
    {
        return false;
    }
    
	isCooldown = false;
	isPaused = false;
    _countHitTarget1 = 0;
	instance = this;
    
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic(gameScene_01_01MusicFile, true);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Bang Cities", font_markerfelt, 48);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // label pra mostrar msg de vitoria
    _labelWin = Label::createWithTTF("", font_markerfelt, 60);
    _labelWin->setColor(Color3B::BLACK);
    _labelWin->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height/2));

    this->addChild(_labelWin);

    // Adicionando terreno
    _ground = Sprite::create(game_scene_01_floor);
    _ground->setAnchorPoint(Vec2(0,0));
    _ground->setPosition(Vec2(0,0));
    this->addChild(_ground, 0);

    // label com a porcentagem (vida) do tanque 2
    _labelLifeCannon2 = Label::createWithTTF("100%", font_markerfelt, 48);
    
    _labelLifeCannon2->setPosition(Vec2(visibleSize.width - 100, _ground->getContentSize().height/2));
    this->addChild(_labelLifeCannon2);

    // Adicionando arma do canhao (Player 1)
    _cannon_gun = Sprite::create(cannon_gun_player);
    _cannon_gun->setPosition(Vec2(150 + origin.x, _ground->getBoundingBox().size.height + 80));
    this->addChild(_cannon_gun, 0);
    
    // Adicionando canhao (Player 1)
    _cannon = Sprite::create(cannon_player);
    _cannon->setPosition(Vec2(100 + origin.x, _ground->getBoundingBox().size.height + 35));
    this->addChild(_cannon, 0);
    
    // Adicionando arma do canhao (Player 2 - Maquina)
    _cannon_gun2 = Sprite::create(cannon_gun_player);
    _cannon_gun2->setPosition(Vec2(visibleSize.width - 150, _ground->getBoundingBox().size.height + 80));
    _cannon_gun2->setFlippedX(true);
    this->addChild(_cannon_gun2, 0);

    // Adicionando canhao (Player 2 - Maquina)
    _cannon2 = Sprite::create(cannon_player);
    _cannon2->setPosition(Vec2(visibleSize.width - 100, _ground->getBoundingBox().size.height + 35));
    _cannon2->setFlippedX(true);
    
    // Sobre a colisao
    auto cannonSize = _cannon_gun2->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(cannonSize.width, cannonSize.height),
                                              PhysicsMaterial(0.1f, 1.0f, 0.0f));
    
    physicsBody->setDynamic(true);
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Cannon);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);
    
    _cannon_gun2->setPhysicsBody(physicsBody);
    
    this->addChild(_cannon2, 0);
    
    // Para travar o evento de touch
    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    // Sobre a colisao: Finally, you need to register to receive contact notifications...
    // https://www.raywenderlich.com/95835/cocos2d-x-tutorial-beginners
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    return true;
}

// seguindo https://www.raywenderlich.com/95835/cocos2d-x-tutorial-beginners
// no trecho: Then implement your callback in HelloWorldScene.cpp:
bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    cocos2d::log("touch began");
    
	if (!isCooldown) {
		isCooldown = true;

		Vec2 touchLocation = touch->getLocation();
		Vec2 offset = touchLocation - _cannon_gun->getPosition();

		if (offset.x < 0) {
			return true;
		}

		auto bullet = Sprite::create("res/bullet.png");
		bullet->setPosition(_cannon_gun->getPosition());
		bullet->setFlippedX(true);

		// Sobre a colisao
		auto bulletSize = bullet->getContentSize();
		auto physicsBody = PhysicsBody::createCircle(bulletSize.width / 2);
		physicsBody->setDynamic(true);
		physicsBody->setCategoryBitmask((int)PhysicsCategory::Projectile);
		physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
		physicsBody->setContactTestBitmask((int)PhysicsCategory::Cannon);
		bullet->setPhysicsBody(physicsBody);

		this->addChild(bullet);

		offset.normalize();
		auto shootAmount = offset * 1000;

		auto realDest = shootAmount + bullet->getPosition();

		auto actionMove = MoveTo::create(2.0f, realDest);
		auto actionRemove = RemoveSelf::create();
		bullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));

		// som do canhao
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect(cannonShotFxFile, false, 1.0f, 1.0f, 1.0f);

		// criando callback, cooldown para próximo tiro
		auto loading = CallFunc::create(CC_CALLBACK_0(HelloWorld::updateCooldown, this));
		auto delay = DelayTime::create(cooldownTime);
		auto sequence = Sequence::create(delay, loading, NULL);

		// executa a sequencia criada
		this->runAction(sequence);
	}
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

bool HelloWorld::onContactBegan(PhysicsContact &contact)
{
    cocos2d::log("on contact began");

    if (canExplodeTarget()) {
        // som da explosao
        auto audioExplosion = CocosDenshion::SimpleAudioEngine::getInstance();
        audioExplosion->playEffect(explosionFxFile, false, 1.0f, 1.0f, 1.0f);
        
        // explode
        auto particle = ParticleFire::createWithTotalParticles(400);
        particle->setDuration(1.0f);
        particle->setPosition(Vec2(_cannon_gun2->getPosition()));
        this->addChild(particle);
        particle->setAutoRemoveOnFinish(true);
        
        // remove a arma do canhão 2
        this->removeChild(_cannon_gun2);
        this->removeChild(_cannon2);
        
        // faz alguma coisa pra mostrar que o jogo acabou...
        _labelWin->setString("Parabens Capitao! Missao Cumprida!");
        
		/* Menu item pra reiniciar o jogo
        auto labelRestart = Label::createWithTTF("Jogar novamente?", "fonts/Marker Felt.ttf", 48);
        labelRestart->setColor(Color3B::WHITE);
        
        auto restartGame = MenuItemLabel::create(labelRestart, CC_CALLBACK_1(HelloWorld::restartScence, this));
        
        restartGame->setPosition(Vec2(origin.x + this->visibleSize.width/2, this->origin.y + this->visibleSize.height/2));
        
        auto menuRestart = Menu::create(restartGame, NULL);
        menuRestart->setPosition(Vec2::ZERO);
        this->addChild(menuRestart);
        */
       
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(stageClearMusicFile, false);
        
		// criando callback, delay de 7 segundos e sequência
		auto loading = CallFunc::create(CC_CALLBACK_0(HelloWorld::updateScene, this));
		auto delay = DelayTime::create(7.0f);
		auto sequence = Sequence::create(delay, loading, NULL);

		// executa a sequencia criada
		this->runAction(sequence);
    } else {
        // som da explosao
        auto audioExplosion = CocosDenshion::SimpleAudioEngine::getInstance();
        audioExplosion->playEffect(explosionFxFile, false, 1.0f, 1.0f, 1.0f);
        
        auto particle = ParticleSmoke::createWithTotalParticles(300);
        particle->setDuration(0.5f);
        particle->setPosition(Vec2(_cannon_gun2->getPosition()));
        this->addChild(particle);
        particle->setAutoRemoveOnFinish(true);
    }
    
    return true;
}

void HelloWorld::updateScene()
{
	Director::getInstance()->replaceScene(StageSelectionScene::createScene());
}

void HelloWorld::updateCooldown()
{
	isCooldown = false;
}

void HelloWorld::pauseGame()
{
	isPaused = !(isPaused);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	if (isPaused)
	{
		Director::getInstance()->pause();
		pauseLayer->setVisible(true);
	}
	else
	{
		pauseLayer->setVisible(false);
		Director::getInstance()->resume();
	}
}

// função para sair da cena
void HelloWorld::quitGame()
{
	pauseLayer->setVisible(false);
	Director::getInstance()->resume();
	unscheduleUpdate();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(MenuScene::createScene());
}

bool HelloWorld::canExplodeTarget()
{
    _countHitTarget2++;
    
    int percentage = (100 - (_countHitTarget2 * 10));
    _labelLifeCannon2->setString(std::to_string(percentage) + "%");
    
    if (percentage <= 70 && percentage >= 50 ) {
        _labelLifeCannon2->setColor(cocos2d::Color3B::YELLOW);
    }
    else if (percentage < 50) {
        _labelLifeCannon2->setColor(cocos2d::Color3B::RED);
    }
    
    cocos2d::log("Alvo atingido %d vezes.", _countHitTarget2);
    if (_countHitTarget2 == 10) {
        // autorizacao para explodir o alvo
        cocos2d::log("Alvo atingido 10 vezes!");
        return true;
    }
    return false;
}

void HelloWorld::restartScence(Ref* pSender)
{
    cocos2d::log("restart game");
    auto newScene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(newScene);
}