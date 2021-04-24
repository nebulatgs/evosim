#pragma once

#include "tile.hpp"

class Level;

class Thing
{
public:
	Thing(int x, int y, TileType type);
	~Thing();
	void randomize();
	virtual bool update(uint8_t *pixels, Level *lvl);

public:
	v2d pos;
	uint32_t color;
	TileType type;
	std::vector<Tile> tiles;
};
