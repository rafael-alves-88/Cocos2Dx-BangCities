#include "HudLayer.h"
#include "Resources.h"
#include "HelloWorldScene.h"

bool HudLayer::init()
{
	if (!Layer::init())
		return false;

	size = Director::getInstance()->getWinSize();

	initPauseMenu();

	return true;
}

void HudLayer::initPauseMenu()
{
	// criando item de pausa
	auto pauseItem = MenuItemImage::create(
		uiPauseNormal,
		uiPausePressed,
		this,
		menu_selector(HudLayer::pauseCallback));
	pauseItem->setPosition(Vec2(size.width * 0.90f, size.height * 0.88f));
	pauseItem->setScale(0.75f);

	// criando menu e adicionando pausa
	auto menu = Menu::create(pauseItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
}

void HudLayer::pauseCallback(Ref* sender)
{
	HelloWorld::getInstance()->pauseGame();
}