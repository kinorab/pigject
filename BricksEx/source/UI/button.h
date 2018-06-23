#pragma once

#include "UIComponent.h"

namespace game {
	class MouseMovedEvent;
	class MousePressedEvent;
	class MouseReleasedEvent;
	class Button
		: public InteractiveObject {
	public:
		Button(std::shared_ptr<sf::Sprite> upObject, std::shared_ptr<sf::Drawable> overObject, std::shared_ptr<sf::Drawable> downObject);
		virtual ~Button();
		virtual bool containsPoint(const sf::Vector2f & point) const override;
		virtual std::shared_ptr<sf::Drawable> getDrawable() const override;
	private:
		enum class ButtonState {
			UP,
			OVER,
			DOWN
		};
		ButtonState currentState;
		std::shared_ptr<sf::Drawable> upObject;
		std::shared_ptr<sf::Drawable> overObject;
		std::shared_ptr<sf::Drawable> downObject;
		std::shared_ptr<InteractiveObject> hitObject;
		virtual void onMouseEntered();
		virtual void onMouseLeft();
		virtual void onMousePressed(MousePressedEvent & event);
		virtual void onMouseReleased(MouseReleasedEvent & event);
	};
}