#include "tile.hpp"
#include <SFML/Graphics.hpp>

Tile::Tile() {
	tileShape = sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
}

// Set inital postion of tiles relative to their array index
void Tile::initPosition(int i, int j) {
	this->i = i;
	this->j = j;
	int new_x = (i * TILE_TOTAL_SIZE) + TILE_BORDER;
	int new_y = (j * TILE_TOTAL_SIZE) + TILE_BORDER;
	tileShape.setPosition(new_x, new_y);
	
	// Reset attributes internally
	reset();
}

// Reset attributes
void Tile::reset() {
	isMine = false;
	isCovered = true;
	isMarked = false;
	mineProximity = 0;
	setColorFromState();
	tileShape.setTexture(NULL);
}

void Tile::mark() {
	isMarked = true;
	setColorFromState();
}

void Tile::uncover() {
	isCovered = false;
	setColorFromState();
}

void Tile::green() {
	tileShape.setFillColor(sf::Color::Green);
}

void Tile::setColorFromState() {
	if (isCovered) {
		if (isMarked) {
			tileShape.setFillColor(sf::Color::Yellow);
		} else {
			tileShape.setFillColor(sf::Color(GREY));
		}
	} else {
		if (isMine) {
			tileShape.setFillColor(sf::Color::Red);
		} else {
			tileShape.setFillColor(sf::Color(DARK_GREY));
		}
	}
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(tileShape);
}
