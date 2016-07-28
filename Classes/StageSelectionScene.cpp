#include "StageSelectionScene.h"
#include "SettingsScene.h"
#include "MenuScene.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
#include "GameScene01.h"
using namespace CocosDenshion;

Scene* StageSelectionScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StageSelectionScene::create();

	scene->addChild(layer);
	return scene;
}

bool StageSelectionScene::init()
{
	if (!Layer::init())
		return false;

	// tamanho e configuração de fonte
	Size size = Director::getInstance()->getWinSize();
	TTFConfig ttfConfig(font_riffic, 64.0f);

	SimpleAudioEngine::getInstance()->playBackgroundMusic(sceneSelectionMusicFile, true);

	// background do menu
	auto backgroundSprite = Sprite::create(backgroundSceneSelection);
	backgroundSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	backgroundSprite->setOpacity(130);
	this->addChild(backgroundSprite);

	// label de título
	auto labelTitle = Label::createWithTTF(ttfConfig, "SELECAO DE FASE", TextHAlignment::CENTER);
	labelTitle->setPosition(Vec2(size.width / 2, size.height * 0.85f));
	labelTitle->setColor(Color3B::WHITE);
	this->addChild(labelTitle);

	// fase 1
	auto stage1 = MenuItemImage::create(
		stage1Flag,
		stage1FlagPressed,
		this,
		menu_selector(StageSelectionScene::stage1Callback));
	stage1->setPosition(Vec2(size.width * 0.25f, size.height * 0.55f));

	// label de fase 1
	auto labelStage1 = Label::createWithTTF(ttfConfig, "Rio Grande do Sul", TextHAlignment::CENTER);
	labelStage1->setPosition(Vec2(size.width * 0.25f, size.height * 0.40f));
	labelStage1->setColor(Color3B::WHITE);
	labelStage1->setSystemFontSize(30);
	this->addChild(labelStage1);

	// fase 2
	auto stage2 = MenuItemImage::create(
		stage2Flag,
		stage2FlagPressed,
		this,
		menu_selector(StageSelectionScene::stage2Callback));
	stage2->setPosition(Vec2(size.width * 0.50f, size.height * 0.55f));
	stage2->setOpacity(100);

	// label de fase 2
	auto labelStage2 = Label::createWithTTF(ttfConfig, "Sao Paulo", TextHAlignment::CENTER);
	labelStage2->setPosition(Vec2(size.width * 0.50f, size.height * 0.40f));
	labelStage2->setColor(Color3B::WHITE);
	labelStage2->setSystemFontSize(30);
	this->addChild(labelStage2);

	// fase 3
	auto stage3 = MenuItemImage::create(
		stage3Flag,
		stage3FlagPressed,
		this,
		menu_selector(StageSelectionScene::stage3Callback));
	stage3->setPosition(Vec2(size.width * 0.75f, size.height * 0.55f));
	stage3->setOpacity(100);

	// label de fase 3
	auto labelStage3 = Label::createWithTTF(ttfConfig, "Rio de Janeiro", TextHAlignment::CENTER);
	labelStage3->setPosition(Vec2(size.width * 0.75f, size.height * 0.40f));
	labelStage3->setColor(Color3B::WHITE);
	labelStage3->setSystemFontSize(30);
	this->addChild(labelStage3);

	// cria item de voltar
	auto backItem = MenuItemImage::create(
		uiBackNormal,
		uiBackPressed,
		this,
		menu_selector(StageSelectionScene::backCallback));
	backItem->setPosition(Vec2(size.width * 0.85f, size.height * 0.20f));

	// menu com opções
	auto menu = Menu::create(stage1, stage2, stage3, backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

// transita para a GameScene 1
void StageSelectionScene::stage1Callback(Ref* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(GameScene01::createScene());
}

// transita para a GameScene 2
void StageSelectionScene::stage2Callback(Ref* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//Director::getInstance()->replaceScene(GameScene02::createScene());
}

// transita para a GameScene 3
void StageSelectionScene::stage3Callback(Ref* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//Director::getInstance()->replaceScene(GameScene03::createScene());
}

void StageSelectionScene::backCallback(Ref* sender)
{
	// retorna para MenuScene
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	Director::getInstance()->replaceScene(MenuScene::createScene());
}