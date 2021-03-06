#include "GameStage01Scene.h"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include "MenuScene.h"
#include "StageSelectionScene.h"
#include "Constants.h"

USING_NS_CC;

// contagem de acertos no tanque 2
int _countHitTarget2;

// cooldown do tiro
boolean isCooldown;
float cooldownTime = 1.5f;

// pause
boolean isPaused;

// size
Size size;
Vec2 origin;

GameStage01* GameStage01::instance = NULL;
HudLayer* GameStage01::hudLayer = NULL;
PauseLayer* GameStage01::pauseLayer = NULL;

Scene* GameStage01::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    auto layer = GameStage01::create();

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

GameStage01* GameStage01::getInstance()
{
	if (instance == NULL)
		instance = new GameStage01();

	return instance;
}

enum class PhysicsCategory {
    None = 0,
    Cannon = (1 << 0),             // 1
    Projectile = (1 << 1),         // 2
    All = PhysicsCategory::Cannon | PhysicsCategory::Projectile // 3
};

bool GameStage01::init()
{
    if ( !LayerColor::initWithColor(Color4B(Color4B::ORANGE)))
    {
        return false;
    }
    
	initVariables();
	initBackground();
	initSound();
	initLabels();
	initPlayers();
	initTouchEvent();
    
    return true;
}

void GameStage01::initVariables()
{
	origin = Director::getInstance()->getVisibleOrigin();
	isCooldown = false;
	isPaused = false;
	_countHitTarget1 = 0;
	instance = this;

	size = Director::getInstance()->getWinSize();
}

void GameStage01::initBackground()
{
	// criando sprite de fundo
	auto background = Sprite::create(backgroundGameScene01);
	background->setPosition(Vec2(size.width / 2, size.height / 2));
	background->setOpacity(150);
	this->addChild(background);

	// adicionando terreno
	_ground = Sprite::create(game_scene_01_floor);
	_ground->setAnchorPoint(Vec2(0, 0));
	_ground->setPosition(Vec2(0, 0));
	this->addChild(_ground, 0);
}

void GameStage01::initSound()
{
	bool hasSound = UserDefault::getInstance()->getBoolForKey(SOUND_SETTINGS, true);

	if (hasSound) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(gameScene_01_01MusicFile, true);
	}
	else {
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}

void GameStage01::initLabels()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::createWithTTF("Bang Cities", font_markerfelt, 48);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);

	// label pra mostrar msg de vitória
	_labelWin = Label::createWithTTF("", font_markerfelt, 60);
	_labelWin->setColor(Color3B::BLACK);
	_labelWin->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	this->addChild(_labelWin);

	// label com a porcentagem (vida) do tanque 2
	_labelLifeCannon2 = Label::createWithTTF("100%", font_markerfelt, 48);

	_labelLifeCannon2->setPosition(Vec2(visibleSize.width - 100, _ground->getContentSize().height / 2));
	this->addChild(_labelLifeCannon2);
}

void GameStage01::initPlayers()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

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
}

void GameStage01::initTouchEvent()
{
	// Para travar o evento de touch
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(GameStage01::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameStage01::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameStage01::onTouchBegan, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GameStage01::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameStage01::onContactBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool GameStage01::onTouchBegan(Touch* touch, Event* event)
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
		bool hasSfx = UserDefault::getInstance()->getBoolForKey(SFX_SETTINGS, true);
		if (hasSfx) {
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect(cannonShotFxFile, false, 1.0f, 1.0f, 1.0f);
		}

		// criando callback, cooldown para próximo tiro
		auto loading = CallFunc::create(CC_CALLBACK_0(GameStage01::updateCooldown, this));
		auto delay = DelayTime::create(cooldownTime);
		auto sequence = Sequence::create(delay, loading, NULL);

		// executa a sequencia criada
		this->runAction(sequence);
	}
    return true;
}

void GameStage01::onTouchEnded(Touch* touch, Event* event)
{
    cocos2d::log("touch ended");
}

void GameStage01::onTouchMoved(Touch* touch, Event* event)
{
    cocos2d::log("touch moved");
}

void GameStage01::onTouchCancelled(Touch* touch, Event* event)
{
    cocos2d::log("touch cancelled");
}

bool GameStage01::onContactBegan(PhysicsContact &contact)
{
    cocos2d::log("on contact began");

    if (canExplodeTarget()) {
        // som da explosao
		bool hasSfx = UserDefault::getInstance()->getBoolForKey(SFX_SETTINGS, true);
		if (hasSfx) {
			auto audioExplosion = CocosDenshion::SimpleAudioEngine::getInstance();
			audioExplosion->playEffect(explosionFxFile, false, 1.0f, 1.0f, 1.0f);
		}
        
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
        
        auto restartGame = MenuItemLabel::create(labelRestart, CC_CALLBACK_1(GameStage01::restartScence, this));
        
        restartGame->setPosition(Vec2(origin.x + this->visibleSize.width/2, this->origin.y + this->visibleSize.height/2));
        
        auto menuRestart = Menu::create(restartGame, NULL);
        menuRestart->setPosition(Vec2::ZERO);
        this->addChild(menuRestart);
        */
       
		bool hasSound = UserDefault::getInstance()->getBoolForKey(SOUND_SETTINGS, true);

		if (hasSound) {
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(stageClearMusicFile, true);
		}
		else {
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		}
        
		// criando callback, delay de 7 segundos e sequência
		auto loading = CallFunc::create(CC_CALLBACK_0(GameStage01::updateScene, this));
		auto delay = DelayTime::create(7.0f);
		auto sequence = Sequence::create(delay, loading, NULL);

		// executa a sequencia criada
		this->runAction(sequence);
    } else {
        // som da explosao
		bool hasSfx = UserDefault::getInstance()->getBoolForKey(SFX_SETTINGS, true);
		if (hasSfx) {
			auto audioExplosion = CocosDenshion::SimpleAudioEngine::getInstance();
			audioExplosion->playEffect(explosionFxFile, false, 1.0f, 1.0f, 1.0f);
		}
        
        auto particle = ParticleSmoke::createWithTotalParticles(300);
        particle->setDuration(0.5f);
        particle->setPosition(Vec2(_cannon_gun2->getPosition()));
        this->addChild(particle);
        particle->setAutoRemoveOnFinish(true);
    }
    
    return true;
}

void GameStage01::updateScene()
{
	Director::getInstance()->replaceScene(StageSelectionScene::createScene());
}

void GameStage01::updateCooldown()
{
	isCooldown = false;
}

void GameStage01::pauseGame()
{
	isPaused = !(isPaused);
	bool hasSfx = UserDefault::getInstance()->getBoolForKey(SFX_SETTINGS, true);
	if (hasSfx) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	}
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
void GameStage01::quitGame()
{
	pauseLayer->setVisible(false);
	Director::getInstance()->resume();
	unscheduleUpdate();
	bool hasSfx = UserDefault::getInstance()->getBoolForKey(SFX_SETTINGS, true);
	if (hasSfx) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	}
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(MenuScene::createScene());
}

bool GameStage01::canExplodeTarget()
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

void GameStage01::restartScence(Ref* pSender)
{
    cocos2d::log("restart game");
    auto newScene = GameStage01::createScene();
    Director::getInstance()->replaceScene(newScene);
}