#include "enemy.h"

Enemy::Enemy()
{
	sprite.setPosition(200.f, 200.f);
	sprite.setSize({ 40.f, 40.f });
	sprite.setFillColor(sf::Color::Green);
}