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
#include "src/reticleSystem.h"
#include "src/collisionsystem.h"
#include "src/healthSystem.h"

sf::Clock deltaClock;
sf::Time dt;

Entity createReticleEntity(sf::Texture& reticleTexture);
Entity createEnemyEntity(sf::Texture& texture, sf::Vector2f position, float health);
void SpawnEnemy(const sf::Window& window, std::vector<Entity>& enemies, sf::Texture& texture);

int main() {
  sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
  window.setMouseCursorVisible(false);

  sf::Texture playerTexture;
  playerTexture.loadFromFile("assets/textures/player.png");

  sf::Texture bulletTexture;
  bulletTexture.loadFromFile("assets/textures/player.png");
  sf::Texture reticleTexture;
  reticleTexture.loadFromFile("assets/textures/player.png");

  Entity reticle = createReticleEntity(reticleTexture);
  Entity playerEntity;
  auto positionComponent = std::make_shared<PositionComponent>();
  auto velocityComponent = std::make_shared<VelocityComponent>();
  auto inputComponent    = std::make_shared<InputComponent>();
  auto spriteComponent = std::make_shared<SpriteComponent>();
  auto collisionComponent = std::make_shared<CollisionComponent>();
  collisionComponent->bounds = spriteComponent->sprite.getGlobalBounds();
  auto healthComponent = std::make_shared<HealthComponent>(100.f, 2.f);

  spriteComponent->sprite.setTexture(playerTexture);
  playerEntity.addComponent("Position", positionComponent);
  playerEntity.addComponent("Sprite", spriteComponent);
  playerEntity.addComponent("Velocity", velocityComponent);
  playerEntity.addComponent("Input", inputComponent);
  playerEntity.addComponent("Collision", collisionComponent);
  playerEntity.addComponent("Health", healthComponent);

  std::vector<Entity> entities = { playerEntity };
  std::vector<Entity> bullets;
  std::vector<Entity> enemies;


  EventQueue eventQueue;
  BulletFactory bulletFactory(bulletTexture, 400.0f);
  CombatSystem combatSystem(eventQueue);
  InputSystem inputSystem(eventQueue, combatSystem);
  PhysicsSystem physicsSystem;
  RenderSystem renderSystem;
  BulletSystem bulletSystem(bulletFactory, eventQueue);
  ReticleSystem reticleSystem;
  CollisionSystem collisionSystem;
  HealthSystem healthSystem;

  Player player;
  UI ui;

  ui.init(&window);

  SpawnEnemy(window, enemies, playerTexture);

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
    reticleSystem.update(window, reticle);
    collisionSystem.checkPlayerEnemyCollisions(playerEntity, enemies, dt.asSeconds());
    collisionSystem.update(bullets, enemies);
    healthSystem.update(entities, enemies);
    if (enemies.size() <= 0)
        SpawnEnemy(window, enemies, playerTexture);
    window.clear();
    renderSystem.update(window, entities);
    renderSystem.update(window, bullets);
    renderSystem.update(window, enemies);
    renderSystem.drawReticle(window, reticle);
    
    ui.Draw(window);
    window.display();
  }

  return 0;
}

Entity createReticleEntity(sf::Texture& reticleTexture) {
    Entity reticle;
    auto positionComponent = std::make_shared<PositionComponent>();
    auto spriteComponent = std::make_shared<SpriteComponent>();
    spriteComponent->sprite.setTexture(reticleTexture);

    reticle.addComponent("Position", positionComponent);
    reticle.addComponent("Sprite", spriteComponent);

    return reticle;
}

Entity createEnemyEntity(sf::Texture& texture, sf::Vector2f position, float health) {
    Entity enemy;
    auto positionComponent = std::make_shared<PositionComponent>();
    positionComponent->position = position;
    auto spriteComponent = std::make_shared<SpriteComponent>();
    spriteComponent->sprite.setTexture(texture);
    auto collisionComponent = std::make_shared<CollisionComponent>();
    collisionComponent->bounds = spriteComponent->sprite.getGlobalBounds();
    auto healthComponent = std::make_shared<HealthComponent>(20.f, 0.f);

    enemy.addComponent("Position", positionComponent);
    enemy.addComponent("Sprite", spriteComponent);
    enemy.addComponent("Collision", collisionComponent);
    enemy.addComponent("Health", healthComponent);

    return enemy;
}

void SpawnEnemy(const sf::Window& window, std::vector<Entity>& enemies, sf::Texture& texture)
{
    // Spawn an enemy at a random position
    for (int i = 0; i < 5; ++i)
    { // Spawning 5 random enemies
        auto enemy = createEnemyEntity(texture, { 0,0 }, 100.0f);
        auto spriteComponent = enemy.getComponent<SpriteComponent>("Sprite");
        auto positionComponent = enemy.getComponent<PositionComponent>("Position");

        // 1. Generate random position within the window's bounds
        float randomX = static_cast<float>(std::rand() % (window.getSize().x - static_cast<int>(spriteComponent->sprite.getGlobalBounds().getSize().x)));
        float randomY = static_cast<float>(std::rand() % (window.getSize().y - static_cast<int>(spriteComponent->sprite.getGlobalBounds().getSize().y)));

        positionComponent->position = { randomX, randomY };

        enemies.push_back(enemy);
    }
}