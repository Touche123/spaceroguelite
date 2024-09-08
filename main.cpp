#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
  sf::CircleShape rectile(100.f);
  sf::RectangleShape playerShape(sf::Vector2f(50, 50));
  rectile.setFillColor(sf::Color::Green);
  rectile.setOrigin(100.f, 100.f);
  sf::Vector2f playerDirection = sf::Vector2f(0, 0);
  sf::Vector2f playerPosition = sf::Vector2f(50, 50);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
          playerDirection.x = -1;
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
          playerDirection.x = 1;
      else
          playerDirection.x = 0;

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
          playerDirection.y = -1;
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
          playerDirection.y = 1;
      else
          playerDirection.y = 0;
    }
    playerPosition.x += playerDirection.x * 0.01;
    playerPosition.y += playerDirection.y * 0.01;

    window.clear();
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
    rectile.setPosition(localPosition.x, localPosition.y);

    playerShape.setPosition(playerPosition.x, playerPosition.y);
    window.draw(rectile);
    window.draw(playerShape);
    window.display();
  }

  return 0;
}