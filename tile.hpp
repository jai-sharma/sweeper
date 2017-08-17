#pragma once

#include <SFML/Graphics.hpp>

// Tile size
#define TILE_SIZE 18
#define TILE_BORDER 1
#define TILE_TOTAL_SIZE 20

// Tile colours
#define GREY 150, 150, 150
#define DARK_GREY 80, 80, 80

class Tile : public sf::Drawable {
public:
	sf::RectangleShape tileShape;
	bool isMine = false;
	bool isCovered = true;
	bool isMarked = false;
	int mineProximity = 0;
	int i, j;

	Tile();
	void initPosition(int x, int y);
	void uncover();
	void mark();
	void green();

private:
	
	void reset();
	void setColorFromState();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
