#include "MenuScene.h"
#include "SettingsScene.h"
#include "StageSelectionScene.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();

	scene->addChild(layer);
	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init())
		return false;

	// tamanho e configuração de fonte
	Size size = Director::getInstance()->getWinSize();
	TTFConfig ttfConfig(font_riffic, 64.0f);

	SimpleAudioEngine::getInstance()->playBackgroundMusic(menuSceneMusicFile, true);

	// background do menu
	auto backgroundSprite = Sprite::create(backgroundMenuScene);
	backgroundSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	backgroundSprite->setOpacity(150);
	this->addChild(backgroundSprite);

	// label de título
	auto label = Label::createWithTTF(ttfConfig, "BANG CITIES", TextHAlignment::CENTER);
	label->setPosition(Vec2(size.width / 2, size.height * 0.75f));
	label->setColor(Color3B::WHITE);
	this->addChild(label);

	// item jogar
	auto playItem = MenuItemImage::create(
		uiPlayNormal,
		uiPlayPressed,
		this,
		menu_selector(MenuScene::playCallback));
	playItem->setPosition(Vec2(size.width * 0.33f, size.height * 0.25f));

	// item de settings
	auto settingsItem = MenuItemImage::create(
		uiSettingsNormal,
		uiSettingsPressed,
		this,
		menu_selector(MenuScene::settingsCallback));
	settingsItem->setPosition(Vec2(size.width * 0.66f, size.height * 0.25f));

	// menu com opções
	auto menu = Menu::create(playItem, settingsItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

// transita para Settings
void MenuScene::settingsCallback(Ref* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	Director::getInstance()->replaceScene(SettingsScene::createScene());
}

// transita para a GameScene
void MenuScene::playCallback(Ref* sender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
	Director::getInstance()->replaceScene(StageSelectionScene::createScene());
}

// transita para o ranking
void MenuScene::rankingCallback(Ref* sender)
{
	SimpleAudioEngine::sharedEngine()->playEffect(buttonFxFile);
}