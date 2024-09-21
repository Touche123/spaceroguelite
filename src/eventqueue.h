#pragma once
#include <SFML/Graphics.hpp>

enum class EventType {
	FireBullet
};

struct Event {
	EventType type;
	sf::Vector2f position;
	sf::Vector2f direction;
};

class EventQueue {
public:
	void addEvent(const Event& event) {
		events.push_back(event);
	}

	std::vector<Event>& getEvents() {
		return events;
	}

	void clear() {
		events.clear();
	}

private:
	std::vector<Event> events;
};