#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>

#include "src/item.h"
#include "src/ui.h"
#include "src/entity.h"
#include "src/systems/inputsystem.h"
#include "src/systems/physicssystem.h"
#include "src/systems/rendersystem.h"
#include "src/systems/bulletsystem.h"
#include "src/eventqueue.h"
#include "src/bulletfactory.h"
#include "src/systems/reticleSystem.h"
#include "src/systems/collisionsystem.h"
#include "src/systems/healthSystem.h"
#include "src/systems/enemyShootingSystem.h"
#include "src/movementBehavior.h"
#include "src/systems/MovementSystem.h"
#include "src/systems/entitySystem.h"

sf::Clock deltaClock;
sf::Time dt;

Entity createReticleEntity(sf::Texture& reticleTexture);
Entity createEnemyEntity(sf::Texture& texture, sf::Vector2f position, float health);
Entity playerEntity;

void SpawnEnemy(const sf::Window& window, EntitySystem& entitySystem, sf::Texture& texture);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	window.setMouseCursorVisible(false);

	sf::Texture playerTexture;
	playerTexture.loadFromFile("assets/textures/player.png");

	sf::Texture enemyTexture;
	enemyTexture.loadFromFile("assets/textures/enemy.png");

	sf::Texture playerBulletTexture;
	playerBulletTexture.loadFromFile("assets/textures/playerBullet.png");

	sf::Texture enemyBulletTexture;
	enemyBulletTexture.loadFromFile("assets/textures/enemyBullet.png");

	sf::Texture reticleTexture;
	reticleTexture.loadFromFile("assets/textures/reticle.png");

	Entity reticle = createReticleEntity(reticleTexture);
	
	EntitySystem entitySystem;

	auto positionComponent = std::make_shared<PositionComponent>();
	auto velocityComponent = std::make_shared<VelocityComponent>();
	auto inputComponent = std::make_shared<InputComponent>();
	auto spriteComponent = std::make_shared<SpriteComponent>();
	auto collisionComponent = std::make_shared<CollisionComponent>();
	collisionComponent->bounds = spriteComponent->sprite.getGlobalBounds();
	auto healthComponent = std::make_shared<HealthComponent>(10000.f, 2.f);
	auto playerComponent = std::make_shared<PlayerComponent>();

	spriteComponent->sprite.setTexture(playerTexture);
	playerEntity.addComponent("Position", positionComponent);
	playerEntity.addComponent("Sprite", spriteComponent);
	playerEntity.addComponent("Velocity", velocityComponent);
	playerEntity.addComponent("Input", inputComponent);
	playerEntity.addComponent("Collision", collisionComponent);
	playerEntity.addComponent("Health", healthComponent);
	playerEntity.addComponent("Player", playerComponent);

	//std::vector<Entity> entities = { playerEntity };

	entitySystem.AddEntity(playerEntity);

	EventQueue eventQueue;

	CombatSystem combatSystem(eventQueue);
	InputSystem inputSystem(eventQueue, combatSystem);
	PhysicsSystem physicsSystem;
	RenderSystem renderSystem;

	ReticleSystem reticleSystem;
	CollisionSystem collisionSystem(eventQueue);
	HealthSystem healthSystem(eventQueue, entitySystem);
	MovementSystem movementSystem;

	BulletFactory bulletFactory(enemyBulletTexture);
	BulletSystem bulletSystem(bulletFactory, eventQueue);
	EnemyShootingSystem enemyShootingSystem;

	UI ui;

	ui.init(&window);

	while (window.isOpen())
	{
		sf::Event event;
		dt = deltaClock.restart();

		ui.update(dt.asSeconds(), playerEntity);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
				window.close();
		}
		inputSystem.update(window, entitySystem, bulletFactory.playerBullets);
		enemyShootingSystem.update(playerEntity, entitySystem, bulletFactory, dt.asSeconds());
		movementSystem.update(entitySystem, dt.asSeconds());

		bulletSystem.update(dt.asSeconds());
		physicsSystem.update(dt.asSeconds(), entitySystem);
		reticleSystem.update(window, reticle);
		collisionSystem.checkPlayerEnemyCollisions(playerEntity, entitySystem, dt.asSeconds());
		collisionSystem.updateEnemyBullets(playerEntity, bulletFactory.enemyBullets);
		collisionSystem.updatePlayerBullets(bulletFactory.playerBullets, entitySystem);
		healthSystem.update();

		entitySystem.remove_marked_for_removal();
		
		if (entitySystem.GetEntitiesCount() <= 1)
			SpawnEnemy(window, entitySystem, enemyTexture);
		window.clear();

		eventQueue.clearProcessedEvents();

		renderSystem.update(window, entitySystem);
		renderSystem.update(window, bulletFactory.playerBullets);
		renderSystem.update(window, bulletFactory.enemyBullets);
		renderSystem.drawReticle(window, reticle);

		ui.draw(window);
		window.display();
	}

	return 0;
}

Entity createReticleEntity(sf::Texture& reticleTexture)
{
	Entity reticle;
	auto positionComponent = std::make_shared<PositionComponent>();
	auto spriteComponent = std::make_shared<SpriteComponent>();
	spriteComponent->sprite.setTexture(reticleTexture);

	reticle.addComponent("Position", positionComponent);
	reticle.addComponent("Sprite", spriteComponent);

	return reticle;
}

Entity createEnemyEntity(sf::Texture& texture, sf::Vector2f position, float health)
{
	Entity enemy;
	auto positionComponent = std::make_shared<PositionComponent>();
	positionComponent->position = position;
	auto velocityComponent = std::make_shared<VelocityComponent>();

	auto spriteComponent = std::make_shared<SpriteComponent>();
	spriteComponent->sprite.setTexture(texture);
	auto collisionComponent = std::make_shared<CollisionComponent>();
	collisionComponent->bounds = spriteComponent->sprite.getGlobalBounds();
	auto healthComponent = std::make_shared<HealthComponent>(1000.f, 0.f);


	auto shootingComponent = std::make_shared<ShootingComponent>(2.f);

	std::shared_ptr<IMovementBehavior> movementBehavior;
	movementBehavior = std::make_shared<FollowPlayerMovement>(playerEntity);

	auto behaviorComponent = std::make_shared<BehaviorComponent>(movementBehavior);
	auto audioComponent = std::make_shared<AudioComponent>();
	audioComponent->loadSound("Death", "assets/sounds/death.wav");
	audioComponent->loadSound("Shoot", "assets/sounds/shoot.wav");
	enemy.addComponent("Audio", audioComponent);

	enemy.addComponent("Position", positionComponent);
	enemy.addComponent("Velocity", velocityComponent);
	enemy.addComponent("Sprite", spriteComponent);
	enemy.addComponent("Collision", collisionComponent);
	enemy.addComponent("Health", healthComponent);
	enemy.addComponent("Shooting", shootingComponent);
	enemy.addComponent("MovementBehavior", behaviorComponent);

	return enemy;
}

void SpawnEnemy(const sf::Window& window, EntitySystem& entitySystem, sf::Texture& texture)
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

		entitySystem.AddEntity(enemy);
	}
}