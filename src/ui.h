#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include "entity.h"

class UI {
private:
	int score = 0;
	sf::Font font;
	sf::Text text_score;
	sf::Text text_playerHealth;
	sf::Text text_delta;

	sf::RenderWindow* window;
public:
	UI();
	void init(sf::RenderWindow* window);
	void draw(sf::RenderWindow& window);
	void update(float dt, Entity& player);
};

