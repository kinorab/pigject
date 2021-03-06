#pragma once
#include "../UIEvent.h"
#include "../cancelable.h"
#include <SFML/Window/Event.hpp>

namespace sf {
	class RenderWindow;
}

namespace game {
	class MouseMovedEvent :
		public UIEvent,
		public Cancelable {
	public:
		explicit MouseMovedEvent(const sf::Event::MouseMoveEvent eventData);
		virtual ~MouseMovedEvent() = default;
		virtual void accept(EventListenerBase & visitor) override;
		sf::Event::MouseMoveEvent moved;
	};
}