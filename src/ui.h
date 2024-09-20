#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include "player.h"

class UI {
private:
	int score = 0;
	sf::Font font;
	sf::Text text_score;
	sf::Text text_delta;

	sf::RenderWindow* window;
public:
	UI();
	void init(sf::RenderWindow* window);
	void Draw(sf::RenderWindow& window);
	void update(float dt, Player& player);
};

