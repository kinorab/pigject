#include "joystickHandler.h"
#include "../interact/container.h"

namespace game {
	JoystickHandler::JoystickHandler() noexcept {
	}

	void JoystickHandler::handler(const sf::Event & event, Container & thing) {
		if (event.type == sf::Event::JoystickConnected) {
			handleJoystickConnected(event, thing);
		}
		else if (event.type == sf::Event::JoystickDisconnected) {
			handleJoystickDisconnected(event, thing);
		}
		else if (event.type == sf::Event::JoystickMoved) {
			handleJoystickMoved(event, thing);
		}
		else if (event.type == sf::Event::JoystickButtonPressed) {
			handleJoystickPressed(event, thing);
		}
		else if (event.type == sf::Event::JoystickButtonReleased) {
			handleJoystickReleased(event, thing);
		}
	}

	void JoystickHandler::handleJoystickConnected(const sf::Event & event, Container & thing) {
		thing.dispatchEvent(JoystickConnectedEvent(event.joystickConnect));
	}

	void JoystickHandler::handleJoystickDisconnected(const sf::Event & event, Container & thing) {
		thing.dispatchEvent(JoystickDisconnectedEvent(event.joystickConnect));
	}

	void JoystickHandler::handleJoystickMoved(const sf::Event & event, Container & thing) {
		thing.dispatchEvent(JoystickMovedEvent(event.joystickMove));
	}

	void JoystickHandler::handleJoystickPressed(const sf::Event & event, Container & thing) {
		thing.dispatchEvent(JoystickPressedEvent(event.joystickButton));
	}

	void JoystickHandler::handleJoystickReleased(const sf::Event & event, Container & thing) {
		thing.dispatchEvent(JoystickReleasedEvent(event.joystickButton));
	}
}