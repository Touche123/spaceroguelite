#pragma once
#include <SFML/Graphics.hpp>

class Item {
private:
	
public:
	Item();
	sf::Vector2f position;
	sf::RectangleShape sprite;
	void setPosition(sf::Vector2f position);
	void draw(sf::RenderWindow& window);
};