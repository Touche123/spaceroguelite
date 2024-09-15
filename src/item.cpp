#include "item.h"

Item::Item()
{
	sprite.setFillColor(sf::Color::Yellow);
	sprite.setSize({ 15.f, 15.f });
}

void Item::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Item::setPosition(sf::Vector2f position)
{
	this->position = position;
	sprite.setPosition(position);
}