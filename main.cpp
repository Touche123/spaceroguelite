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
//sf::Time dt;
float deltaTime;

Entity createReticleEntity(sf::Texture& reticleTexture);
Entity createEnemyEntity(sf::Texture& texture, sf::Vector2f position, float health);
Entity player_entity;

void SpawnEnemy(const sf::Window& window, EntitySystem& entitySystem, sf::Texture& texture);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
	window.setMouseCursorVisible(false);

	sf::Texture player_texture;
	player_texture.loadFromFile("assets/textures/player.png");

	sf::Texture enemy_texture;
	enemy_texture.loadFromFile("assets/textures/enemy.png");

	sf::Texture player_bullet_texture;
	player_bullet_texture.loadFromFile("assets/textures/playerBullet.png");

	sf::Texture enemy_bullet_texture;
	enemy_bullet_texture.loadFromFile("assets/textures/enemyBullet.png");

	sf::Texture reticle_texture;
	reticle_texture.loadFromFile("assets/textures/reticle.png");

	Entity reticle = createReticleEntity(reticle_texture);
	
	EntitySystem entity_system;

	auto position_component		= std::make_shared<PositionComponent>();
	auto velocity_component		= std::make_shared<VelocityComponent>();
	auto input_component		= std::make_shared<InputComponent>();
	auto sprite_component		= std::make_shared<SpriteComponent>();
	auto collision_component	= std::make_shared<CollisionComponent>();
	collision_component->bounds = sprite_component->sprite.getGlobalBounds();
	auto health_component		= std::make_shared<HealthComponent>(10000.f, 2.f);
	auto player_component		= std::make_shared<PlayerComponent>();

	sprite_component->sprite.setTexture(player_texture);
	player_entity.addComponent("Position",	position_component);
	player_entity.addComponent("Sprite",	sprite_component);
	player_entity.addComponent("Velocity",	velocity_component);
	player_entity.addComponent("Input",		input_component);
	player_entity.addComponent("Collision", collision_component);
	player_entity.addComponent("Health",	health_component);
	player_entity.addComponent("Player",	player_component);

	//std::vector<Entity> entities = { playerEntity };

	entity_system.AddEntity(player_entity);

	EventQueue event_queue;

	CombatSystem combat_system(event_queue);
	InputSystem input_system(event_queue, combat_system);
	PhysicsSystem physics_system;
	RenderSystem render_system;

	ReticleSystem reticle_system;
	CollisionSystem collision_system(event_queue);
	HealthSystem health_system(event_queue, entity_system);
	MovementSystem movement_system;

	BulletFactory bullet_factory(enemy_bullet_texture);
	BulletSystem bullet_system(bullet_factory, event_queue);
	EnemyShootingSystem enemy_shooting_system;

	UI ui;

	ui.init(&window);

	while (window.isOpen())
	{
		sf::Event event;
		deltaTime = deltaClock.restart().asSeconds();

		ui.update(deltaTime, player_entity);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
				window.close();
		}
		input_system.update(window, entity_system, bullet_factory.playerBullets);
		enemy_shooting_system.update(player_entity, entity_system, bullet_factory, deltaTime);
		movement_system.update(entity_system, deltaTime);

		bullet_system.update(deltaTime);
		physics_system.update(deltaTime, entity_system);
		reticle_system.update(window, reticle);
		collision_system.checkPlayerEnemyCollisions(player_entity, entity_system, deltaTime);
		collision_system.updateEnemyBullets(player_entity, bullet_factory.enemyBullets);
		collision_system.updatePlayerBullets(bullet_factory.playerBullets, entity_system);
		health_system.update();

		entity_system.remove_marked_for_removal();
		
		if (entity_system.GetEntitiesCount() <= 1)
			SpawnEnemy(window, entity_system, enemy_texture);
		window.clear();

		event_queue.clearProcessedEvents();

		render_system.update(window, entity_system);
		render_system.update(window, bullet_factory.playerBullets);
		render_system.update(window, bullet_factory.enemyBullets);
		render_system.drawReticle(window, reticle);

		ui.draw(window);
		window.display();
	}

	return 0;
}

Entity createReticleEntity(sf::Texture& reticle_texture)
{
	Entity reticle;
	auto position_component = std::make_shared<PositionComponent>();
	auto sprite_component = std::make_shared<SpriteComponent>();
	sprite_component->sprite.setTexture(reticle_texture);

	reticle.addComponent("Position", position_component);
	reticle.addComponent("Sprite", sprite_component);

	return reticle;
}

Entity createEnemyEntity(sf::Texture& texture, sf::Vector2f position, float health)
{
	Entity enemy;
	auto position_component = std::make_shared<PositionComponent>();
	position_component->position = position;
	auto velocity_component = std::make_shared<VelocityComponent>();

	auto sprite_component = std::make_shared<SpriteComponent>();
	sprite_component->sprite.setTexture(texture);
	auto collision_component = std::make_shared<CollisionComponent>();
	collision_component->bounds = sprite_component->sprite.getGlobalBounds();
	auto health_component = std::make_shared<HealthComponent>(1000.f, 0.f);


	auto shooting_component = std::make_shared<ShootingComponent>(2.f);

	std::shared_ptr<IMovementBehavior> movement_behavior;
	movement_behavior = std::make_shared<FollowPlayerMovement>(player_entity);

	auto behavior_component = std::make_shared<BehaviorComponent>(movement_behavior);
	auto audio_component = std::make_shared<AudioComponent>();
	audio_component->loadSound("Death", "assets/sounds/death.wav");
	audio_component->loadSound("Shoot", "assets/sounds/shoot.wav");
	enemy.addComponent("Audio", audio_component);

	enemy.addComponent("Position", position_component);
	enemy.addComponent("Velocity", velocity_component);
	enemy.addComponent("Sprite", sprite_component);
	enemy.addComponent("Collision", collision_component);
	enemy.addComponent("Health", health_component);
	enemy.addComponent("Shooting", shooting_component);
	enemy.addComponent("MovementBehavior", behavior_component);

	return enemy;
}

void SpawnEnemy(const sf::Window& window, EntitySystem& entity_system, sf::Texture& texture)
{
	// Spawn an enemy at a random position
	for (int i = 0; i < 5; ++i)
	{ // Spawning 5 random enemies
		auto enemy = createEnemyEntity(texture, { 0,0 }, 100.0f);
		auto sprite_component = enemy.getComponent<SpriteComponent>("Sprite");
		auto position_component = enemy.getComponent<PositionComponent>("Position");
		
		// 1. Generate random position within the window's bounds
		float randomX = static_cast<float>(std::rand() % (window.getSize().x - static_cast<int>(sprite_component->sprite.getGlobalBounds().getSize().x)));
		float randomY = static_cast<float>(std::rand() % (window.getSize().y - static_cast<int>(sprite_component->sprite.getGlobalBounds().getSize().y)));

		position_component->position = { randomX, randomY };

		entity_system.AddEntity(enemy);
	}
}