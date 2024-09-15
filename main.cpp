#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#include "src/player.h"
#include "src/enemy.h"
#include "src/item.h"

sf::Clock deltaClock;
sf::Time dt;
std::vector<Enemy> enemies;
std::vector<Item> items;

void SpawnEnemy(const sf::Window& window);

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
  window.setMouseCursorVisible(false);
  sf::Font font;
  if (!font.loadFromFile("assets/fonts/TitilliumWeb-Regular.ttf"))
    std::cout << "error" << std::endl;

  sf::Text text_delta;
  text_delta.setPosition(0.0f, 0.0f);
  text_delta.setFont(font);
  text_delta.setString("Thest");
  text_delta.setCharacterSize(24);
  text_delta.setFillColor(sf::Color::Red);
  text_delta.setStyle(sf::Text::Bold);

  Player player;

  while (window.isOpen()) {
    sf::Event event;
    dt = deltaClock.restart();

    std::ostringstream ss;
    ss << "Deltatime: " << dt.asSeconds();
    text_delta.setString(ss.str());

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
        window.close();
    }

    player.update(window, dt.asSeconds());

    // Iterate over all the projectiles and enemies.
    // Subract the damage from the healt of the enemy. And destroy accordingly.
    for (size_t i = 0; i < player.projectiles.size(); i++) {
        for (size_t j = 0; j < enemies.size(); j++) {
            if (player.projectiles[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds())) {
                enemies[j].life -= player.projectiles[i].damage;
                if (enemies[j].life < 0.f) {
                    Item item;
                    item.setPosition(enemies[j].sprite.getPosition());
                    items.push_back(item);
                    enemies.erase(enemies.begin() + j);
                }
                    
                player.projectiles.erase(player.projectiles.begin() + i);
                i--;
                break;
            }
        }
    }

    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
            player.firerate -= 0.01f;
            items.erase(items.begin() + i);
            i--;
            break;
        }
    }

    if (enemies.size() <= 0)
        SpawnEnemy(window);

    window.clear();
    
    player.draw(window);
    
    for (auto& enemy: enemies) {
        enemy.draw(window);
    }

    for (auto& item : items)
    {
        item.draw(window);
    }

    for (auto& projectile : player.projectiles) {
        projectile.draw(window);
    }
    
    window.draw(text_delta);
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