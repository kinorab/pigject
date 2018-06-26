#include "brick.h"
#include "../../definition/gameState.h"
#include "../../definition/utility.h"
#include "../../manager/textureManager.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace item;

Brick::Brick(const sf::Vector2f & size, const float frameSize)
	: brick(new RectangleShape){
	setBrickSize(size);
	brick->setOrigin(brick->getSize() / 2.f);
	setFrameSize(frameSize);
}

Brick::Brick(const Brick & copy) 
	: brick(new RectangleShape(*copy.brick)){
}

void Brick::update(const float updateRatio) {

}

void Brick::loadTexture(const std::string & fileName) {
	brick->setTexture(TextureManager::getInstance().get(fileName));
}

void Brick::setPosition(const sf::Vector2f & position) {
	brick->setPosition(position);
}

void Brick::setBrickColor(const Color &color) {
	brick->setFillColor(color);
}

void Brick::setFrameColor(const Color &color) {
	if (getFrameSize() <= 0.0f) throw std::invalid_argument("Brick frames do not exist.");
	brick->setOutlineColor(color);
}

void Brick::setBrickSize(const Vector2f & sideLength) {
	if (sideLength.x <= 0 || sideLength.y <= 0) throw std::invalid_argument("Side-length cannot be negative.");
	brick->setSize(sideLength);
}

void Brick::setFrameSize(const float frameSize) {
	if (frameSize < 0.0f) throw std::invalid_argument("Frame size cannot be negative.");
	brick->setOutlineThickness(frameSize);
}

const Vector2f & Brick::getBrickSize() const {
	return brick->getSize();
}

float Brick::getFrameSize() const {
	return brick->getOutlineThickness();
}

sys::DPointf Brick::getDP() const {
	return sys::DPointf(brick->getGlobalBounds());
}

FloatRect Brick::getGlobalBounds() const {
	return brick->getGlobalBounds();
}

const sf::Color & Brick::getBrickColor() const {
	return brick->getFillColor();
}

const Color & Brick::getFrameColor() const {
	return brick->getOutlineColor();
}

const Vector2f & Brick::getPosition() const {
	return brick->getPosition();
}

Brick::~Brick() { }

Brick & Brick::operator=(Brick right) {
	brick.swap(right.brick);
	return *this;
}

void Brick::draw(RenderTarget &target, RenderStates states) const {
	states.transform *= getTransform();
	target.draw(*brick, states);
}