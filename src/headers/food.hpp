#pragma once

#include "thing.hpp" 

class Level;

class Food : public Thing
{
public:
	Food(int x, int y, bool food);
	bool update(uint8_t *pixels, Level *lvl);
	void randomize(Level *lvl);

public:
	bool food;
	TileType type = TileType::Food;
	uint32_t color = 0xFF6E9055;
};
