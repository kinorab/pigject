#include "define.h"
#include "brick.h"
#include <stdexcept>
#include <iostream>

// user set the brick array manually
Brick::Brick(const size_t row, const size_t col, const float wid, const float hei, Window *window, const Vector2f &interval){

	try {
		if (row <= 0 || col <= 0 || wid <= 0 || hei <= 0 || interval.x < 0.0f || interval.y < 0.0f) {
			throw domain_error("Invaild brick initialization.");
		}
		else if (col * wid + interval.x * (col + 1) > window->getSize().x
			  || row * hei + interval.y * (row + 1) > window->getSize().y) {
			throw domain_error("Invaild brick initialization.");
		}
		else {
			area.resize(row * col);
			amount = col;
			sideLength = Vector2f(wid, hei);
			this->interval = interval;

			if (getAreaSize() == row * amount) {
				settlePlace(window);
			}
			else {
				throw out_of_range("The subscripts are out of range.");
			}
		}
	}
	catch (domain_error &ex) {
		cout << "Exception: " << ex.what() << endl;
	}
	catch (out_of_range &ex) {
		cout << "Exception: " << ex.what() << endl;
	}
}

// auto full up the window
Brick::Brick(const size_t rowAmount, const float wid, const float hei, Window * window, const Vector2f &interval){

	try {
		if (rowAmount <= 0 || wid <= 0 || hei <= 0 || interval.x < 0.0f || interval.y < 0.0f) {
			throw domain_error("Invaild brick initialization.");
		}
		else {
			sideLength = Vector2f(wid, hei);
			this->interval = interval;
			amount = static_cast<size_t>((window->getSize().x - getInterval().x) / (getInterval().x + getSideLength().x));
			area.resize(rowAmount * amount);

			if (getAreaSize() == rowAmount * amount) {
				settlePlace(window);
			}
			else {
				throw out_of_range("The subscripts are out of range.");
			}
		}
	}
	catch (domain_error &ex) {
		cout << "Exception: " << ex.what() << endl;
	}
	catch (out_of_range &ex) {
		cout << "Exception: " << ex.what() << endl;
	}
}

void Brick::loadImage(Texture *image){
	for (size_t i = 0; i < getAreaSize(); ++i) {
		area.at(i).setTexture(image);
	}
}

void Brick::fillEntityColor(const Color &color){
	for (size_t i = 0; i < getAreaSize(); ++i) {
		area.at(i).setFillColor(color);
	}
}

void Brick::setRowAmount(const size_t row, Window *window) {

	if (row > 0) {
		area.resize(row * amount);
		changeEntity = true;
		settlePlace(window);
	}
	else {
		cout << "Invalid area setting." << endl;
	}
}

void Brick::setInterval(const Vector2f &interval, Window *window) {

	if (interval.x >= 0.0f && interval.y >= 0.0f) {
		this->interval = interval;
		settlePlace(window);
	}
	else {
		cout << "Invalid interval setting." << endl;
	}
}

void Brick::setInterval(const float x, const float y, Window *window) {

	if (x >= 0.0f && y >= 0.0f) {
		interval = Vector2f(x, y);
		settlePlace(window);
	}
	else {
		cout << "Invalid interval setting." << endl;
	}
}

void Brick::setSideLength(const Vector2f & sideLength, Window * window){

	if (sideLength.x > 0 && sideLength.y > 0) {
		this->sideLength = sideLength;
		changeEntity = true;
		settlePlace(window);
	}
	else {
		cout << "Invalid side-length setting." << endl;
	}
}

void Brick::setSideLength(const float wid, const float hei, Window *window){

	if (wid > 0 && hei > 0) {
		sideLength = Vector2f(wid, hei);
		changeEntity = true;
		settlePlace(window);
	}
	else {
		cout << "Invalid side-length setting." << endl;
	}
}

const size_t Brick::getAreaSize() const {
	return getArea().size();
}

const Vector2f & Brick::getSideLength() const{
	return sideLength;
}

const Vector2f & Brick::getInterval() const{
	return interval;
}

void Brick::settlePlace(Window *window){

	static float whiteSpace = (window->getSize().x - ((interval.x + sideLength.x) * amount + interval.x)) / 2;
	// if window's size().x cannot be filled with full screen, remain the white space of bound
	static Vector2f initialPos(whiteSpace + interval.x + sideLength.x / 2, interval.y + sideLength.y / 2);
	static Vector2f tempPos = Vector2f(initialPos.x, initialPos.y);
	static size_t tempCount = 1;

	try {
		if (changeEntity == true) {
			for (size_t i = 0; i < area.size(); ++i) {
				area.at(i).setSize(Vector2f(sideLength.x, sideLength.y));
				// center origin position in every brick
				area.at(i).setOrigin(Vector2f(area.at(i).getSize().x / 2, area.at(i).getSize().y / 2));
			}
			// cover all attributes again
			whiteSpace = (window->getSize().x - ((interval.x + sideLength.x) * amount + interval.x)) / 2;
			initialPos = Vector2f(whiteSpace + interval.x + sideLength.x / 2, interval.y + sideLength.y / 2);
			tempPos = Vector2f(initialPos.x, initialPos.y);
			changeEntity = false;
		}

		// ensure that total with the intervals should not be out of screen
		if (whiteSpace >= 0.0f) {
			// start placing area array
			for (size_t i = 0; i < area.size(); ++i) {

				area.at(i).setPosition(tempPos);
				if (tempCount < amount) {
					tempPos += Vector2f(sideLength.x + interval.x, 0);
					++tempCount;
				}
				else {
					tempPos = Vector2f(initialPos.x, tempPos.y + sideLength.y + interval.x);
					tempCount = 1;
				}
			}
		}
		else {
			cout << "Intervals are too long to place." << endl;
		}
	}
	catch (out_of_range &ex) {
		cout << "Exception: " << ex.what() << endl;
	}
}

const vector<RectangleShape> Brick::getArea() const{
	return area;
}

void Brick::draw(RenderTarget &target, RenderStates states) const{

	for (size_t i = 0; i < getAreaSize(); ++i) {
		states.texture = area.at(i).getTexture();
		target.draw(area.at(i), states);
	}
}
