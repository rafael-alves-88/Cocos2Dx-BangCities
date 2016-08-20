#include "PauseLayer.h"
#include "Resources.h"
#include "HelloWorldScene.h"

bool PauseLayer::init()
{
	if (!Layer::init())
		return false;

	size = Director::getInstance()->getWinSize();
	TTFConfig ttfConfig(font_riffic, 64.0f);

	auto layer = LayerColor::create();
	layer->setOpacity(220);
	this->addChild(layer);

	auto label = Label::createWithTTF(ttfConfig, "PAUSADO", TextHAlignment::CENTER);
	label->setPosition(Vec2(size.width / 2, size.height * 0.75f));
	this->addChild(label);

	auto playItem = MenuItemImage::create(
		uiPlayNormal,
		uiPlayPressed,
		this,
		menu_selector(PauseLayer::resumeGameCallback));
	playItem->setPosition(Vec2(size.width * 0.35f, size.height / 2));

	auto quitItem = MenuItemImage::create(
		uiBackNormal,
		uiBackPressed,
		this,
		menu_selector(PauseLayer::quitGameCallback));
	quitItem->setPosition(Vec2(size.width * 0.70f, size.height / 2));

	auto menu = Menu::create(playItem, quitItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void PauseLayer::resumeGameCallback(Ref* sender)
{
	HelloWorld::getInstance()->pauseGame();
}

void PauseLayer::quitGameCallback(Ref* sender)
{
	HelloWorld::getInstance()->quitGame();
}
