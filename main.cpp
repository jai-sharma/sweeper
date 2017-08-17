#include "tile.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

#define MAP_SIZE 20
#define WINDOW_SIZE 400
#define NUM_MINES 40
#define TEXTURE_PATH "numbers.png"

bool gameOver = false;

Tile tileMap[MAP_SIZE][MAP_SIZE];
Tile *mines[NUM_MINES];
int coveredTiles = MAP_SIZE * MAP_SIZE;

sf::Texture numbersTexture;

void setNumTexture(int num, Tile *t) {
	t->tileShape.setTexture(&numbersTexture);
	switch (num) {
		case 1:
			t->tileShape.setTextureRect(sf::IntRect(0,0,15,15));
			break;
		case 2:
			t->tileShape.setTextureRect(sf::IntRect(17,0,15,15));
			break;
		case 3:
			t->tileShape.setTextureRect(sf::IntRect(34,0,15,15));
			break;
		case 4:
			t->tileShape.setTextureRect(sf::IntRect(51,0,15,15));
			break;
		case 5:
			t->tileShape.setTextureRect(sf::IntRect(68,0,15,15));
			break;
		case 6:
			t->tileShape.setTextureRect(sf::IntRect(85,0,15,15));
			break;
		case 7:
			t->tileShape.setTextureRect(sf::IntRect(102,0,15,15));
			break;
		case 8:
			t->tileShape.setTextureRect(sf::IntRect(119,0,15,15));
			break;
		default:
			break;
	}
}

// Making random numbers lol
// https://msdn.microsoft.com/en-us/library/bb982398.aspx
std::random_device rd;
std::mt19937 gen(rd());
// Range for index values of tileMap
std::uniform_int_distribution<> dist(0, MAP_SIZE - 1);

// Returns rand int between 0 and MAP_SIZE - 1 inclusive
// Used to find random index values for the tileMap
int mapRandInt() {
	return dist(gen);
}

// Put mines on the map
void placeMines() {
	for (int m = 0; m < NUM_MINES; m++) {
		// Get random postion
		int i = mapRandInt();
		int j = mapRandInt();
	
		// If it is already a mine then keep getting new postions until it isnt
		while (tileMap[i][j].isMine) {
			i = mapRandInt();
			j = mapRandInt();
		}

		// Set this position as a mine
		tileMap[i][j].isMine = true;

		// Add to the mine array
		mines[m] = &tileMap[i][j];
	}
}

void resetMap() {
	// Set tile map positions
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			tileMap[i][j].initPosition(i, j);
		}
	}
	
	// Fill it with new mines
	placeMines();

	gameOver = false;
}

void showMines(bool win) {
	if (win) {
		for (int i = 0; i < NUM_MINES; i++) {
			mines[i]->green();
		}
	}
	for (int i = 0; i < NUM_MINES; i++) {
		mines[i]->uncover();
	}
}

void winGame() {
	showMines(true);
	gameOver = true;
}

void loseGame() {
	showMines(false);
	gameOver = true;
}

// Returns an minimal list of pointers to ajacent tiles
std::vector<Tile*> adjacentTiles(int i, int j) {
	// Adjacent tiles to check for mines form a 3x3 matrix with the given tile
	// in the center. Therefore check tiles with indexes from
	// (i - 1), (j - 1)	to (i + 1), (j + 1)
	int is = i - 1, ie = i + 2;
	int js = j - 1, je = j + 2;
	std::vector<Tile*> out;
	for (; is < ie; is++) {
		for (; js < je; js++) {
			// Array oob checks
			if (is < 0 || js < 0) {
				continue;
			}
			if (is > MAP_SIZE - 1 || js > MAP_SIZE - 1) {
				continue;
			}

			// Skip already uncovered tiles
			if (!tileMap[is][js].isCovered) {
				continue;
			}

			// Set tile pointer
			out.push_back(&tileMap[is][js]);
		}
		js = j - 1;
	}
	return out;
}

// Calculates and sets the mine proximity for a given tile
void setMineProximity(int i, int j) {
	// Mine proximity count
	int proxy = 0;
	// Get list of adjacent tile
	std::vector<Tile*> adj = adjacentTiles(i, j);
	for (Tile *t : adj) {
		if (t->isMine) {
			proxy++;
		}
	}
	// set proximity count for given position
	tileMap[i][j].mineProximity = proxy;
	if (proxy > 0) {
		setNumTexture(proxy, &tileMap[i][j]);
	}
}

// Recursive function to reveal adjacent non mine tiles
void cascade(int i, int j) {
	// Check proximity for this tile
	setMineProximity(i, j);
	// If this tile has mines around stop
	if (tileMap[i][j].mineProximity > 0) {
		return;
	}
	// If no mines around, uncover all adjacent tiles and recurse
	std::vector<Tile*> adj = adjacentTiles(i, j);
	for (Tile *t : adj) {
		if (t->mineProximity == 0) 
		{
			t->uncover();
			coveredTiles--;
			cascade(t->i, t->j);
		}
	}
}

void uncoverTile(int i, int j) {
	Tile *thisTile = &tileMap[i][j];
	thisTile->uncover();
	coveredTiles--;

	if (thisTile->isMine) {
		loseGame();
		return;
	}

	if (coveredTiles == NUM_MINES) {
		winGame();
		return;
	}

	// Cascade to reveal other tiles
	cascade(i,j);
}

void handleClick(sf::Event &e) {
	if (gameOver) {
		return;
	}

	int x = e.mouseButton.x;
	int y = e.mouseButton.y;

	// Ignore out of bounds clicks
	if (x > WINDOW_SIZE || y > WINDOW_SIZE) {
		return;
	}

	// Get tile indexes for array
	int tileI = x / TILE_TOTAL_SIZE;
	int tileJ = y / TILE_TOTAL_SIZE;

	switch (e.mouseButton.button) {
		// Left click to uncover
		case sf::Mouse::Left:
			uncoverTile(tileI, tileJ);
			break;

		// Right click to mark
		case sf::Mouse::Right:
			tileMap[tileI][tileJ].mark();
			break;

		default:
			break;
	}
}

int main() {
	sf::RenderWindow window(
		sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), // Window size
		"sweeper",                               // Window title
		sf::Style::Titlebar             // Show titlebar (doesn't mess up i3wm)
	);

	window.setVerticalSyncEnabled(true);

	// Set it up
	resetMap();
	numbersTexture.loadFromFile(TEXTURE_PATH);

	sf::Event event;
	while (window.isOpen()) {

		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;

				// Prevent window resizing
				case sf::Event::Resized:
					window.setSize(sf::Vector2u(WINDOW_SIZE, WINDOW_SIZE));

				case sf::Event::KeyPressed:
					// (R)eset
					if (event.key.code == sf::Keyboard::R) {
						resetMap();
					}
					// Just end it now fam
					if (event.key.code == sf::Keyboard::Escape) {
						loseGame();
					}
					break;


				// Clicks
				case sf::Event::MouseButtonReleased:
					handleClick(event);
					break;

				default:
					break;
			}
		}

		window.clear();

		// Draw all the tiles
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				window.draw(tileMap[i][j]);
			}
		}

		window.display();
	}

	return 0;
}
