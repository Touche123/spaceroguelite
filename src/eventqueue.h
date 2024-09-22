#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <variant>

enum class EventType {
	FireBullet,
	EnemyDeath,
	DamageEntity
};

enum class EventStatus {
	Pending,
	Processed,
	Expired // Useful if we want to keep events around but no longer process them
};

struct FireBulletEventData {
	sf::Vector2f position;
	sf::Vector2f direction;
};

struct DamageEntityEventData {
	int entityId;
	float damageAmount;
};

using EventData = std::variant<std::monostate, FireBulletEventData, DamageEntityEventData>;

struct Event {
	EventType type;
	EventStatus status = EventStatus::Pending;
	EventData data;
	
};

class EventQueue {
public:
	void addEvent(const Event& event) {
		events.push_back(event);
	}

	void processEvents() {
		for (const auto& event : events) {
			handleEvent(event);
		}

		events.clear();
	}

	std::vector<Event>& getEvents() {
		return events;
	}

	std::vector<std::reference_wrapper<Event>> getEventsByType(EventType type) {
		std::vector<std::reference_wrapper<Event>> result;
		for (auto& event : events) {
			if (event.type == type)
				result.push_back(event);
		}
		return result;
	}


	void clearProcessedEvents() {
		events.erase(std::remove_if(events.begin(), events.end(),
			[](const Event& event) {
				return event.status == EventStatus::Expired;
			}),
			events.end());
	}

	/*void clear() {
		events.clear();
	}*/

private:
	std::vector<Event> events;
	

	void handleEvent(const Event& event) {

	}
};