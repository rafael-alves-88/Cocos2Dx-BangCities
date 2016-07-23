#include "GameOverScene.h"
#include "MenuScene.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();

	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
		return false;

	// Tamanho da tela
	Size size = Director::getInstance()->getWinSize();

	// música de game over
	SimpleAudioEngine::getInstance()->playBackgroundMusic(gameOverMusicFile, false);

	// sprite de splash
	auto splash = Sprite::create(gameOver);
	splash->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(splash);

	// criando callback, delay de 11 segundos e sequência
	auto loading = CallFunc::create(CC_CALLBACK_0(GameOverScene::updateScene, this));
	auto delay = DelayTime::create(11.0f);
	auto sequence = Sequence::create(delay, loading, NULL);
	
	// executa a sequencia criada
	this->runAction(sequence);

	return true;
}

// função para transição entre game over e menu (será chamada após 11 segundos)
void GameOverScene::updateScene()
{
	Director::getInstance()->replaceScene(MenuScene::createScene());
}