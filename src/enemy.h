#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
private:

public:
	sf::RectangleShape sprite;
	float life = 10.f;
	Enemy();
};

