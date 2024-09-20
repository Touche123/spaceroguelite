#include "ui.h"
#include <iostream>
#include <format>

UI::UI()
{
	
}

void UI::init(sf::RenderWindow* window)
{
	this->window = window;

	if (!font.loadFromFile("assets/fonts/TitilliumWeb-Regular.ttf"))
		std::cout << "error" << std::endl;

	text_score.setPosition(0.0f, 0.0f);
	text_score.setFont(font);
	text_score.setString("Score: ");
	text_score.setCharacterSize(24);
	text_score.setFillColor(sf::Color::Red);
	text_score.setStyle(sf::Text::Bold);

	
	text_delta.setFont(font);
	text_delta.setString("Delta: ");
	text_delta.setCharacterSize(24);
	text_delta.setFillColor(sf::Color::Red);
	text_delta.setStyle(sf::Text::Bold);
}

void UI::Draw(sf::RenderWindow& window)
{
	window.draw(text_score);
	window.draw(text_delta);
}

void UI::update(float dt, Player& player)
{
	text_score.setString(std::format("Score: {} ", player.score));
	text_delta.setString(std::format("Delta: {} ", dt));
	text_delta.setPosition(window->getSize().x - 250, 0.0f);
}