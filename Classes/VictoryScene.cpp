#include "VictoryScene.h"
#include "StageSelectionScene.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* VictoryScene::createScene()
{
	auto scene = Scene::create();
	auto layer = VictoryScene::create();

	scene->addChild(layer);
	return scene;
}

bool VictoryScene::init()
{
	if (!Layer::init())
		return false;

	// Tamanho da tela
	Size size = Director::getInstance()->getWinSize();

	// música de game over
	SimpleAudioEngine::getInstance()->playBackgroundMusic(stageClearMusicFile, false);

	// sprite de splash
	auto splash = Sprite::create(victory);
	splash->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(splash);

	// criando callback, delay de 7 segundos e sequência
	auto loading = CallFunc::create(CC_CALLBACK_0(VictoryScene::updateScene, this));
	auto delay = DelayTime::create(7.0f);
	auto sequence = Sequence::create(delay, loading, NULL);
	
	// executa a sequencia criada
	this->runAction(sequence);

	return true;
}

// função para transição entre game over e menu (será chamada após 7 segundos)
void VictoryScene::updateScene()
{
	Director::getInstance()->replaceScene(StageSelectionScene::createScene());
}