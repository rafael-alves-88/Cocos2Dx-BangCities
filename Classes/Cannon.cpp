#include "Cannon.h"
#include "Resources.h"

// init do canh�o
bool Cannon::init()
{
	if (!Sprite::init())
		return false;

	this->initWithSpriteFrameName(cannon_player);

	return true;
}