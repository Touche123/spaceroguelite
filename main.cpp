#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>

#include "src/player.h"
#include "src/enemy.h"
#include "src/item.h"
#include "src/ui.h"
#include "src/entity.h"
#include "src/inputsystem.h"
#include "src/physicssystem.h"
#include "src/rendersystem.h"
#include "src/bulletsystem.h"
#include "src/eventqueue.h"
#include "src/bulletfactory.h"

sf::Clock deltaClock;
sf::Time dt;
std::vector<Enemy> enemies;
std::vector<Item> items;

void SpawnEnemy(const sf::Window& window);

int main() {
  sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
  window.setMouseCursorVisible(false);

  sf::Texture playerTexture;
  playerTexture.loadFromFile("assets/textures/player.png");

  sf::Texture bulletTexture;
  bulletTexture.loadFromFile("assets/textures/player.png");

  Entity playerEntity;
  auto positionComponent = std::make_shared<PositionComponent>();
  auto velocityComponent = std::make_shared<VelocityComponent>();
  auto inputComponent    = std::make_shared<InputComponent>();
  auto spriteComponent = std::make_shared<SpriteComponent>();
  spriteComponent->sprite.setTexture(playerTexture);
  playerEntity.addComponent("Position", positionComponent);
  playerEntity.addComponent("Sprite", spriteComponent);
  playerEntity.addComponent("Velocity", velocityComponent);
  playerEntity.addComponent("Input", inputComponent);

  std::vector<Entity> entities = { playerEntity };
  std::vector<Entity> bullets;

  EventQueue eventQueue;
  BulletFactory bulletFactory(bulletTexture, 400.0f);
  CombatSystem combatSystem(eventQueue);
  InputSystem inputSystem(eventQueue, combatSystem);
  PhysicsSystem physicsSystem;
  RenderSystem renderSystem;
  BulletSystem bulletSystem(bulletFactory, eventQueue);

  Player player;
  UI ui;

  ui.init(&window);

  while (window.isOpen()) {
    sf::Event event;
    dt = deltaClock.restart();

    ui.update(dt.asSeconds(), player);

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
        window.close();
    }
    inputSystem.update(window, entities, bullets);
    bulletSystem.update(dt.asSeconds(), bullets);
    physicsSystem.update(dt.asSeconds(), entities);

    player.update(window, dt.asSeconds());

    // Iterate over all the projectiles and enemies.
    // Subract the damage from the healt of the enemy. And destroy accordingly.
    for (size_t i = 0; i < player.projectiles.size(); i++) {
        for (size_t j = 0; j < enemies.size(); j++)
        {
            if (player.projectiles[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds()))
            {
                enemies[j].life -= player.projectiles[i].damage;
                // delete the enemy if it is dead
                if (enemies[j].life < 0.f)
                {
                    player.score++;
                    Item item;
                    item.setPosition(enemies[j].sprite.getPosition());
                    items.push_back(item);
                    enemies.erase(enemies.begin() + j);
                }
                // delete the projectile
                player.projectiles.erase(player.projectiles.begin() + i);
                i--;
                break;
            }
            
            // delete the projectile if it is out of bounds
           if (player.projectiles[i].position.x < 0
                || player.projectiles[i].position.x > window.getSize().x
                || player.projectiles[i].position.y < 0
                || player.projectiles[i].position.y > window.getSize().y)
            {
                player.projectiles.erase(player.projectiles.begin() + i);
                break;
            }
        }
    }
    // handle pickups
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
            player.firerate -= 0.01f;

            if (player.firerate < 0.05f)
                player.firerate = 0.05f;
            
            items.erase(items.begin() + i);
            i--;
            break;
        }
    }

    if (enemies.size() <= 0)
        SpawnEnemy(window);

    window.clear();
    renderSystem.update(window, entities);
    renderSystem.update(window, bullets);
    //player.draw(window);
    
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
    
    ui.Draw(window);
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