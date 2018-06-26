#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace sf {
	class RenderTarget;
	class RenderStates;
}

class SubPlayer :
	public sf::Drawable
	, public sf::Transformable {
public:
	explicit SubPlayer();
	virtual void update(const float intervalRate);
	virtual ~SubPlayer();

private:
	void draw(sf::RenderTarget &, sf::RenderStates) const override;

};