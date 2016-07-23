#include "GameScene01.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "VictoryScene.h"
#include "Resources.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

GameScene01* GameScene01::instance = NULL;
HudLayer* GameScene01::hudLayer = NULL;
PauseLayer* GameScene01::pauseLayer = NULL;

GameScene01::GameScene01()
{
}

GameScene01::~GameScene01()
{

}

GameScene01* GameScene01::getInstance()
{
	if (instance == NULL)
		instance = new GameScene01();

	return instance;
}

Scene* GameScene01::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene01::create();

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

bool GameScene01::init()
{
	if (!Layer::init())
		return false;

	isPaused = false;
	instance = this;

	// tamanho da tela
	size = Director::getInstance()->getWinSize();

	SimpleAudioEngine::getInstance()->playBackgroundMusic(gameScene_01_01MusicFile, true);

	// inicializando input
	initInput();

	// inicializando background
	initBackground();

	// adicionando os objetos do jogo
	initGameObjects();

	// função que ativa update (gameloop)
	scheduleUpdate();

	schedule(schedule_selector(GameScene01::updateGameLogic), .5f);



	// botões temporários
	// cria item de game over
	auto gameOver = MenuItemImage::create(
		uiCloseNormal,
		uiClosePressed,
		this,
		menu_selector(GameScene01::gameOverCallback));
	gameOver->setPosition(Vec2(size.width * 0.85f, size.height * 0.25f));

	// cria item de vitória
	auto victory = MenuItemImage::create(
		uiNextNormal,
		uiNextPressed,
		this,
		menu_selector(GameScene01::victoryCallback));
	victory->setPosition(Vec2(size.width * 0.65f, size.height * 0.25f));

	// cria menu e adiciona item
	auto menu = Menu::create(gameOver, victory, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void GameScene01::initInput()
{
	// registrando listener para touch
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene01::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene01::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene01::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameScene01::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// inicializando estado da tela pressionada
	isTouchDown = false;

	// inicializando posições x e y 
	initialTouchPosition[0] = 0;
	initialTouchPosition[1] = 0;
}

void GameScene01::initBackground()
{
	// criando sprite de fundo
	auto background = Sprite::create(backgroundGameScene01);
	background->setPosition(Vec2(size.width / 2, size.height / 2));
	background->setOpacity(150);
	this->addChild(background);
}

void GameScene01::initGameObjects()
{
	
}

// usuário tocou na tela
bool GameScene01::onTouchBegan(Touch* touch, Event* pEvent)
{
	//CCLOG("Touch Began");

	initialTouchPosition[0] = touch->getLocation().x;
	initialTouchPosition[1] = touch->getLocation().y;
	currentTouchPosition[0] = touch->getLocation().x;
	currentTouchPosition[1] = touch->getLocation().y;

	isTouchDown = true;

	Vec2 location = touch->getLocationInView();
	location = Director::getInstance()->convertToGL(location);

	return true;
}

// usuário tirou dedo da tela
void GameScene01::onTouchEnded(Touch* touch, Event* pEvent)
{
	//CCLOG("Touch Ended");

	isTouchDown = false;
}

// usuário arrastou dedo na tela
void GameScene01::onTouchMoved(Touch* touch, Event* pEvent)
{
	//CCLOG("Touch Moved");

	currentTouchPosition[0] = touch->getLocation().x;
	currentTouchPosition[1] = touch->getLocation().y;
}

// usuário cancelou toque
void GameScene01::onTouchCancelled(Touch* touch, Event* pEvent)
{
	onTouchEnded(touch, pEvent);
}

void GameScene01::pauseGame()
{
	isPaused = !(isPaused);

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
void GameScene01::quitGame()
{
	pauseLayer->setVisible(false);
	Director::getInstance()->resume();
	unscheduleUpdate();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(MenuScene::createScene());
}

void GameScene01::updateGameLogic(float dt)
{
	
}

// atualiza apenas gestos (swipe)
void GameScene01::updateSwipeInput(float dt)
{
	if (true == isTouchDown)
	{
		if (initialTouchPosition[0] - currentTouchPosition[0] > size.width * 0.05f)
		{
			CCLOG("SWIPED LEFT");

			isTouchDown = false;
		}
		else if (initialTouchPosition[0] - currentTouchPosition[0] < -size.width * 0.05f)
		{
			CCLOG("SWIPED RIGHT");

			isTouchDown = false;
		}
		else if (initialTouchPosition[1] - currentTouchPosition[1] > size.height * 0.05f)
		{
			CCLOG("SWIPED DOWN");

			isTouchDown = false;
		}
		else if (initialTouchPosition[1] - currentTouchPosition[1] < -size.height * 0.05f)
		{
			CCLOG("SWIPED UP");

			isTouchDown = false;
		}
	}
}

// update geral da cena
void GameScene01::update(float dt)
{
	//if (!isPaused)
	//{
		updateSwipeInput(dt);
	//}
}


void GameScene01::gameOverCallback(Ref* sender)
{
	// vai para Game Over
	Director::getInstance()->replaceScene(GameOverScene::createScene());
}

void GameScene01::victoryCallback(Ref* sender)
{
	// vai para Vitória
	Director::getInstance()->replaceScene(VictoryScene::createScene());
}