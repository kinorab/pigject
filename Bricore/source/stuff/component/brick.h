#pragma once
#include "../../interact/interactiveObject.h"
#include "../../definition/diagonalPoint.h"
#include <memory>

namespace sf {
	class RenderTarget;
	class RenderStates;
	class RectangleShape;
	class Color;
}

namespace item {
	class Globular;

	class Brick :
		public game::InteractiveObject
		, public std::enable_shared_from_this<Brick> {
	public:
		// origin on center
		explicit Brick(const sf::Vector2f & size, const float frameSize = 0);
		explicit Brick(const Brick &copy);
		virtual bool containsPoint(const sf::Vector2f & point) const override;
		virtual std::shared_ptr<sf::Drawable> getDrawable() const override;
		void update(const float updateRatio);
		void loadTexture(const std::string & fileName);
		void setPosition(const sf::Vector2f & position);
		void setSize(const sf::Vector2f & size);
		void setFillColor(const sf::Color & color);
		void setFrameSize(const float frame);
		void setFrameColor(const sf::Color & color);
		void setSpeed(const sf::Vector2f & speed);

		sys::DPointf getDP() const;
		sf::FloatRect getGlobalBounds() const;
		const sf::Vector2f & getBrickSize() const;
		const sf::Color & getBrickColor() const;
		float getFrameSize() const;
		const sf::Color & getFrameColor() const;
		const sf::Vector2f & getPosition() const;
		Brick & operator =(Brick right);
		virtual ~Brick();

	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

		sf::Vector2f speed;
		std::shared_ptr<sf::RectangleShape> brick;
	};
}
