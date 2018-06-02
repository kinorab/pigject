#include "interactiveObject.h"
#include "../event/event.h"
#include "../event/dispatchHelper.h"
#include "container.h"
#include "../definition/utility.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace game {
	InteractiveObject::InteractiveObject() :
		idCount(0),
		enabled(true) {
	}

	InteractiveObject::~InteractiveObject() {
	}

	int InteractiveObject::addEventListener(EventType type, std::function<void(Event *)> callback) {
		return addEventListener(type, callback, false);
	}

	int InteractiveObject::addEventListener(EventType type, std::function<void(Event *)> callback, bool useCapture) {
		listeners[idCount] = EventListener{ type, callback, useCapture };
		int returnId = idCount;
		idCount += 1;
		return returnId;
	}

	bool InteractiveObject::dispatchEvent(Event * event) {
		DispatchHelper helper(event);
		helper.setCurrentTarget(this);

		if (event->getPhase() == EventPhase::NONE) {
			helper.setTarget(this);
			helper.setPhase(EventPhase::CAPTURING_PHASE);
		}

		if (event->getPhase() == EventPhase::CAPTURING_PHASE && parent != nullptr) {
			parent->dispatchEvent(event);
			helper.setCurrentTarget(this);
		}

		if (event->getCurrentTarget() == event->getTarget()) {
			helper.setPhase(EventPhase::AT_TARGET);
		}

		if (!helper.isPropagationStopped()) {
			std::map<int, EventListener> tempListeners = listeners;
			std::for_each(tempListeners.begin(), tempListeners.end(),
				[&](const std::pair<const int, EventListener> & listener) {
				if (event->getType() == listener.second.type
					&& !((event->getPhase() == EventPhase::CAPTURING_PHASE && !listener.second.useCapture)
						|| (event->getPhase() == EventPhase::BUBBLING_PHASE && listener.second.useCapture))) {
					listener.second.callback(event);
				}
			});
		}

		if (event->getCurrentTarget() == event->getTarget()) {
			helper.setPhase(EventPhase::BUBBLING_PHASE);
		}

		if (event->getPhase() == EventPhase::BUBBLING_PHASE && event->getBubbles() && parent != nullptr) {
			parent->dispatchEvent(event);
			helper.setCurrentTarget(this);
		}

		if (event->getCurrentTarget() == event->getTarget()) {
			helper.setPhase(EventPhase::NONE);
		}

		return !event->getDefaultPrevented();
	}

	bool InteractiveObject::getEnabled() const {
		return enabled;
	}

	Container * InteractiveObject::getParent() {
		return parent;
	}

	void InteractiveObject::onDisabled() {
	}

	void InteractiveObject::onEnabled() {
	}

	void InteractiveObject::removeEventListener(int id) {
		listeners.erase(std::find_if(listeners.begin(), listeners.end(),
			[&](std::pair<const int, EventListener> & listener) {
			return listener.first == id;
		}));
	}

	void InteractiveObject::setEnabled(bool value) {
		if (value == enabled) {
			return;
		}

		if (value) {
			onEnabled();
		}
		else {
			onDisabled();
		}

		enabled = value;
	}

	void InteractiveObject::setParent(Container * container) {
		parent = container;
	}

	void InteractiveObject::update(const float) {
	}

	void InteractiveObject::draw(sf::RenderTarget & target, sf::RenderStates states) const {
		target.draw(*getDrawable(), states.transform.combine(getTransform()));
	}
}