#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#include "src/player.h"
#include "src/enemy.h"

sf::Clock deltaClock;
sf::Time dt;
std::vector<Enemy> enemies;
void SpawnEnemy(const sf::Window& window);

int main() {
   
  Player player;

  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
  window.setMouseCursorVisible(false);
  sf::Font font;
  sf::Text textDelta;

  if (!font.loadFromFile("assets/fonts/TitilliumWeb-Regular.ttf"))
    std::cout << "error" << std::endl;

  textDelta.setPosition(0.0f, 0.0f);
  textDelta.setFont(font);
  textDelta.setString("Thest");
  textDelta.setCharacterSize(24);
  textDelta.setFillColor(sf::Color::Red);
  textDelta.setStyle(sf::Text::Bold);

  enemies.push_back(Enemy());

  while (window.isOpen()) {
    sf::Event event;
    dt = deltaClock.restart();

    player.update(window, dt.asSeconds());

    std::ostringstream ss;
    ss << "Deltatime: " << dt.asSeconds();
    textDelta.setString(ss.str());

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
        window.close();
    }

    // Iterate over all the projectiles and enemies.
    // Subract the damage from the healt of the enemy. And destroy accordingly.
    for (size_t i = 0; i < player.Projectiles.size(); i++) {
        for (size_t j = 0; j < enemies.size(); j++) {
            if (player.Projectiles[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds())) {
                enemies[j].life -= player.Projectiles[i].damage;
                if (enemies[j].life < 0.f)
                    enemies.erase(enemies.begin() + j);
                player.Projectiles.erase(player.Projectiles.begin() + i);
                i--;
                break;
            }
        }
    }

    if (enemies.size() <= 0)
        SpawnEnemy(window);

    window.clear();
    
    window.draw(player.sprite);
    window.draw(player.rectile);
    
    for (auto const& enemy: enemies) {
        window.draw(enemy.sprite);
    }

    for (auto const &projectile : player.Projectiles)
    {
        window.draw(projectile.sprite);
    }
    
    window.draw(textDelta);
    window.display();
  }

  return 0;
}

void SpawnEnemy(const sf::Window& window)
{
    // Spawn an enemy at a random position
    for (int i = 0; i < 5; ++i)
    { // Spawning 5 random enemies
        Enemy enemy; // Create an enemy of size 50x50
       
        // 1. Generate random position within the window's bounds
        float randomX = static_cast<float>(std::rand() % (window.getSize().x - static_cast<int>(enemy.sprite.getSize().x)));
        float randomY = static_cast<float>(std::rand() % (window.getSize().y - static_cast<int>(enemy.sprite.getSize().y)));

        // 2. Set the enemy's position to the random coordinates
        enemy.sprite.setPosition(randomX, randomY);

        // 3. Add enemy to the list of enemies
        enemies.push_back(enemy);
    }
}