#include "interactiveObject.h"
#include "../event/UIEvent.h"
#include "../event/eventListener.h"
#include "../event/dispatchHelper.h"
#include "container.h"
#include "../definition/utility.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace game {
	InteractiveObject::InteractiveObject() :
		bEnabled(true) {
		onEnabled();
	}

	InteractiveObject::~InteractiveObject() {
	}

	void InteractiveObject::dispatchEvent(UIEvent & event) {
		DispatchHelper helper(event);
		helper.setCurrentTarget(this);

		if (event.getPhase() == EventPhase::NONE) {
			helper.setTarget(this);
			helper.setPhase(EventPhase::AT_TARGET);
		}

		dispatchEvent(static_cast<Event &>(event));
		if (event.getPhase() == EventPhase::AT_TARGET) {
			helper.setPhase(EventPhase::BUBBLING_PHASE);
		}

		if (event.getPhase() == EventPhase::BUBBLING_PHASE
			&& parent != nullptr
			&& !helper.isPropagationStopped()) {
			parent->dispatchEvent(event);
		}

		if (event.getTarget() == this) {
			helper.setPhase(EventPhase::NONE);
			helper.setCurrentTarget(nullptr);
		}
	}

	void InteractiveObject::dispatchEvent(UIEvent && event) {
		dispatchEvent(event);
	}

	bool InteractiveObject::getEnabled() const {
		return bEnabled;
	}

	Container * InteractiveObject::getParent() {
		return parent;
	}

	void InteractiveObject::onDisabled() {
	}

	void InteractiveObject::onEnabled() {
	}

	void InteractiveObject::setEnabled(bool value) {
		if (value == bEnabled) {
			return;
		}

		if (value) {
			onEnabled();
		}
		else {
			onDisabled();
		}

		bEnabled = value;
	}

	void InteractiveObject::setParent(Container * container) {
		parent = container;
	}

	void InteractiveObject::tryUpdate(const float updateRatio) {
		if (getEnabled()) {
			update(updateRatio);
		}
	}

	void InteractiveObject::tryHandle(const sf::Event & event) {
		if (getEnabled()) {
			handle(event);
		}
	}

	void InteractiveObject::update(const float) {
	}

	void InteractiveObject::handle(const sf::Event & event) {
	}

	void InteractiveObject::draw(sf::RenderTarget & target, sf::RenderStates states) const {
		target.draw(*getDrawable(), states.transform.combine(getTransform()));
	}
}