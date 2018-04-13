#include "rectangleShapeNode.h"
#include <iostream>
namespace game {
	RectangleShapeNode::RectangleShapeNode(std::shared_ptr<sf::RectangleShape> rectangleShape) {
		this->rectangleShape = std::move(rectangleShape);
	}

	RectangleShapeNode::~RectangleShapeNode() {
		
	}

	bool RectangleShapeNode::containsPoint(const sf::Vector2f & point) const {
		return rectangleShape->getGlobalBounds().contains(point);
	}

	std::shared_ptr<sf::Drawable> RectangleShapeNode::getDrawable() {
		return rectangleShape;
	}
}
