#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
private:

public:
	sf::RectangleShape sprite;
	float life = 3.f;
	Enemy();
	void draw(sf::RenderWindow& window);
};

